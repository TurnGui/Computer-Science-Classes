import hashlib
import hmac
from abc import ABC, abstractmethod

class MAC(ABC):
    @abstractmethod
    def tag(self, message: bytes) -> bytes:
        pass
    
    @abstractmethod
    def verify(self, message: bytes, tag: bytes) -> bool:
        pass


class INT_PTXT_Adv(ABC):
    @abstractmethod
    def forge(self) -> tuple[bytes, bytes]:
        # Retorna mensagem forjada
        pass

class InsecureHashMAC(MAC):

    def tag(self, message: bytes) -> bytes:
        return hashlib.sha256(message).digest()
    
    def verify(self, message: bytes, tag: bytes) -> bool:
        return self.tag(message) == tag


class SecureHMAC(MAC):

    def __init__(self):
        self.key = b'secret_key_12345'  # Chave secreta
    
    def tag(self, message: bytes) -> bytes:
        return hmac.new(self.key, message, hashlib.sha256).digest()
    
    def verify(self, message: bytes, tag: bytes) -> bool:
        return hmac.compare_digest(self.tag(message), tag)


class HashMACAdversary(INT_PTXT_Adv):

    def forge(self) -> tuple[bytes, bytes]:
        # Escolhe uma mensagem qualquer
        forged_message = b"Transferir 1000 euros"
        
        # Calcula o hash (sem precisar de chave!)
        forged_tag = hashlib.sha256(forged_message).digest()
        
        print(f"[ADVERSÁRIO] Forjei mensagem: {forged_message}")
        print(f"[ADVERSÁRIO] Calculei tag: {forged_tag.hex()[:32]}...")
        
        return forged_message, forged_tag


class HMACAdversary(INT_PTXT_Adv):

    def forge(self) -> tuple[bytes, bytes]:
        # Tenta forjar, mas sem conhecer a chave secreta
        forged_message = b"Transferir 1000 euros"
        
        # Tenta adivinhar um tag aleatório (vai falhar!)
        forged_tag = b'\x00' * 32  # Tag inválida
        
        print(f"[ADVERSÁRIO] Tentei forjar: {forged_message}")
        print(f"[ADVERSÁRIO] Mas não sei a chave secreta!")
        
        return forged_message, forged_tag


def INT_PTXT(mac: MAC, adversary: INT_PTXT_Adv) -> bool:

    print("\n" + "="*60)
    print("JOGO INT-PTXT")
    print("="*60)
    
    # Adversário tenta forjar
    message, tag = adversary.forge()
    
    # Verificar se a forgery é válida
    is_valid = mac.verify(message, tag)
    
    print(f"\n[VERIFICADOR] Tag é válida? {is_valid}")
    
    if is_valid:
        print("ADVERSÁRIO GANHOU! (Conseguiu forjar!)")
    else:
        print("MAC SEGURO! (Adversário falhou!)")
    
    print("="*60 + "\n")
    
    return is_valid


if __name__ == "__main__":
    
    print("\n" + "#"*60)
    print("# TESTE 1: MAC INSEGURO (Hash sem chave)")
    print("#"*60)
    
    insecure_mac = InsecureHashMAC()
    hash_adversary = HashMACAdversary()
    
    result = INT_PTXT(insecure_mac, hash_adversary)
    print(f"Resultado: Adversário {'GANHOU' if result else 'PERDEU'}")
    
    
    print("\n" + "#"*60)
    print("# TESTE 2: MAC SEGURO (HMAC com chave)")
    print("#"*60)
    
    secure_mac = SecureHMAC()
    hmac_adversary = HMACAdversary()
    
    result = INT_PTXT(secure_mac, hmac_adversary)
    print(f"Resultado: Adversário {'GANHOU' if result else 'PERDEU'}")