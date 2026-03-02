# Guião da Semana 3

## Relatório do Guião da Semana 3
### ChaCha20
- Desenvolvimento do programa [cfich_chacha20.py](./cfich_chacha20.py) que cifra ficheiros usando a cifra sequencial ChaCha20. O programa suporta três operações: `setup` para gerar uma chave apropriada para ChaCha20, `enc` para cifrar um ficheiro usando XOR com o keystream gerado, e `dec` para decifrar. O NONCE é gravado juntamente com o criptograma para permitir a decifragem.

### Ataque de Integridade

- Desenvolvimento de um programa [chacha20_int_attck.py](./chacha20_int_attck.py) que demonstra como manipular informação cifrada por ChaCha20. Se soubermos um fragmento do texto-limpo numa posição específica, podemos alterá-lo sem conhecer a chave. O programa calcula a modificação necessária através de `XOR(texto_original, texto_novo)` e aplica-a ao criptograma.

### A ser feito ... 

## Respostas das Questões
- [Questão 1](./cfich_chacha20.py)
- [Questão 2](./Questão2.txt)
- [Questão 3](./Questao3.txt)
