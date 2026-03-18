#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>
 
#include "carta.h"
#include "golf_estado.h"
 
static void imprimir_espaco_carta(void) { //dois espaços em branco quando nao tem carta naquele lugar
    wprintf(L"  ");
}
 
static void imprimir_carta(Carta c) {
    wprintf(L"%lc ", carta_unicode(c));
}
 
static void ui_mostrar_mesa(const GolfEstado *e) { // Tive que dividir a ui_mostrar em 2 pq do limite de 15 statemnts!
    for (int linha = 0; linha < GOLF_LINHAS; linha++) { //esta agora so trata da grelha
        wprintf(L"   ");
        for (int col = 0; col < GOLF_COLUNAS; col++) {
            if (linha <= e->topo[col])
                imprimir_carta(e->mesa[col][linha]);
            else
                imprimir_espaco_carta();
        }
        wprintf(L"\n");
    }
}
 
static void ui_mostrar(const GolfEstado *e) { //e assim esta so fica com 5 statements
    wprintf(L"\nDescarte: ");
    imprimir_carta(golf_descarte(e));
    wprintf(L"   Stock: %d\n\n", TAMANHO_BARALHO - e->stock.topo); // stock é Tamanho_BARALHO - o topo do stock
    ui_mostrar_mesa(e);
    wprintf(L"\nComandos: j <0-6> (jogar), p (puxar), q (sair)\n");
}
 
static int ler_comando(char *cmd, int *arg) { //cmd guarda letra do comando e arg numero de coluna
    char linha[64]; // buffer pra guardar essas letras e comandos do player
 
    if (!fgets(linha, sizeof(linha), stdin)) return 0; //le o teclado (o input bruto mm) , se falhar devolve 0, fazemos isto pq so scanf deixa /n no buffer e falha com inputs invalidos
 
    *cmd = '\0'; //inicializar valores logo pra evitar lixo de memoria 
    *arg = -1;
 
    if (sscanf(linha, " %c %d", cmd, arg) >= 1) return 1; // aqui finalmente traduz o input bruto do tipo j 4
    return 0;
}
 
static int jogo_terminou(const GolfEstado *e) {
    if (golf_ganhou(e)) return 1; // se todas as colunas vazias gg
    if (golf_stock_vazio(e) && !golf_tem_jogada(e)) return 1; // ou baralho vazio e sem jogadas
    return 0;
}
 
static void mostrar_resultado(const GolfEstado *e) { //print da mensagem de vitoria ou derrota
    if (golf_ganhou(e)) {
        wprintf(L"\n Parabéns ganhou! Nice :)\n");
    } else if (golf_stock_vazio(e) && !golf_tem_jogada(e)) {
        wprintf(L"\nSem baralho e sem jogadas possíveis. Perdeu :(\n");
    }
}
 
static int cmd_puxar(GolfEstado *e, int arg) { // chama golf_puxar_stock, o (void)arg ignora o argumento que não usa
    (void)arg; // puxar nao usa argumento
    if (!golf_puxar_stock(e)) {
        wprintf(L"Baralho vazio!\n");
    }
    return 1; // continua o jogo (andar sempre a passar o 1 ou 0 como o stor disse na aula)
}
 
static int cmd_jogar(GolfEstado *e, int arg) { // chama golf_jogar_coluna com o arg (número da coluna)
    if (!golf_jogar_coluna(e, arg)) {
        wprintf(L"Jogada inválida.\n");
    }
    return 1;
}
 
static int cmd_sair(GolfEstado *e, int arg) { 
    (void)e;
    (void)arg;
    return 0; // sair do loop
}
 
static int cmd_invalido(GolfEstado *e, int arg) { // imprime erro, devolve 1 — jogo continua
    (void)e;
    (void)arg;
    wprintf(L"Comando inválido.\n");
    return 1; // continua o jogo
}
 
/* lookup table: associa letra do comando à função */
typedef int (*ComandoFn)(GolfEstado *, int);
 
typedef struct {
    char     letra;
    ComandoFn funcao;
} EntradaComando;
 
static const EntradaComando comandos[] = {
    { 'p', cmd_puxar },
    { 'j', cmd_jogar },
    { 'q', cmd_sair },
};
 
static ComandoFn procurar_comando(char letra) {
    int n = sizeof(comandos) / sizeof(comandos[0]); // número de entradas
    for (int i = 0; i < n; i++) {
        if (comandos[i].letra == letra)
            return comandos[i].funcao;
    }
    return cmd_invalido; // não encontrou, devolve o erro
}
 
int main(void) {
    setlocale(LC_ALL, ""); //ativar unicode no terminal
 
    GolfEstado e; //espaço da memoria pra isto
    golf_estado_inicializar(&e, (unsigned)time(NULL)); // seed random com hora actual
 
    char cmd = '\0'; //Declaradas fora do while porque a condição do while precisa de lhes passar o endereço (&cmd, &arg).
    int  arg  = -1;
 
    ui_mostrar(&e);
    wprintf(L"> ");
 
    while (ler_comando(&cmd, &arg) && procurar_comando(cmd)(&e, arg) && !jogo_terminou(&e)) { // while pra evitar usar os breaks como o stor disse
        ui_mostrar(&e); //equanto ler e procurar comando nao der erro e jogo_terminou parar isto
        wprintf(L"> "); //imprimir o ui e o cursor
    }
 
    mostrar_resultado(&e); //print final pra dizer que perdeste :)
    return 0;
}