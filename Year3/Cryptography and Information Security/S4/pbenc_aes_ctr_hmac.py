#!/usr/bin/env python3
import sys
import os
import getpass

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
from cryptography.hazmat.primitives import hashes, hmac
from cryptography.exceptions import InvalidSignature

SALT_SIZE = 16 #para PBKDF2 não gerar sempre as mm chaves
IV_SIZE = 16 #contador inicial no ctr ( 16 bytes pq AES tem bloco de 16 bytes)
TAG_SIZE = 32 #HMAC-SHA256 tem tag de 32 bytes
ITERS = 200_000 #número de iterações para PBKDF2 

def derive_keys(passphrase_bytes, salt): #Recebe a senha e o salt, gera as chaves de encriptação e autenticação
    # PBKDF2 gera 64 bytes e depois divide em 2 chaves de 32
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(), #diz que hash(misturador de chaves) é SHA256
        length=64, #tamanho total da chave gerada (da saida final , 32 + 32 para AES e HMAC)
        salt=salt, # um aletorio extra para a chave para garantir que a mesma senha gere chaves diferentes
        iterations=ITERS, #número de iterações para aumentar o custo computacional do ataque de força bruta
    )
    keymat = kdf.derive(passphrase_bytes) #executa a maquina
    enc_key = keymat[:32]   # AES-256 key (primeiros 32 bytes que sao a chave de encriptação)
    mac_key = keymat[32:]   # HMAC key (últimos 32 bytes que sao a chave de autenticação)
    return enc_key, mac_key

def main():
   
    op = sys.argv[1] #enc ou dec
    filename = sys.argv[2]

    pwd = getpass.getpass("passphrase: ").encode("utf-8") #pega a senha do usuário e converte para bytes (utf-8)

    if op == "enc":
        with open(filename, "rb") as f: #abre em leitura binária o arquivo que vai ser encriptado
            ptxt = f.read()

        salt = os.urandom(SALT_SIZE) #cria bytes aleatórios para o salt (garante que a mesma senha gere chaves diferentes em execuções diferentes)
        iv = os.urandom(IV_SIZE) #cria bytes aleatórios para o vetor de inicialização (garante que a mesma senha e mesmo arquivo gerem cifrados diferentes)

        enc_key, mac_key = derive_keys(pwd, salt) #gera as chaves de encriptação e autenticação a partir da senha e do salt

        cipher = Cipher(algorithms.AES(enc_key), modes.CTR(iv)) #cria o objeto de cifra usando AES no modo CTR com iv com a chave de encriptação e o vetor de inicialização
        encryptor = cipher.encryptor() #cria o objeto de encriptação a partir do objeto de cifra
        ctxt = encryptor.update(ptxt) + encryptor.finalize() #encripta o texto plano e finaliza a operação de encriptação

        h = hmac.HMAC(mac_key, hashes.SHA256()) #cria o objeto de HMAC usando a chave de autenticação e SHA256 como função de hash
        h.update(salt + iv + ctxt) #atualiza o HMAC com os dados que serão autenticados (salt, iv e texto cifrado)
        tag = h.finalize() # resultado final do HMAC, que é a tag de autenticação

        outname = filename + ".enc" #cria o nome do arquivo de saída adicionando a extensão .enc
        with open(outname, "wb") as f: 
            f.write(salt + iv + ctxt + tag) # e escreve la na ordem salt + iv + ctxt + tag)

    elif op == "dec":
        with open(filename, "rb") as f:
            data = f.read()

        salt = data[:SALT_SIZE]
        iv = data[SALT_SIZE:SALT_SIZE + IV_SIZE]
        tag = data[-TAG_SIZE:]
        ctxt = data[SALT_SIZE + IV_SIZE:-TAG_SIZE]

        enc_key, mac_key = derive_keys(pwd, salt) # gera as chaves de encriptação e autenticação a partir da senha e do salt

        h = hmac.HMAC(mac_key, hashes.SHA256())  #cria o objeto de HMAC usando a chave de autenticação e SHA256 como função de hash
        h.update(salt + iv + ctxt) #atualiza o HMAC com os dados que serão autenticados (salt, iv e texto cifrado)
        try:
            h.verify(tag) #verifica se a tag de autenticação é válida para os dados fornecidos (salt, iv e texto cifrado)
        except InvalidSignature:
            print("ERRO: passphrase errada ou file foi modificado")
            sys.exit(2)

        cipher = Cipher(algorithms.AES(enc_key), modes.CTR(iv)) #cria o objeto de cifra usando AES no modo CTR com iv com a chave de encriptação e o vetor de inicialização
        decryptor = cipher.decryptor() #cria o objeto de decriptação a partir do objeto de cifra
        ptxt = decryptor.update(ctxt) + decryptor.finalize() #decripta o texto cifrado e finaliza a operação de decriptação

        outname = filename + ".dec"
        with open(outname, "wb") as f:
            f.write(ptxt) # escreve o texto decriptado no arquivo de saída

    else:
        print("operação desconhecida")

if __name__ == "__main__":
    main()