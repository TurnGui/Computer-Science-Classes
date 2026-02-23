import sys

def preproc(str):
    l = []
    for c in str:
        if c.isalpha():
            l.append(c.upper())
    return "".join(l)

def shift(c, k):
    return chr((ord(c) - ord('A') + k) % 26 + ord('A'))

def cesar(text, key, mode):
    k = ord(key) - ord('A')
    if mode == "dec":
        k = -k
    res = ""
    for c in text:
        res += shift(c, k)
    return res

def main(args):
    if len(args) != 4:
        print("Uso: python cesar.py <enc|dec> <key> <msg>")
        return

    mode = args[1]
    key = args[2].upper()
    msg = args[3]

    if mode not in ["enc", "dec"] or len(key) != 1 or not key.isalpha():
        print("Argumentos inválidos")
        return

    msg = preproc(msg)
    print(cesar(msg, key, mode))

if __name__ == "__main__":
    main(sys.argv)
