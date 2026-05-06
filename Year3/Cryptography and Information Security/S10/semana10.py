import numpy as np
import os


# Parâmetros do FrodoKEM-640
PARAMS_640 = {
    "n":     640,
    "q":     2**15,
    "n_bar": 8,
    "B":     2,
    "T_CHI": [4643, 13363, 20579, 25843, 29227, 31145, 32103, 32525, 32689, 32745, 32762, 32767],
    "k_bits": 15,
}


def genMat(seedA, n, q):
    """
    Gera matriz aleatória A de dimensão n×n com elementos em Z_q.
    Usa seedA para inicializar o gerador de números pseudoaleatórios,
    garantindo reprodutibilidade (A pode ser reconstruída pela pk).
    """
    rng = np.random.default_rng(seedA) # Gerador de números aleatórios com semente igual pra emissor e receptor
    A = rng.integers(0, q, size=(n, n), dtype=np.int64) # Gera uma matriz n×n preenchida com inteiros aleatórios uniformes entre 0 e q-1. 
    return A # Devolve a matriz A gerada, que é usada tanto na geração de chaves quanto na encriptação e desencriptação, garantindo que ambas as partes tenham acesso à mesma matriz sem precisar transmiti-la diretamente.


def chiSample(T_CHI, k_bits, rows, cols):
    """
    Gera uma matriz (rows × cols) com elementos amostrados segundo a
    distribuição gaussiana discreta χ, usando a tabela de probabilidades
    acumuladas T_CHI com k_bits de precisão.

    Algoritmo:
      1. Gera x ~ Uniforme[0, 2^k_bits[
      2. Encontra i = primeiro índice onde T_CHI[i] >= x  → valor absoluto
      3. Amostra bit de sinal s ∈ {0,1}
      4. Retorna (1 - 2*s) * i  → valor com sinal
    """
    T = np.array(T_CHI, dtype=np.int64) #Converte a tabela Python para um array numpy de inteiros de 64 bits, para poder usar funções numpy nela.
    precision = 2**k_bits #Calcula o tamanho do universo de amostragem. Para k_bits=15, precision=32768

    result = np.zeros((rows, cols), dtype=np.int64) #Cria a matriz de resultado preenchida a zeros, com as dimensões pedidas.

    for r in range(rows):
        for c in range(cols): #Itera sobre cada posição da matriz para preencher elemento a elemento.
            x = np.random.randint(0, precision) #Sorteia um número aleatório uniforme entre 0 e precision-1.Este número vai ser comparado com a tabela para decidir o valor absoluto do elemento.
            i = int(np.searchsorted(T, x, side='left')) #Procura na tabela T a posição do primeiro valor maior ou igual a x. Esse índice i é o valor absoluto do elemento
            s = np.random.randint(0, 2) #Sorteia o bit de sinal: 0 ou 1.
            result[r, c] = (1 - 2 * s) * i #Aplica o sinal: se s=0 fica +i, se s=1 fica -i. O valor fica centrado em 0 com simetria.

    return result #devolve matriz de ruido


def encode(m_bits, n_bar, B, q):
    """
    Codifica uma mensagem de (n_bar * n_bar * B) bits numa matriz n_bar×n_bar.

    Cada elemento da matriz guarda B bits da mensagem, escalados para Z_q:
        valor ∈ {0,...,2^B - 1}  →  valor * (q / 2^B)

    Desta forma, os bits ficam "espalhados" pelo anel Z_q,
    o que permite absorver o ruído na descodificação.
    """
    scale  = q // (2**B)      # factor de escala: q/4 para B=2
    n_elems = n_bar * n_bar   # número total de elementos na matriz

    # Agrupa B bits consecutivos e converte cada grupo para um inteiro
    m_bits = np.array(m_bits, dtype=np.int64) #Converte a lista de bits para array numpy.
    values = np.array([ #Para cada elemento i, extrai os B bits correspondentes (m_bits[i*B:(i+1)*B]), converte-os para um inteiro com packbits
        int(np.packbits(m_bits[i*B:(i+1)*B], bitorder='big')[0] >> (8 - B))
        for i in range(n_elems) #e faz shift para alinhar os bits à direita. O resultado são inteiros em {0,1,2,3} para B=2.
    ], dtype=np.int64)

    # Escala e organiza em matriz n_bar×n_bar
    M = (values * scale).reshape(n_bar, n_bar) #Multiplica cada inteiro pelo factor de escala
    return M #um 1 passa a q/4, um 2 passa a q/2, etc. Depois organiza o array de 64 valores numa matriz 8×8. e devolve a matriz codificada.


def decode(M, B, q):
    """
    Descodifica uma matriz n_bar×n_bar em Z_q de volta para bits.

    Cada elemento x é mapeado para B bits por arredondamento:
        round( x * 2^B / q )  mod 2^B

    O arredondamento é o que permite corrigir o pequeno ruído
    introduzido durante a encriptação.
    """
    scale = 2**B #Calcula 2^B = 4. É o número de níveis possíveis (os valores {0,1,2,3}).

    # Arredonda cada elemento para o inteiro de B bits mais próximo
    values = np.round(M.flatten().astype(np.float64) * scale / q).astype(np.int64) % scale
    #M.flatten() — transforma a matriz num array de 64 elementos ; .astype(np.float64) — converte para vírgula flutuante para o arredondamento ser preciso
    #* scale / q — divide pelo factor de escala, mapeando cada valor de volta para perto de {0,1,2,3}
    #np.round(...) — arredonda ao inteiro mais próximo, eliminando o ruído residual ; .astype(np.int64) — converte de volta para inteiro
    #% scale — garante que o resultado fica em {0,1,2,3}, tratando o caso de arredondamento para 4

    # Converte cada inteiro de volta para B bits
    bits = [] #Lista vazia que vai acumular os bits de saída.
    for v in values: # Para cada inteiro v, extrai os B bits do mais significativo para o menos significativo. 
        for bit_pos in range(B - 1, -1, -1): # (v >> bit_pos) & 1 isola o bit na posição bit_pos
            bits.append(int((v >> bit_pos) & 1))

    return np.array(bits, dtype=np.int64) #Devolve os bits recuperados como array numpy.


def keyGen(params): #Define a função que recebe o dicionário de parâmetros.

    """
    Geração de chaves.

    Saída:
        sk = S              (chave secreta)
        pk = (seedA, B)     (chave pública)

    A chave pública contém apenas seedA (não A diretamente) porque
    qualquer um pode reconstruir A a partir de seedA com genMat.
    Isso poupa largura de banda.

    Construção:
        B = A·S + E  (mod q)

    O ruído E "esconde" S: sem conhecer E, é difícil recuperar S a partir de B e A
    (este é o problema LWE).
    """
    n     = params["n"]
    q     = params["q"]
    n_bar = params["n_bar"]
    T_CHI = params["T_CHI"]
    k     = params["k_bits"] #Extrai os parâmetros necessários do dicionário.

    # Semente aleatória para gerar A (128 bits de entropia)
    seedA = int.from_bytes(os.urandom(16), 'big') % q

    A = genMat(seedA, n, q)               # matriz pública n×n usando a semente
    S = chiSample(T_CHI, k, n, n_bar)     # segredo S de dimensao n×n_bar
    E = chiSample(T_CHI, k, n, n_bar)     # ruído E com a dimensao n×n_bar

    B = (A @ S + E) % q #Calcula B = A·S + E mod q. @ é a multiplicação de matrizes em numpy. O % q mantém todos os elementos em Z_q.

    pk = (seedA, B)
    sk = S 
    return sk, pk #Organiza e devolve o par de chaves.


def enc(pk, m_bits, params):
    """
    Encriptação da mensagem m_bits com a chave pública pk.
    Saída:
        C1 = S'·A + E'          (n_bar × n)
        C2 = S'·B + E'' + M     (n_bar × n_bar)
    O truque é que S'·B ≈ S'·A·S, por isso o receptor pode calcular
    C1·S ≈ S'·A·S e subtrair de C2 para recuperar M.
    """
    n     = params["n"]
    q     = params["q"]
    n_bar = params["n_bar"]
    B_enc = params["B"]
    T_CHI = params["T_CHI"]
    k     = params["k_bits"]

    seedA, B = pk
    A = genMat(seedA, n, q)

    Sp  = chiSample(T_CHI, k, n_bar, n)
    Ep  = chiSample(T_CHI, k, n_bar, n)
    Epp = chiSample(T_CHI, k, n_bar, n_bar)

    C1 = (Sp @ A + Ep) % q
    V  = (Sp @ B + Epp) % q

    M  = encode(m_bits, n_bar, B_enc, q)
    C2 = (V + M) % q

    return C1, C2


def dec(sk, C, params):
    """
    Desencriptação do criptograma C = (C1, C2) com a chave secreta sk.

    Cálculo:
        V  = C1·S  ≈  S'·A·S          (cancela o ruído pequeno)
        M  = C2 - V  ≈  E'' + M        (ruído pequeno + mensagem codificada)
        m' = decode(M)                  (arredondamento remove o ruído residual)

    O arredondamento na decode só funciona se o ruído acumulado for
    suficientemente pequeno face à escala q/2^B.
    """
    q     = params["q"]
    B_enc = params["B"]

    C1, C2 = C
    S = sk

    V      = (C1 @ S) % q        # aproxima S'·A·S
    M      = (C2 - V) % q        # isola M (com ruído residual pequeno)
    m_bits = decode(M, B_enc, q) # arredondamento recupera os bits originais

    return m_bits


if __name__ == "__main__":
    # PARAMS_640 é lento (matriz 640×640); para testar depressa usa PARAMS_MINI
    params = PARAMS_640

    n_bar      = params["n_bar"]
    B          = params["B"]
    total_bits = n_bar * n_bar * B   # 128 bits

    # Gera mensagem aleatória de 128 bits
    m = np.random.randint(0, 2, size=total_bits, dtype=np.int64)
    print(f"Mensagem original:    {m.tolist()}")

    # Corre o PKE completo
    sk, pk = keyGen(params)
    C      = enc(pk, m, params)
    m_rec  = dec(sk, C, params)

    print(f"Mensagem recuperada:  {m_rec.tolist()}")
    erros = int(np.sum(m != m_rec))
    print(f"Bits errados: {erros}  →  {'OK ✓' if erros == 0 else 'FALHOU ✗'}")
