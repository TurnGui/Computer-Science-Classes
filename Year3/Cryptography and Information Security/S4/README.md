# pbenc_aes_ctr_hmac.py

Password-Based Encryption de ficheiros usando PBKDF2 + AES-CTR + HMAC-SHA256.

## Requisitos
- Python 3
- Biblioteca cryptography

Instalar:
pip install cryptography

## Uso

Encriptar:
python3 pbenc_aes_ctr_hmac.py enc <ficheiro>

Exemplo:
python3 pbenc_aes_ctr_hmac.py enc a.txt

Isto cria:
a.txt.enc


Decifrar:
python3 pbenc_aes_ctr_hmac.py dec <ficheiro.enc>

Exemplo:
python3 pbenc_aes_ctr_hmac.py dec a.txt.enc

Isto cria:
a.txt.enc.dec


Notas:
- A passphrase é pedida no terminal (não aparece no ecrã).
- Se a passphrase estiver errada OU o ficheiro tiver sido alterado, o programa dá erro.
- O ficheiro .enc contém: salt(16) || iv(16) || ciphertext || tag(32)
