import sys

def xor_bytes(data1, data2):
    return bytes([data1[i] ^ data2[i] for i in range(len(data1))])


def attack(fctxt, pos, ptxt_at_pos, new_ptxt_at_pos):
    ptxt_original = ptxt_at_pos.encode('utf-8')
    ptxt_novo = new_ptxt_at_pos.encode('utf-8')
    
    with open(fctxt, 'rb') as f:
        ciphertext = bytearray(f.read())
    
    modification = xor_bytes(ptxt_original, ptxt_novo)
    
    for i in range(len(modification)):
        ciphertext[pos + i] ^= modification[i]
    
    with open(fctxt + '.attck', 'wb') as f:
        f.write(ciphertext)


if __name__ == '__main__':
    attack(sys.argv[1], int(sys.argv[2]), sys.argv[3], sys.argv[4])