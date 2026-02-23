import sys
#brute force :(

def preproc(str):
    l = []
    for c in str:
        if c.isalpha():
            l.append(c.upper())
    return "".join(l)


def vigenere_dec(text, key):
    res = []
    m = len(key)
    for i, ch in enumerate(text):
        c = ord(ch) - ord('A')
        k = ord(key[i % m]) - ord('A')
        p = (c - k) % 26
        res.append(chr(p + ord('A')))
    return "".join(res)


def attack_bruteforce(crypt, m, words):
    key = ['A'] * m

    while True:
        k = "".join(key)
        plain = vigenere_dec(crypt, k)

        for w in words:
            if w in plain:
                print(k)
                print(plain)
                return

        i = m - 1
        while i >= 0 and key[i] == 'Z':
            key[i] = 'A'
            i -= 1
        if i < 0:
            return
        key[i] = chr(ord(key[i]) + 1)


def main():
    m = int(sys.argv[1])
    crypt = preproc(sys.argv[2])
    words = [preproc(w) for w in sys.argv[3:]]

    attack_bruteforce(crypt, m, words)


if __name__ == "__main__":
    main()
