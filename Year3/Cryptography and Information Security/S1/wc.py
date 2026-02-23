import sys

def contar(ficheiro):
    linhas = 0
    palavras = 0
    caracteres = 0

    with open(ficheiro, 'r', encoding='utf-8') as f:
        for linha in f:
            linhas += 1
            caracteres += len(linha)
            palavras += len(linha.split())

    return linhas, palavras, caracteres


def main(inp):
    """ função que executa a funcionalidade pretendida... """

    #print("Argumentos da linha de comando: ", inp)

    if len(inp) != 2:
        print("Use: python wc.py <ficheiro>")
        return

    ficheiro = inp[1]

    try:
        l, p, c = contar(ficheiro)
        print(l, p, c)
    except FileNotFoundError:
        print(f"Erro: ficheiro '{ficheiro}' não encontrado")

# Se for chamada como script...
if __name__ == "__main__":
    main(sys.argv)