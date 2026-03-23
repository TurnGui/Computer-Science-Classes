import sys
import os
from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.ciphers.aead import ChaCha20Poly1305


# Parâmetros DH fixos (grupo 14 - RFC 3526)
P = 0xFFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF
G = 2


def get_dh_parameters():
    ## Se fosse para gerar parametros iria demorar muito tempo.
    ## Usando parametros padronizados irá ser instantâneo
    ## Gerar parametros G = 2  
    return dh.DHParameterNumbers(P, G).parameters()


def mkpair(x, y): # combina dois valores em um único byte string, prefixando o primeiro com seu comprimento
    len_x = len(x)
    len_x_bytes = len_x.to_bytes(2, 'little')
    return len_x_bytes + x + y


def unpair(xy): # extrai os dois valores de um byte string criado por mkpair
    len_x = int.from_bytes(xy[:2], 'little')
    x = xy[2:len_x + 2]
    y = xy[len_x + 2:]
    return x, y


def derive_session_key(shared_secret): # deriva uma chave de sessão a partir do segredo compartilhado usando HKDF
    return HKDF(
        algorithm=hashes.SHA256(),
        length=32,
        salt=None,
        info=b'session key',
    ).derive(shared_secret)


def setup(user): #cria um par de chaves DH para o usuário e as salva em arquivos .sk (chave privada) e .pk (chave pública)
    parameters = get_dh_parameters()
    
    # Gerar chave privada
    private_key = parameters.generate_private_key()
    public_key = private_key.public_key()
    
    # Serializar chave privada
    private_pem = private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )
    
    # Serializar chave pública
    public_pem = public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    # Gravar ficheiros
    with open(f"{user}.sk", 'wb') as f:
        f.write(private_pem)
    
    with open(f"{user}.pk", 'wb') as f:
        f.write(public_pem)
    
    print(f"Chaves geradas para '{user}':")
    print(f"  - {user}.sk (chave privada)")
    print(f"  - {user}.pk (chave pública)")


def encrypt(user, filename):
    # Ler chave pública do destinatário
    with open(f"{user}.pk", 'rb') as f:
        recipient_public_pem = f.read()
    
    recipient_public_key = serialization.load_pem_public_key(recipient_public_pem)
    
    # Gerar par de chaves efémero para Alice
    parameters = get_dh_parameters()
    alice_private_key = parameters.generate_private_key()
    alice_public_key = alice_private_key.public_key()
    
    # Derivar chave de sessão (acordo DH)
    shared_secret = alice_private_key.exchange(recipient_public_key)
    session_key = derive_session_key(shared_secret)
    
    # Ler ficheiro
    with open(filename, 'rb') as f:
        plaintext = f.read()
    
    # Cifrar com ChaCha20-Poly1305
    nonce = os.urandom(12)
    cipher = ChaCha20Poly1305(session_key)
    ciphertext = cipher.encrypt(nonce, plaintext, None)
    
    # Serializar chave pública de Alice
    alice_public_pem = alice_public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )
    
    # Criar ficheiro cifrado: [alice_pk][nonce + ciphertext]
    encrypted_data = mkpair(alice_public_pem, nonce + ciphertext)
    
    # Gravar
    output_file = filename + '.enc'
    with open(output_file, 'wb') as f:
        f.write(encrypted_data)
    
    print(f"Ficheiro cifrado: {output_file}")


def decrypt(user, filename):
    # Ler chave privada do destinatário
    with open(f"{user}.sk", 'rb') as f:
        private_pem = f.read()
    
    private_key = serialization.load_pem_private_key(private_pem, password=None)
    
    # Ler ficheiro cifrado
    with open(filename, 'rb') as f:
        encrypted_data = f.read()
    
    # Extrair componentes
    alice_public_pem, nonce_and_ciphertext = unpair(encrypted_data)
    
    # Carregar chave pública de Alice
    alice_public_key = serialization.load_pem_public_key(alice_public_pem)
    
    # Derivar chave de sessão
    shared_secret = private_key.exchange(alice_public_key)
    session_key = derive_session_key(shared_secret)
    
    # Extrair nonce e ciphertext
    nonce = nonce_and_ciphertext[:12]
    ciphertext = nonce_and_ciphertext[12:]
    
    # Decifrar
    cipher = ChaCha20Poly1305(session_key)
    plaintext = cipher.decrypt(nonce, ciphertext, None)
    
    # Gravar
    output_file = filename + '.dec'
    with open(output_file, 'wb') as f:
        f.write(plaintext)
    
    print(f"Ficheiro decifrado: {output_file}")


if __name__ == '__main__':
    command = sys.argv[1]
    
    if command == 'setup':
        user = sys.argv[2]
        setup(user)
    
    elif command == 'enc':
        user = sys.argv[2]
        filename = sys.argv[3]
        encrypt(user, filename)
    
    elif command == 'dec':
        user = sys.argv[2]
        filename = sys.argv[3]
        decrypt(user, filename)