import sys


def preproc(s): # VER SE É LETRA, TRANSFORMAR EM MAIÚSCULA, JUNTAR EM STRING
    l = []
    for c in s:
        if c.isalpha(): # VER SE É LETRA
            l.append(c.upper())
    return "".join(l) #LISTA DE LETRAS PARA STRING


def vigenere_transform(text, key, enc): #CIFRAR E DECIFRAR
    res = []
    m = len(key)

    for i, ch in enumerate(text):
        p = ord(ch) - ord('A') #CONVERTE LETRA PARA NÚMERO (0-25)
        k = ord(key[i % m]) - ord('A') # BACO = 4 FAZER  4%i PARA TER O INDICE DA CHAVE
        if enc:
            x = (p + k) % 26 #26 LETRAS POSSIVEIS
        else:
            x = (p - k) % 26
        res.append(chr(x + ord('A'))) #PASSAR A LETRA DE VOLTA A CHAR

    return "".join(res)

def main():
    op = sys.argv[1]
    key = preproc(sys.argv[2])
    msg = preproc(sys.argv[3]) #VERIFICAR MENSAGEM COM PREPROC

    if op == "enc":
        print(vigenere_transform(msg, key, True))
    else:
        print(vigenere_transform(msg, key, False))

if __name__ == "__main__":
     main()
