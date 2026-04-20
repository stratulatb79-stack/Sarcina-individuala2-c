#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------------------- Functii pentru tablou --------------------
void init_random(int *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;   // numere între 0 și 9999
}

void display_array(int *arr, int n) {
    if (n == 0) {
        printf("Tabloul este gol.\n");
        return;
    }
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void free_memory(int **arr, int *n) {
    if (*arr != NULL) {
        free(*arr);
        *arr = NULL;
        *n = 0;
        printf("Memoria a fost eliberată.\n");
    } else {
        printf("Tabloul nu este alocat.\n");
    }
}

// -------------------- Algoritmi de căutare --------------------
// Linear search: returnează indexul sau -1
int linear_search(int *arr, int n, int key) {
    for (int i = 0; i < n; i++)
        if (arr[i] == key)
            return i;
    return -1;
}

// Binary search (iterativ) – necesită tablou sortat crescător
int binary_search(int *arr, int n, int key) {
    int left = 0, right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1;
}

// -------------------- Sortare pentru binary search --------------------
// Quick sort (necesar pentru a sorta înainte de căutarea binară)
void quick_sort(int *arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        int pi = i + 1;
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// -------------------- Măsurare timp pentru o singură căutare --------------------
double measure_linear_search(int *arr, int n, int key) {
    clock_t start = clock();
    linear_search(arr, n, key);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double measure_binary_search(int *arr, int n, int key) {
    // Sortăm o copie pentru a nu modifica originalul 
    int *copy = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    quick_sort(copy, 0, n - 1);
    clock_t start = clock();
    binary_search(copy, n, key);
    clock_t end = clock();
    free(copy);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// -------------------- Analiză empirică: 10000 de căutări --------------------
void empirical_analysis() {
    int sizes[] = {100, 1000, 10000, 100000};
    double linear_times[4], binary_times[4];

    printf("\n=== ANALIZĂ EMPIRICĂ (10.000 de căutări pentru fiecare N) ===\n");
    printf("Se generează tablouri aleatoare și se măsoară timpul...\n\n");

    for (int i = 0; i < 4; i++) {
        int N = sizes[i];
        int *arr = (int*)malloc(N * sizeof(int));
        init_random(arr, N);

        // ----- Linear search: 10000 căutări -----
        clock_t start_lin = clock();
        for (int k = 0; k < 10000; k++) {
            int key = rand() % 10000;   // cheie aleatoare în același interval
            linear_search(arr, N, key);
        }
        clock_t end_lin = clock();
        linear_times[i] = (double)(end_lin - start_lin) / CLOCKS_PER_SEC;

        // ----- Binary search: sortăm o singură dată, apoi 10000 căutări -----
        // Sortăm o copie pentru a nu distruge array-ul original (ne trebuie doar timpul căutărilor)
        int *sorted_copy = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) sorted_copy[j] = arr[j];
        quick_sort(sorted_copy, 0, N - 1);   // sortare o singură dată
        clock_t start_bin = clock();
        for (int k = 0; k < 10000; k++) {
            int key = rand() % 10000;
            binary_search(sorted_copy, N, key);
        }
        clock_t end_bin = clock();
        binary_times[i] = (double)(end_bin - start_bin) / CLOCKS_PER_SEC;

        free(sorted_copy);
        free(arr);
    }

    // Afișare tabel
    printf("+----------------+------------+------------+------------+------------+\n");
    printf("| N              |        100 |       1000 |      10000 |     100000 |\n");
    printf("+----------------+------------+------------+------------+------------+\n");
    printf("| Linear search  | %9.6f | %9.6f | %9.6f | %9.6f |\n",
           linear_times[0], linear_times[1], linear_times[2], linear_times[3]);
    printf("| Binary search  | %9.6f | %9.6f | %9.6f | %9.6f |\n",
           binary_times[0], binary_times[1], binary_times[2], binary_times[3]);
    printf("+----------------+------------+------------+------------+------------+\n");
}

// -------------------- Main cu meniu --------------------
int main() {
    int *array = NULL;
    int n = 0;
    int option;
    srand(time(NULL));

    do {
        printf("\n===== MENIU =====\n");
        printf("1. Inițializare tablou cu numere aleatorii\n");
        printf("2. Afișare tablou\n");
        printf("3. Eliberare memorie tablou\n");
        printf("4. Căutare liniară (măsoară timpul)\n");
        printf("5. Căutare binară (măsoară timpul)\n");
        printf("6. Analiză empirică (10.000 căutări pentru N = 100, 1000, 10000, 100000)\n");
        printf("0. Ieșire\n");
        printf("Alegere: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Dimensiunea tabloului: ");
                scanf("%d", &n);
                if (n <= 0) {
                    printf("Dimensiune invalidă.\n");
                    break;
                }
                if (array != NULL) free(array);
                array = (int*)malloc(n * sizeof(int));
                if (array == NULL) {
                    printf("Eroare alocare memorie.\n");
                    n = 0;
                    break;
                }
                init_random(array, n);
                printf("Tabloul a fost inițializat cu %d elemente aleatoare.\n", n);
                break;
            case 2:
                if (array == NULL || n == 0)
                    printf("Tabloul nu este alocat.\n");
                else
                    display_array(array, n);
                break;
            case 3:
                free_memory(&array, &n);
                break;
            case 4:
                if (array == NULL || n == 0) {
                    printf("Tabloul nu este alocat.\n");
                } else {
                    int key;
                    printf("Introduceți numărul căutat: ");
                    scanf("%d", &key);
                    double t = measure_linear_search(array, n, key);
                    printf("Timp căutare linear search: %f secunde\n", t);
                }
                break;
            case 5:
                if (array == NULL || n == 0) {
                    printf("Tabloul nu este alocat.\n");
                } else {
                    int key;
                    printf("Introduceți numărul căutat: ");
                    scanf("%d", &key);
                    double t = measure_binary_search(array, n, key);
                    printf("Timp căutare binary search (inclusiv sortare copie): %f secunde\n", t);
                }
                break;
            case 6:
                empirical_analysis();
                break;
            case 0:
                printf("Ieșire din program.\n");
                break;
            default:
                printf("Opțiune invalidă.\n");
        }
    } while (option != 0);

    if (array != NULL) free(array);
    return 0;
}