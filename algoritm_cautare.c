#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------------------- Functii pentru tablou --------------------
void init_random(int *arr, int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;   // numere intre 0 si 9999
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
        printf("Memoria a fost eliberata.\n");
    } else {
        printf("Tabloul nu este alocat.\n");
    }
}

// -------------------- Algoritmi de cautare --------------------
// Linear search: returneaza indexul sau -1
int linear_search(int *arr, int n, int key) {
    for (int i = 0; i < n; i++)
        if (arr[i] == key)
            return i;
    return -1;
}

// Binary search (iterativ) – necesita tablou sortat crescator
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
// Quick sort (necesar pentru a sorta inainte de cautarea binara)
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

// -------------------- Masurare timp pentru o singura cautare --------------------
double measure_linear_search(int *arr, int n, int key) {
    clock_t start = clock();
    linear_search(arr, n, key);
    clock_t end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double measure_binary_search(int *arr, int n, int key) {
    // Sortam o copie pentru a nu modifica originalul 
    int *copy = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) copy[i] = arr[i];
    quick_sort(copy, 0, n - 1);
    clock_t start = clock();
    binary_search(copy, n, key);
    clock_t end = clock();
    free(copy);
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// -------------------- Analiza empirica: 10000 de cautari --------------------
void empirical_analysis() {
    int sizes[] = {100, 1000, 10000, 100000};
    double linear_times[4], binary_times[4];

    printf("\n=== ANALIZA EMPIRICA (10.000 de cautari pentru fiecare N) ===\n");
    printf("Se genereaza tablouri aleatoare si se masoara timpul...\n\n");

    for (int i = 0; i < 4; i++) {
        int N = sizes[i];
        int *arr = (int*)malloc(N * sizeof(int));
        init_random(arr, N);

        // ----- Linear search: 10000 cautari -----
        clock_t start_lin = clock();
        for (int k = 0; k < 10000; k++) {
            int key = rand() % 10000;   // cheie aleatoare in acelasi interval
            linear_search(arr, N, key);
        }
        clock_t end_lin = clock();
        linear_times[i] = (double)(end_lin - start_lin) / CLOCKS_PER_SEC;

        // ----- Binary search: sortam o singura data, apoi 10000 cautari -----
        // Sortam o copie pentru a nu distruge array-ul original (ne trebuie doar timpul cautarilor)
        int *sorted_copy = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) sorted_copy[j] = arr[j];
        quick_sort(sorted_copy, 0, N - 1);   // sortare o singura data
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

    // Afisare tabel
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
        printf("1. Initializare tablou cu numere aleatorii\n");
        printf("2. Afisare tablou\n");
        printf("3. Eliberare memorie tablou\n");
        printf("4. Cautare liniara (masoara timpul)\n");
        printf("5. Cautare binara (masoara timpul)\n");
        printf("6. Analiza empirica (10.000 cautari pentru N = 100, 1000, 10000, 100000)\n");
        printf("0. Iesire\n");
        printf("Alegere: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("Dimensiunea tabloului: ");
                scanf("%d", &n);
                if (n <= 0) {
                    printf("Dimensiune invalida.\n");
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
                printf("Tabloul a fost initializat cu %d elemente aleatoare.\n", n);
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
                    printf("Introduceti numarul cautat: ");
                    scanf("%d", &key);
                    double t = measure_linear_search(array, n, key);
                    printf("Timp cautare linear search: %f secunde\n", t);
                }
                break;
            case 5:
                if (array == NULL || n == 0) {
                    printf("Tabloul nu este alocat.\n");
                } else {
                    int key;
                    printf("Introduceti numarul cautat: ");
                    scanf("%d", &key);
                    double t = measure_binary_search(array, n, key);
                    printf("Timp cautare binary search (inclusiv sortare copie): %f secunde\n", t);
                }
                break;
            case 6:
                empirical_analysis();
                break;
            case 0:
                printf("Iesire din program.\n");
                break;
            default:
                printf("Optiune invalida.\n");
        }
    } while (option != 0);

    if (array != NULL) free(array);
    return 0;
}
