
import sys
import os
import datetime
from cryptography import x509
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh, padding as asym_padding
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.ciphers.aead import ChaCha20Poly1305


P = 0xFFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF
G = 2


def get_dh_parameters():
    return dh.DHParameterNumbers(P, G).parameters()


def mkpair(x, y):
    len_x_bytes = len(x).to_bytes(2, 'little')
    return len_x_bytes + x + y


def unpair(xy):
    len_x = int.from_bytes(xy[:2], 'little')
    x = xy[2:len_x + 2]
    y = xy[len_x + 2:]
    return x, y


def derive_session_key(shared_secret):
    return HKDF(
        algorithm=hashes.SHA256(),
        length=32,
        salt=None,
        info=b'cfich-certs-session-key',
    ).derive(shared_secret)


def cert_load(fname):
    try:
        with open(fname, "rb") as f:
            return x509.load_pem_x509_certificate(f.read())
    except FileNotFoundError:
        print(f"Ficheiro '{fname}' não encontrado.")
        sys.exit(1)
    except Exception as e:
        print(f"Não foi possível ler certificado '{fname}': {e}")
        sys.exit(1)


def cert_validtime(cert, now=None):
    if now is None:
        now = datetime.datetime.now(tz=datetime.timezone.utc)
    if now < cert.not_valid_before_utc or now > cert.not_valid_after_utc:
        raise x509.verification.VerificationError(
            f"Certificado fora do período de validade "
            f"(válido de {cert.not_valid_before_utc} a {cert.not_valid_after_utc})"
        )


def cert_validsubject(cert, attrs=[]):
    for oid, expected in attrs:
        values = cert.subject.get_attributes_for_oid(oid)
        if not values or values[0].value != expected:
            raise x509.verification.VerificationError(
                f"Subject inválido: esperava '{expected}', obteve "
                f"'{values[0].value if values else 'nenhum'}'"
            )


def validate_cert(cert_file, ca_cert, expected_cn):
    cert = cert_load(cert_file)
    
    try:
        cert.verify_directly_issued_by(ca_cert)
    except Exception as e:
        raise x509.verification.VerificationError(
            f"Certificado não emitido pela CA de confiança: {e}"
        )
    
    cert_validtime(cert)
    cert_validsubject(cert, [(x509.NameOID.COMMON_NAME, expected_cn)])
    
    try:
        key_usage = cert.extensions.get_extension_for_oid(
            x509.ExtensionOID.KEY_USAGE
        ).value
        if not key_usage.digital_signature:
            raise x509.verification.VerificationError(
                "Não e permitimito assinaturas digitais"
            )
    except x509.ExtensionNotFound:
        pass
    
    return cert

def setup(user):
    print(f"A gerar chaves DH para '{user}'...")
    
    cert_file = f"{user}.crt"
    key_file = f"{user}.key"
    
    if not os.path.exists(cert_file):
        print(f"ERRO: Ficheiro '{cert_file}' não encontrado.")
        sys.exit(1)
    if not os.path.exists(key_file):
        print(f"ERRO: Ficheiro '{key_file}' não encontrado.")
        sys.exit(1)
    
    with open(key_file, "rb") as f:
        rsa_private_key = serialization.load_pem_private_key(
            f.read(), password=b"1234"
        )
    
    parameters = get_dh_parameters()
    dh_private_key = parameters.generate_private_key()
    dh_public_key = dh_private_key.public_key()
    
    dh_public_pem = dh_public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    signature = rsa_private_key.sign(
        dh_public_pem,
        asym_padding.PSS(
            mgf=asym_padding.MGF1(hashes.SHA256()),
            salt_length=asym_padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )
    
    dh_public_signed = mkpair(dh_public_pem, signature)
    
    dh_private_pem = dh_private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )
    
    with open(f"{user}.dhsk", 'wb') as f:
        f.write(dh_private_pem)
    
    with open(f"{user}.dhpk", 'wb') as f:
        f.write(dh_public_signed)
    
    print(f"Chaves DH geradas:")
    print(f"  - {user}.dhsk")
    print(f"  - {user}.dhpk")


def encrypt(recipient, sender, filename):
    ca_cert = cert_load("CA.crt")
    
    print(f"A validar certificado de {recipient}...")
    try:
        recipient_cert = cert_load(f"{recipient}.crt")
        recipient_cert.verify_directly_issued_by(ca_cert)
        cert_validtime(recipient_cert)
    except Exception as e:
        print(f"ERRO: Certificado de {recipient} inválido: {e}")
        sys.exit(1)
    print(f"Certificado de {recipient} válido.")
    
    recipient_rsa_pub = recipient_cert.public_key()
    
    dhpk_file = f"{recipient}.dhpk"
    if not os.path.exists(dhpk_file):
        print(f"ERRO: Ficheiro '{dhpk_file}' não encontrado.")
        print(f"Execute: python cfich_certs.py setup {recipient}")
        sys.exit(1)
    
    with open(dhpk_file, "rb") as f:
        recipient_dh_signed = f.read()
    
    recipient_dh_pem, recipient_dh_sig = unpair(recipient_dh_signed)
    
    try:
        recipient_rsa_pub.verify(
            recipient_dh_sig,
            recipient_dh_pem,
            asym_padding.PSS(
                mgf=asym_padding.MGF1(hashes.SHA256()),
                salt_length=asym_padding.PSS.MAX_LENGTH,
            ),
            hashes.SHA256(),
        )
        print(f"Assinatura da chave DH de {recipient} verificada.")
    except Exception as e:
        print(f"ERRO: Assinatura da chave DH de {recipient} inválida: {e}")
        sys.exit(1)
    
    recipient_dh_pub = serialization.load_pem_public_key(recipient_dh_pem)
    
    params = get_dh_parameters()
    sender_dh_priv = params.generate_private_key()
    sender_dh_pub = sender_dh_priv.public_key()
    
    shared_secret = sender_dh_priv.exchange(recipient_dh_pub)
    session_key = derive_session_key(shared_secret)
    
    try:
        with open(filename, "rb") as f:
            plaintext = f.read()
    except FileNotFoundError:
        print(f"ERRO: Ficheiro '{filename}' não encontrado.")
        sys.exit(1)
    
    try:
        with open(f"{sender}.key", "rb") as f:
            sender_rsa_priv = serialization.load_pem_private_key(
                f.read(), password=b"1234"
            )
    except FileNotFoundError:
        print(f"ERRO: Ficheiro '{sender}.key' não encontrado.")
        sys.exit(1)
    
    signature = sender_rsa_priv.sign(
        plaintext,
        asym_padding.PSS(
            mgf=asym_padding.MGF1(hashes.SHA256()),
            salt_length=asym_padding.PSS.MAX_LENGTH,
        ),
        hashes.SHA256(),
    )
    
    plaintext_with_sig = mkpair(plaintext, signature)
    
    nonce = os.urandom(12)
    cipher = ChaCha20Poly1305(session_key)
    ciphertext = cipher.encrypt(nonce, plaintext_with_sig, None)
    
    sender_dh_pub_pem = sender_dh_pub.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo,
    )
    
    with open(f"{sender}.crt", "rb") as f:
        sender_cert_pem = f.read()
    
    inner = mkpair(sender_dh_pub_pem, nonce + ciphertext)
    encrypted_data = mkpair(sender_cert_pem, inner)
    
    outfile = filename + ".enc"
    with open(outfile, "wb") as f:
        f.write(encrypted_data)
    
    print(f"Ficheiro cifrado: '{outfile}'")


def decrypt(recipient, sender, filename):
    ca_cert = cert_load("CA.crt")
    
    try:
        with open(filename, "rb") as f:
            encrypted_data = f.read()
    except FileNotFoundError:
        print(f"ERRO: Ficheiro '{filename}' não encontrado.")
        sys.exit(1)
    
    try:
        sender_cert_pem, inner = unpair(encrypted_data)
        sender_dh_pub_pem, nonce_ctxt = unpair(inner)
        nonce = nonce_ctxt[:12]
        ciphertext = nonce_ctxt[12:]
    except Exception as e:
        print(f"ERRO: Ficheiro cifrado com formato inválido: {e}")
        sys.exit(1)
    
    print(f"A validar certificado de {sender}...")
    sender_cert = x509.load_pem_x509_certificate(sender_cert_pem)
    try:
        sender_cert.verify_directly_issued_by(ca_cert)
        cert_validtime(sender_cert)
    except Exception as e:
        print(f"ERRO: Certificado do remetente inválido: {e}")
        sys.exit(1)
    print(f"Certificado de {sender} válido.")
    
    sender_rsa_pub = sender_cert.public_key()
    
    dhsk_file = f"{recipient}.dhsk"
    if not os.path.exists(dhsk_file):
        print(f"ERRO: Ficheiro '{dhsk_file}' não encontrado.")
        print(f"Execute: python cfich_certs.py setup {recipient}")
        sys.exit(1)
    
    with open(dhsk_file, "rb") as f:
        recipient_dh_priv = serialization.load_pem_private_key(
            f.read(), password=None
        )
    
    sender_dh_pub = serialization.load_pem_public_key(sender_dh_pub_pem)
    shared_secret = recipient_dh_priv.exchange(sender_dh_pub)
    session_key = derive_session_key(shared_secret)
    
    cipher = ChaCha20Poly1305(session_key)
    try:
        plaintext_with_sig = cipher.decrypt(nonce, ciphertext, None)
    except Exception as e:
        print(f"ERRO: Falha na decifragem: {e}")
        sys.exit(1)
    
    try:
        plaintext, signature = unpair(plaintext_with_sig)
    except Exception as e:
        print(f"ERRO: Formato interno inválido: {e}")
        sys.exit(1)
    
    try:
        sender_rsa_pub.verify(
            signature,
            plaintext,
            asym_padding.PSS(
                mgf=asym_padding.MGF1(hashes.SHA256()),
                salt_length=asym_padding.PSS.MAX_LENGTH,
            ),
            hashes.SHA256(),
        )
        print(f"Assinatura de {sender} verificada.")
    except Exception:
        print(f"ERRO: Assinatura INVÁLIDA!")
        sys.exit(1)
    
    outfile = filename + ".dec"
    with open(outfile, "wb") as f:
        f.write(plaintext)
    print(f"Ficheiro decifrado: '{outfile}'")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso:")
        print("  python3 cfich_certs.py setup <utilizador>")
        print("  python3 cfich_certs.py enc <destinatário> <remetente> <ficheiro>")
        print("  python3 cfich_certs.py dec <destinatário> <remetente> <ficheiro.enc>")
        sys.exit(1)
    
    command = sys.argv[1]
    
    if command == "setup":
        if len(sys.argv) != 3:
            print("Uso: cfich_certs.py setup <utilizador>")
            sys.exit(1)
        setup(sys.argv[2])
    
    elif command == "enc":
        if len(sys.argv) != 5:
            print("Uso: cfich_certs.py enc <destinatário> <remetente> <ficheiro>")
            sys.exit(1)
        encrypt(sys.argv[2], sys.argv[3], sys.argv[4])
    
    elif command == "dec":
        if len(sys.argv) != 5:
            print("Uso: cfich_certs.py dec <destinatário> <remetente> <ficheiro.enc>")
            sys.exit(1)
        decrypt(sys.argv[2], sys.argv[3], sys.argv[4])
    
    else:
        print(f"Comando desconhecido: '{command}'")
        print("Comandos disponíveis: setup, enc, dec")
        sys.exit(1)