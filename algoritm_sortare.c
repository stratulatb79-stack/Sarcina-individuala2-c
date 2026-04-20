#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------------------- Functii pentru tablou --------------------
void initializare_aleatorie(int *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;   // numere intre 0 si 9999
}

void afisare_tablou(int *arr, int n) {
    if (n == 0) {
        printf("Tabloul este gol.\n");
        return;
    }
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void eliberare_memorie(int **arr, int *n) {
    if (*arr != NULL) {
        free(*arr);
        *arr = NULL;
        *n = 0;
        printf("Memoria a fost eliberata.\n");
    } else {
        printf("Tabloul nu este alocat.\n");
    }
}

// -------------------- Algoritmi de sortare --------------------
// Selection Sort 
void selection_sort(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}

// Quick Sort 
void quick_sort(int *arr, int st, int dr) {
    if (st < dr) {
        int pivot = arr[dr];
        int i = st - 1;
        for (int j = st; j < dr; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[dr];
        arr[dr] = temp;
        int p = i + 1;
        quick_sort(arr, st, p - 1);
        quick_sort(arr, p + 1, dr);
    }
}

// -------------------- Masurare timp --------------------
double masurare_sortare1(int *arr, int n) {
    // Face o copie pentru a nu modifica originalul
    int *copie = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copie[i] = arr[i];
    clock_t start = clock();
    selection_sort(copie, n);
    clock_t end = clock();
    free(copie);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double masurare_sortare2(int *arr, int n) {
    int *copie = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copie[i] = arr[i];
    clock_t start = clock();
    quick_sort(copie, 0, n - 1);
    clock_t end = clock();
    free(copie);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// -------------------- Analiza empirica --------------------
void analiza_empirica() {
    int valori_n[] = {100, 1000, 10000, 100000};
    double timp_sort1[4], timp_sort2[4];

    printf("\n=== ANALIZA EMPIRICA ===\n");
    printf("Se genereaza tablouri aleatoare si se masoara timpul...\n\n");

    for (int i = 0; i < 4; i++) {
        int n = valori_n[i];
        int *arr = (int*)malloc(n * sizeof(int));
        initializare_aleatorie(arr, n);

        // Masoara Selection Sort
        clock_t start1 = clock();
        selection_sort(arr, n);          // sortare directa pe original
        clock_t end1 = clock();
        timp_sort1[i] = (double)(end1 - start1) / CLOCKS_PER_SEC;

        // Refacere tablou aleator pentru Quick Sort
        initializare_aleatorie(arr, n);
        clock_t start2 = clock();
        quick_sort(arr, 0, n - 1);
        clock_t end2 = clock();
        timp_sort2[i] = (double)(end2 - start2) / CLOCKS_PER_SEC;

        free(arr);
    }

    // Afisare tabel
    printf("+----------------+------------+------------+------------+------------+\n");
    printf("| N              |        100 |       1000 |      10000 |     100000 |\n");
    printf("+----------------+------------+------------+------------+------------+\n");
    printf("| Selection Sort | %9.6f | %9.6f | %9.6f | %9.6f |\n",
           timp_sort1[0], timp_sort1[1], timp_sort1[2], timp_sort1[3]);
    printf("| Quick Sort     | %9.6f | %9.6f | %9.6f | %9.6f |\n",
           timp_sort2[0], timp_sort2[1], timp_sort2[2], timp_sort2[3]);
    printf("+----------------+------------+------------+------------+------------+\n");
}

// -------------------- Main cu meniu --------------------
int main() {
    int *tablou = NULL;
    int n = 0;
    int optiune;
    srand(time(NULL));

    do {
        printf("\n===== MENIU =====\n");
        printf("1. Initializare tablou cu numere aleatorii\n");
        printf("2. Afisare tablou\n");
        printf("3. Eliberare memorie tablou\n");
        printf("4. Sortare prin metoda Selection Sort + afisare timp\n");
        printf("5. Sortare prin metoda Quick Sort + afisare timp\n");
        printf("6. Analiza empirica pentru N = 100, 1000, 10000, 100000\n");
        printf("0. Iesire\n");
        printf("Alegere: ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1:
                printf("Dimensiunea tabloului: ");
                scanf("%d", &n);
                if (n <= 0) {
                    printf("Dimensiune invalida.\n");
                    break;
                }
                if (tablou != NULL) free(tablou);
                tablou = (int*)malloc(n * sizeof(int));
                if (tablou == NULL) {
                    printf("Eroare alocare.\n");
                    n = 0;
                    break;
                }
                initializare_aleatorie(tablou, n);
                printf("Tabloul a fost initializat cu %d elemente aleatoare.\n", n);
                break;
            case 2:
                if (tablou == NULL || n == 0)
                    printf("Tabloul nu este alocat.\n");
                else
                    afisare_tablou(tablou, n);
                break;
            case 3:
                eliberare_memorie(&tablou, &n);
                break;
            case 4:
                if (tablou == NULL || n == 0)
                    printf("Tabloul nu este alocat.\n");
                else {
                    double t = masurare_sortare1(tablou, n);
                    printf("Timp sortare Selection Sort: %f secunde\n", t);
                }
                break;
            case 5:
                if (tablou == NULL || n == 0)
                    printf("Tabloul nu este alocat.\n");
                else {
                    double t = masurare_sortare2(tablou, n);
                    printf("Timp sortare Quick Sort: %f secunde\n", t);
                }
                break;
            case 6:
                analiza_empirica();
                break;
            case 0:
                printf("Ieșire din program.\n");
                break;
            default:
                printf("Optiune gresita.\n");
        }
    } while (optiune != 0);

    if (tablou != NULL) free(tablou);
    return 0;
}