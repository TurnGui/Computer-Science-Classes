# CIFRA ONE TIME PADDING - Exemplo de uso :
#$ python otp.py setup 30 otp.key
#$ echo "Mensagem a cifrar" > ptxt.txt
#$ python otp.py enc ptxt.txt otp.key
#$ python otp.py dec ptxt.txt.enc otp.key
#$ cat ptxt.txt.enc.dec
#Mensagem a cifrar


import sys
import secrets


def setup(num_bytes, key_filename):
    num_bytes = int(num_bytes)
    random_key = secrets.token_bytes(num_bytes)
    with open(key_filename, 'wb') as f:
        f.write(random_key)


def xor_bytes(data, key):
    if len(key) < len(data):
        print("A chave está incorreta")
        sys.exit(1)
    return bytes([data[i] ^ key[i] for i in range(len(data))])


def encrypt(plaintext_filename, key_filename):
    with open(plaintext_filename, 'rb') as f:
        plaintext = f.read()
    with open(key_filename, 'rb') as f:
        key = f.read()
    
    ciphertext = xor_bytes(plaintext, key)
    
    output_filename = plaintext_filename + '.enc'
    with open(output_filename, 'wb') as f:
        f.write(ciphertext)


def decrypt(ciphertext_filename, key_filename):
    with open(ciphertext_filename, 'rb') as f:
        ciphertext = f.read()
    with open(key_filename, 'rb') as f:
        key = f.read()
    
    plaintext = xor_bytes(ciphertext, key)
    
    output_filename = ciphertext_filename + '.dec'
    with open(output_filename, 'wb') as f:
        f.write(plaintext)


def main():
    command = sys.argv[1]
    
    if command == 'setup':
        setup(sys.argv[2], sys.argv[3])
    elif command == 'enc':
        encrypt(sys.argv[2], sys.argv[3])
    elif command == 'dec':
        decrypt(sys.argv[2], sys.argv[3])


if __name__ == '__main__':
    main()