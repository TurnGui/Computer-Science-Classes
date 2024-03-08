// Input - day and month | Output - Emoji with correspondent zodiac sign
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

int main() {

    int dia,mes,scanf_check;

    scanf_check = scanf("%d %d", &dia, &mes);
    if (scanf_check !=2){
        printf("Erro!\n");
        return 1;
    }

    if ((mes == 3 && dia > 20) || (mes == 4 && dia <= 20)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x2648;
        wprintf(L"%lc\n", emoji); // Áries 
    } else if ((mes == 4 && dia >= 21) || (mes == 5 && dia <= 20)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x2649;
        wprintf(L"%lc\n", emoji); // Touro 
    } else if ((mes == 5 && dia >= 21) || (mes == 6 && dia <= 20)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x264a;
        wprintf(L"%lc\n", emoji); // Gêmeos 
    } else if ((mes == 6 && dia >= 21) || (mes == 7 && dia <= 22)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x264b;
        wprintf(L"%lc\n", emoji); // Câncer 
    } else if ((mes == 7 && dia >= 23) || (mes == 8 && dia <= 22)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x264c;
        wprintf(L"%lc\n", emoji); // Leão 
    } else if ((mes == 8 && dia >= 23) || (mes == 9 && dia <= 22)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x264d;
        wprintf(L"%lc\n", emoji); // Virgem 
    } else if ((mes == 9 && dia >= 23) || (mes == 10 && dia <= 22)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x264e;
        wprintf(L"%lc\n", emoji); // Libra 
    } else if ((mes == 10 && dia >= 23) || (mes == 11 && dia <= 21)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x264f;
        wprintf(L"%lc\n", emoji); // Escorpião 
    } else if ((mes == 11 && dia >= 22) || (mes == 12 && dia <= 21)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x2650;
        wprintf(L"%lc\n", emoji); // Sagitário 
    } else if ((mes == 12 && dia >= 22) || (mes == 1 && dia <= 19)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x2651;
        wprintf(L"%lc\n", emoji); // Capricórnio 
    } else if ((mes == 1 && dia >= 19) || (mes == 2 && dia <= 18)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x2652;
        wprintf(L"%lc\n", emoji); // Aquário 
    } else if ((mes == 2 && dia >= 19) || (mes == 3 && dia <= 20)) {
        setlocale(LC_CTYPE, "C.UTF-8");
        wchar_t emoji = 0x2653;
        wprintf(L"%lc\n", emoji); // Peixes
    } else {
        printf("Erro!");
    }

 return 0;

}
