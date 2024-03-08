#include <stdio.h>
#include <wchar.h>
#include <locale.h>

void convert_to_yijing(unsigned long long num) {
    if (num == 0) {
        wprintf(L"䷀\n"); // Isto é para o caso de o input ser 0
        return;
    }
    
    wchar_t yijing[64] = {
        0x4DC0, 0x4DC1, 0x4DC2, 0x4DC3, 0x4DC4, 0x4DC5, 0x4DC6, 0x4DC7, 0x4DC8, 0x4DC9, 0x4DCA, 0x4DCB, 0x4DCC, 0x4DCD, 0x4DCE, 0x4DCF,
        0x4DD0, 0x4DD1, 0x4DD2, 0x4DD3, 0x4DD4, 0x4DD5, 0x4DD6, 0x4DD7, 0x4DD8, 0x4DD9, 0x4DDA, 0x4DDB, 0x4DDC, 0x4DDD, 0x4DDE, 0x4DDF,
        0x4DE0, 0x4DE1, 0x4DE2, 0x4DE3, 0x4DE4, 0x4DE5, 0x4DE6, 0x4DE7, 0x4DE8, 0x4DE9, 0x4DEA, 0x4DEB, 0x4DEC, 0x4DED, 0x4DEE, 0x4DEF,
        0x4DF0, 0x4DF1, 0x4DF2, 0x4DF3, 0x4DF4, 0x4DF5, 0x4DF6, 0x4DF7, 0x4DF8, 0x4DF9, 0x4DFA, 0x4DFB, 0x4DFC, 0x4DFD, 0x4DFE, 0x4DFF
    };
    
    int base64[64];
    int i = 0;
    
    while (num > 0) {
        base64[i++] = num % 64;
        num /= 64;
    }
    
    for (int j = i - 1; j >= 0; j--) {
        wprintf(L"%lc", yijing[base64[j]]);
        if(j > 0)
            wprintf(L" ");
    }
    wprintf(L"\n");
}

int main() {
    
    setlocale(LC_CTYPE, "en_US.UTF-8");
    
    unsigned long long int num;
    int scanf_check;
    
    scanf_check = scanf("%llu", &num);
    if (scanf_check != 1) {
        printf("Erro!\n");
        return 1;
    }
    
    convert_to_yijing(num);
    
    return 0;
}

