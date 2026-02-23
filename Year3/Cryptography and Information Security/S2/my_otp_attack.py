#!/usr/bin/env python3

import sys
import random


def seed_expand(x):
    r = x
    for _ in range(14):
        y = x >> 8
        x ^= y
        r ^= x
        x = y
    return r


def my_prng(n, seed):
    random.seed(seed_expand(seed))
    return random.randbytes(n)


def xor_bytes(data, key):
    return bytes([data[i] ^ key[i] for i in range(len(data))])


def check_words(text, words):
    try:
        text_str = text.decode('utf-8', errors='ignore')
        return all(word.lower() in text_str.lower() for word in words)
    except:
        return False


def attack(ciphertext_file, target_words):
    with open(ciphertext_file, 'rb') as f:
        ciphertext = f.read()
    
    for seed in range(100000000):
        key = my_prng(len(ciphertext), seed)
        plaintext = xor_bytes(ciphertext, key)
        
        if check_words(plaintext, target_words):
            print(plaintext.decode('utf-8', errors='ignore').strip())
            return


if __name__ == '__main__':
    attack(sys.argv[1], sys.argv[2:])