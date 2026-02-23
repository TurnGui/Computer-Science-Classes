import sys
import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms
from cryptography.hazmat.backends import default_backend

def main():
    KEY_SIZE = 32 #256 bits para o chacha20
    NONCE_SIZE = 16

    args = sys.argv #ler o que escreveste no terminal

    if len(args) < 2: # não ser do tipo setup, enc ou dec
        sys.exit(1)

    op = args[1] # guarda operação em op

    if op == "setup":
        fkey = args[2]
        key = os.urandom(KEY_SIZE) # gera 32 bytes aleatórios
        with open(fkey, "wb") as f: #abre o ficheiro em escrita binária e mete e escreve la a key
            f.write(key)

    elif op == "enc":
        fich = args[2] #ficheiro pra cifrar
        fkey = args[3] #ficheiro da chave

        with open(fkey, "rb") as f:
            key = f.read() #le a chave

        nonce = os.urandom(NONCE_SIZE) #gera o nounce random

        cipher = Cipher(algorithms.ChaCha20(key, nonce), mode=None, backend=default_backend()) #cria o cipher com a chave e o nounce
        enc = cipher.encryptor() #cria o encryptor do cipher

        with open(fich, "rb") as f:
            ptxt = f.read() #abre o ficheiro com a mensagem em modo plaintext e lê

        ctxt = enc.update(ptxt) #encripta o plaintext

        with open(fich + ".enc", "wb") as f:
            f.write(nonce)
            f.write(ctxt) #manda para um ficheiro.enc o nounce e o texto encriptado

    elif op == "dec":
        fich = args[2] #ficheiro.enc
        fkey = args[3]

        with open(fkey, "rb") as f:
            key = f.read()

        with open(fich, "rb") as f:
            data = f.read()  #leu a chave e o ficheiro.enc

        nonce = data[:NONCE_SIZE] #vai buscar o nounce
        ctxt = data[NONCE_SIZE:] # e o resto do texto encriptado

        cipher = Cipher(algorithms.ChaCha20(key, nonce), mode=None, backend=default_backend()) #cria a mesma cifra de a pouco
        dec = cipher.decryptor() #mas agora cria um decriptor
        ptxt = dec.update(ctxt) # recupera o plaintext

        with open(fich + ".dec", "wb") as f:
            f.write(ptxt) #manda para um ficheiro.dec o plaintext decifrado

    else:
        print("Erro")

if __name__ == "__main__":
    main()