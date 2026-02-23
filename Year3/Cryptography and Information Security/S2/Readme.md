# Guião da Semana 2

## Relatório do Guião da Semana 2
### Cifra de César
- Desenvolvimento de um programa [cesar.py](./cesar.py) que recebe 3 argumentos: o tipo de operação (enc ou dec), a chave e a mensagem. A cifra funciona com o deslocamento cada letra do alfabeto pelo valor correspondente à chave.
- Desenvolvimento de um programa [cesar_attack.py](./cesar_attack.py) que realiza um ataque à cifra de César usando força bruta, testando as 26 chaves possíveis e verificando se o texto decifrado contém alguma das palavras fornecidas.

### Cifra de Cifra de Vigenère
- Desenvolvimento de um programa [vigenere.py](./vigenere.py) que funciona de forma parecida ao programa de [cesar_attack.py](./cesar_attack.py), mas a chave pode ser uma palavra. Cada letra da chave define um deslocamento de César aplicado ciclicamente à mensagem.
- Desenvolvimento de um programa [vigenere_attack.py](./vigenere_attack.py) que realiza um ataque à cifra de Vigenère. Dado o tamanho da chave, o programa divide o criptograma em "fatias" e ataca cada uma como uma cifra de César independente, usando a frequência de letras do Português (A, E, O, S, ...) para determinar o deslocamento mais provável de cada posição.

### One-Time Pad (OTP)
- Desenvolvimento de um programa [otp.py](./otp.py) que implementa a cifra One-Time Pad. O programa suporta três operações: setup para gerar uma chave aleatória criptograficamente segura usando secrets.token_bytes(), enc para cifrar usando XOR, e dec para decifrar usando XOR.
- Desenvolviment de um programa [my_otp.py](./my_otp.py) que substitui o gerador anterior (secrets.token_bytes()) pela função my_prng, que usa random.seed() com uma seed gerada por os.urandom(16).

## Respostas das Questões
- [Questão 1](./Questao1.txt)
- [Questão 2](./Questao2.txt)
