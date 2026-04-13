#!/usr/bin/env python3
#o remetente assina a mensagem antes de a cifrar, 
# e o destinatário verifica a assinatura da chave DH do outro para confirmar a sua identidade
#Porque se fizesses ao contrário — cifrar primeiro, assinar depois — a assinatura estaria "por fora" do envelope, 
# o que significa que qualquer pessoa podia retirar a assinatura de Alice e substituir pela sua própria, sem quebrar a cifra.
import sys
import os
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh, rsa, padding as asym_padding
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.ciphers.aead import ChaCha20Poly1305


# Parâmetros DH (RFC 3526 Grupo 14)
P = 0xFFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF
G = 2


def get_dh_parameters():
    return dh.DHParameterNumbers(P, G).parameters()


def mkpair(x, y):
    len_x = len(x)
    len_x_bytes = len_x.to_bytes(2, 'little')
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
        info=b'session key',
    ).derive(shared_secret)


def setup(user):
    """Gera pares de chaves DH e RSA para utilizador"""
    #gera 4 ficheiros: dhsk, dhpk, rsask, rsapk
    #O setup gera quatro ficheiros de chaves: o par DH (.dhsk e .dhpk) como antes, 
    # mas agora também um par RSA (.rsask e .rsapk). O passo novo aqui é que a chave pública DH é assinada com a chave privada RSA 
    # antes de ser gravada no .dhpk, para provar que pertence mesmo a esse utilizador.
    
    # No setup estamos a assinar a chave pública DH com a chave privada RSA do próprio utilizador
    # E gravamos esse par (chave DH + assinatura) no ficheiro .dhpk. A ideia é que quando Alice for ler a chave pública DH de Bob,
    # consegue verificar que aquela chave foi mesmo gerada pelo Bob — e não foi substituída por um atacante no meio

    # 1. Gerar par DH
    parameters = get_dh_parameters()
    dh_private_key = parameters.generate_private_key()
    dh_public_key = dh_private_key.public_key()
    
    # 2. Gerar par RSA
    rsa_private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
    )
    rsa_public_key = rsa_private_key.public_key()
    
    # 3. Serializar chave pública DH
    dh_public_pem = dh_public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    # 4. ASSINAR a chave pública DH com RSA
    signature = rsa_private_key.sign(
        dh_public_pem,
        asym_padding.PSS(
            mgf=asym_padding.MGF1(hashes.SHA256()),
            salt_length=asym_padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )
    
    # E faz uma coisa extra importante: assina a chave pública DH com a chave privada RSA,
    # e guarda esse par (chave DH + assinatura) no .dhpk. 
    # Isto serve para provar que a chave DH pertence realmente a este utilizador.

    # 5. Juntar chave pública DH + assinatura
    dh_public_signed = mkpair(dh_public_pem, signature)
    
    # 6. Serializar chaves privadas
    dh_private_pem = dh_private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )
    
    rsa_private_pem = rsa_private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )
    
    rsa_public_pem = rsa_public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    # 7. Gravar ficheiros
    with open(f"{user}.dhsk", 'wb') as f:
        f.write(dh_private_pem)
    
    with open(f"{user}.dhpk", 'wb') as f:
        f.write(dh_public_signed)
    
    with open(f"{user}.rsask", 'wb') as f:
        f.write(rsa_private_pem)
    
    with open(f"{user}.rsapk", 'wb') as f:
        f.write(rsa_public_pem)
    
    print(f"Chaves geradas para '{user}':")
    print(f"  - {user}.dhsk (chave privada DH)")
    print(f"  - {user}.dhpk (chave pública DH + assinatura)")
    print(f"  - {user}.rsask (chave privada RSA)")
    print(f"  - {user}.rsapk (chave pública RSA)")


def encrypt(recipient, sender, filename):
    """Alice cifra ficheiro para Bob (autenticado)"""
    # O passo novo é que antes de cifrar, assina a mensagem com a chave privada RSA do remetente e junta mensagem 
    # e assinatura com mkpair. Só depois cifra tudo isso com ChaCha20-Poly1305.
    
    # no enc — Alice assina a mensagem com a sua chave privada RSA antes de a cifrar, e junta a mensagem com a assinatura.
    
    # 1. Ler chave pública DH de Bob (com assinatura)
    with open(f"{recipient}.dhpk", 'rb') as f:
        bob_dh_signed = f.read()
    
    bob_dh_public_pem, bob_dh_signature = unpair(bob_dh_signed)
    
    # 2. Ler chave pública RSA de Bob
    with open(f"{recipient}.rsapk", 'rb') as f:
        bob_rsa_public_pem = f.read()
    
    bob_rsa_public_key = serialization.load_pem_public_key(bob_rsa_public_pem)
    
    # 3. VERIFICAR assinatura da chave DH de Bob é legitima
    # lê o .dhpk de Bob, separa a chave DH da assinatura,
    # e verifica a assinatura com a chave pública RSA de Bob. Se falhar, aborta com erro.
    try:
        bob_rsa_public_key.verify(
            bob_dh_signature,
            bob_dh_public_pem,
            asym_padding.PSS(
                mgf=asym_padding.MGF1(hashes.SHA256()),
                salt_length=asym_padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        print("Chave DH de Bob verificada")
    except:
        print("ERRO: Assinatura da chave DH inválida")
        sys.exit(1)
    
    # 4. Carregar chave pública DH de Bob
    bob_dh_public_key = serialization.load_pem_public_key(bob_dh_public_pem)
    
    # 5. Gerar par efémero para Alice
    parameters = get_dh_parameters()
    alice_private_key = parameters.generate_private_key()
    alice_public_key = alice_private_key.public_key()
    
    # 6. Acordo DH
    shared_secret = alice_private_key.exchange(bob_dh_public_key)
    session_key = derive_session_key(shared_secret)
    
    # 7. Ler mensagem
    with open(filename, 'rb') as f:
        plaintext = f.read()
    
    # 8. Ler chave privada RSA de Alice
    with open(f"{sender}.rsask", 'rb') as f:
        alice_rsa_private_pem = f.read()
    
    alice_rsa_private_key = serialization.load_pem_private_key(
        alice_rsa_private_pem,
        password=None
    )
    
    # 9. SIGN-THEN-ENCRYPT: Assinar mensagem ANTES de cifrar
    # Assina a mensagem com a sua própria chave RSA privada — antes de cifrar, 
    # junta a mensagem com a sua assinatura. Assim Bob pode confirmar depois que foi Alice quem enviou.
    signature = alice_rsa_private_key.sign(
        plaintext, #recebe mensagem
        asym_padding.PSS( #e produz uma sequência de bytes que só podia ter sido gerada por quem tem a chave privada RSA de Alice
            mgf=asym_padding.MGF1(hashes.SHA256()),
            salt_length=asym_padding.PSS.MAX_LENGTH
        ),
        hashes.SHA256()
    )
    
    #Esta ordem — assinar primeiro, cifrar depois  
    # é a recomendada. Protege a assinatura dentro do envelope cifrado.
    
    # 10. Juntar mensagem + assinatura
    plaintext_with_sig = mkpair(plaintext, signature)
    
    # 11. Cifrar tudo
    nonce = os.urandom(12)
    cipher = ChaCha20Poly1305(session_key)
    ciphertext = cipher.encrypt(nonce, plaintext_with_sig, None)
    
    # 12. Serializar chave pública de Alice
    alice_public_pem = alice_public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    # 13. Criar ficheiro cifrado
    encrypted_data = mkpair(alice_public_pem, nonce + ciphertext)
    
    # 14. Gravar
    with open(filename + '.enc', 'wb') as f:
        f.write(encrypted_data)
    
    print(f"Ficheiro cifrado e assinado: {filename}.enc")


def decrypt(recipient, sender, filename):
    """Bob decifra ficheiro de Alice (verifica autenticidade)"""
    # O dec faz o caminho inverso: deriva a chave de sessão pelo acordo DH, decifra o criptograma, 
    # usa unpair para separar a mensagem da assinatura, e verifica a assinatura com a chave pública RSA do remetente
    # se falhar, aborta com erro em vez de devolver a mensagem.
    
    # no dec — Bob verifica essa assinatura depois de decifrar, para confirmar que foi mesmo Alice que enviou a mensagem

    # 1. Ler chave privada DH de Bob
    with open(f"{recipient}.dhsk", 'rb') as f:
        bob_dh_private_pem = f.read()
    
    bob_dh_private_key = serialization.load_pem_private_key(
        bob_dh_private_pem,
        password=None
    )
    
    # 2. Ler chave pública RSA de Alice
    with open(f"{sender}.rsapk", 'rb') as f:
        alice_rsa_public_pem = f.read()
    
    alice_rsa_public_key = serialization.load_pem_public_key(alice_rsa_public_pem)
    
    # 3. Ler ficheiro cifrado
    with open(filename, 'rb') as f:
        encrypted_data = f.read()
    
    # 4. Extrair componentes
    alice_public_pem, nonce_and_ciphertext = unpair(encrypted_data)
    alice_public_key = serialization.load_pem_public_key(alice_public_pem)
    
    # 5. Acordo DH
    shared_secret = bob_dh_private_key.exchange(alice_public_key)
    session_key = derive_session_key(shared_secret)
    
    # 6. Extrair nonce e ciphertext
    nonce = nonce_and_ciphertext[:12]
    ciphertext = nonce_and_ciphertext[12:]
    
    # 7. Decifrar
    cipher = ChaCha20Poly1305(session_key)
    plaintext_with_sig = cipher.decrypt(nonce, ciphertext, None)
    
    # 8. Separar mensagem e assinatura
    # Depois de decifrar, Bob separa a mensagem da assinatura
    # e verifica se foi realmente Alice que assinou, usando a chave pública RSA de Alice.
    #Se a verificação falhar (mensagem adulterada, ou assinatura não corresponde a Alice), o programa aborta com erro.
    plaintext, signature = unpair(plaintext_with_sig)
    
    # 9. VERIFICAR assinatura de Alice
    try:
        alice_rsa_public_key.verify(
            signature,
            plaintext,
            asym_padding.PSS(
                mgf=asym_padding.MGF1(hashes.SHA256()),
                salt_length=asym_padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        print("Assinatura de Alice verificada!")
    except:
        print("ERRO: Assinatura inválida! Mensagem pode ter sido adulterada!")
        sys.exit(1)
    
    # 10. Gravar
    with open(filename + '.dec', 'wb') as f:
        f.write(plaintext)
    
    print(f"Ficheiro decifrado: {filename}.dec")


if __name__ == '__main__':
    command = sys.argv[1]
    
    if command == 'setup':
        user = sys.argv[2]
        setup(user)
    
    elif command == 'enc':
        recipient = sys.argv[2]
        sender = sys.argv[3]
        filename = sys.argv[4]
        encrypt(recipient, sender, filename)
    
    elif command == 'dec':
        recipient = sys.argv[2]
        sender = sys.argv[3]
        filename = sys.argv[4]
        decrypt(recipient, sender, filename)