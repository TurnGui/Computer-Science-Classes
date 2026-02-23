import sys

def shift(c, k):
    return chr((ord(c) - ord('A') + k) % 26 + ord('A'))

def decifrar(texto, k):
    res = ""
    for c in texto:
        res += shift(c, -k)
    return res

def main(args):
    if len(args) < 3:
        return

    criptograma = args[1]
    palavras = [p.upper() for p in args[2:]]

    for k in range(26):
        texto = decifrar(criptograma, k)
        for p in palavras:
            if p in texto:
                print(chr(ord('A') + k))
                print(texto)
                break  #adicionei o break para imprimir todas as palavras possíveis e não apenas a primeira

if __name__ == "__main__":
    main(sys.argv)


    
