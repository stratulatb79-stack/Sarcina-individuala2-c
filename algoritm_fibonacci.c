#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BASE 1000000000
#define DIGITS_PER_BLOCK 9

typedef struct {
    int *blocks;
    int len;
} BigNum;

void bignum_zero(BigNum *a) {
    a->blocks = (int*)malloc(sizeof(int));
    a->blocks[0] = 0;
    a->len = 1;
}

void bignum_from_int(BigNum *a, int val) {
    a->blocks = (int*)malloc(sizeof(int));
    a->blocks[0] = val;
    a->len = 1;
}

void bignum_copy(BigNum *dest, const BigNum *src) {
    dest->blocks = (int*)malloc(src->len * sizeof(int));
    for (int i = 0; i < src->len; i++)
        dest->blocks[i] = src->blocks[i];
    dest->len = src->len;
}

void bignum_free(BigNum *a) {
    if (a->blocks) free(a->blocks);
    a->blocks = NULL;
    a->len = 0;
}

void bignum_add(const BigNum *a, const BigNum *b, BigNum *rez) {
    int max_len = (a->len > b->len) ? a->len : b->len;
    int *tmp = (int*)calloc(max_len + 1, sizeof(int));
    long long carry = 0;
    for (int i = 0; i < max_len; i++) {
        long long sum = carry;
        if (i < a->len) sum += a->blocks[i];
        if (i < b->len) sum += b->blocks[i];
        tmp[i] = (int)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        tmp[max_len] = (int)carry;
        max_len++;
    }
    while (max_len > 1 && tmp[max_len - 1] == 0) max_len--;
    rez->blocks = (int*)malloc(max_len * sizeof(int));
    memcpy(rez->blocks, tmp, max_len * sizeof(int));
    rez->len = max_len;
    free(tmp);
}

char* bignum_to_string(const BigNum *a) {
    if (a->len == 1 && a->blocks[0] == 0) {
        char *s = (char*)malloc(2);
        s[0] = '0'; s[1] = '\0';
        return s;
    }
    int last = a->blocks[a->len - 1];
    int last_digits = snprintf(NULL, 0, "%d", last);
    int total_digits = (a->len - 1) * DIGITS_PER_BLOCK + last_digits;
    char *str = (char*)malloc(total_digits + 1);
    char *ptr = str + total_digits;
    *ptr = '\0';
    for (int i = 0; i < a->len - 1; i++) {
        ptr -= DIGITS_PER_BLOCK;
        sprintf(ptr, "%09d", a->blocks[i]);
    }
    ptr -= last_digits;
    sprintf(ptr, "%d", a->blocks[a->len - 1]);
    return str;
}

char* fibonacci(int n, double *timp) {
    if (n == 0) {
        char *s = malloc(2);
        s[0] = '0'; s[1] = '\0';
        if (timp) *timp = 0.0;
        return s;
    }
    if (n == 1) {
        char *s = malloc(2);
        s[0] = '1'; s[1] = '\0';
        if (timp) *timp = 0.0;
        return s;
    }
    clock_t start = clock();
    BigNum a, b, c;
    bignum_from_int(&a, 0);
    bignum_from_int(&b, 1);
    for (int i = 2; i <= n; i++) {
        bignum_add(&a, &b, &c);
        bignum_free(&a);
        a = b;
        b = c;
    }
    clock_t end = clock();
    if (timp) *timp = (double)(end - start) / CLOCKS_PER_SEC;
    char *rezultat = bignum_to_string(&b);
    bignum_free(&a);
    bignum_free(&b);
    return rezultat;
}

void print_fib_result(int n, const char *fib_str) {
    int len = strlen(fib_str);
    if (len <= 80 || n <= 1000) {
        printf("fib(%d) = %s\n", n, fib_str);
    } else {
        printf("fib(%d) are %d cifre.\n", n, len);
        printf("Primele 20 cifre: ");
        for (int i = 0; i < 20; i++) putchar(fib_str[i]);
        printf(" ... ");
        for (int i = len - 20; i < len; i++) putchar(fib_str[i]);
        printf("\n");
    }
}

void analiza_empirica() {
    int valori[] = {50, 100, 1000, 10000, 100000, 1000000};
    int num_tests = 6;
    double timp[6];

    printf("\n=== ANALIZA EMPIRICA FIBONACCI ===\n");
    printf("Se calculeaza fib(N) pentru N crescator... (poate dura 10-20 secunde pentru N=1.000.000)\n\n");

    for (int i = 0; i < num_tests; i++) {
        int n = valori[i];
        double elapsed;
        char *fib_str = fibonacci(n, &elapsed);
        timp[i] = elapsed;
        printf("fib(%d) calculat in %.6f secunde.\n", n, elapsed);
        free(fib_str);
    }

    printf("\n+----------------+------------+------------+------------+------------+------------+------------+\n");
    printf("| N              |         50 |        100 |       1000 |      10000 |     100000 |    1000000 |\n");
    printf("+----------------+------------+------------+------------+------------+------------+------------+\n");
    printf("| Fibonacci      | %9.6f | %9.6f | %9.6f | %9.6f | %9.6f | %9.6f |\n",
           timp[0], timp[1], timp[2], timp[3], timp[4], timp[5]);
    printf("+----------------+------------+------------+------------+------------+------------+------------+\n");
}

int main() {
    int optiune;
    srand(time(NULL));

    do {
        printf("\n===== MENIU FIBONACCI =====\n");
        printf("1. Calcul Fibonacci pentru o pozitie N\n");
        printf("2. Analiza empirica (N = 50,100,1000,10000,100000,1000000)\n");
        printf("0. Iesire din program\n");
        printf("Alegere: ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1: {
                int n;
                printf("Introduceti N (0 - 1000000): ");
                scanf("%d", &n);
                if (n < 0 || n > 1000000) {
                    printf("N trebuie sa fie intre 0 si 1.000.000.\n");
                    break;
                }
                double timp;
                char *fib_str = fibonacci(n, &timp);
                printf("Timp de calcul: %f secunde\n", timp);
                print_fib_result(n, fib_str);
                free(fib_str);
                break;
            }
            case 2:
                analiza_empirica();
                break;
            case 0:
                printf("Iesire din program.\n");
                break;
            default:
                printf("Optiune invalida.\n");
        }
    } while (optiune != 0);

    return 0;
}