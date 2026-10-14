#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

#define MAX 20

int adj[MAX][MAX];          // матрица смежности
int inc[MAX][MAX * MAX];    // матрица инцидентности
int n;                      // число вершин
int m = 0;                  // число рёбер


void generate_adj(void) {
    srand((unsigned)time(NULL));

    printf("Введите число вершин (2..%d): ", MAX);
    (void)scanf("%d", &n);

    if (n < 2 || n > MAX) {
        printf("Недопустимое число вершин\n");
        exit(1);
    }

    
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adj[i][j] = 0;

    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 2 == 1) {
                adj[i][j] = 1;
                adj[j][i] = 1;
            }
        }
    }
    printf("Матрица смежности сгенерирована.\n");
}


void print_adj(void) {
    printf("\nМатрица смежности:\n   ");
    for (int j = 0; j < n; j++) printf("%3d", j + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < n; j++)
            printf("%3d", adj[i][j]);
        printf("\n");
    }
}


int graph_size(void) {
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (adj[i][j] == 1) count++;
    return count;
}


void find_vertices(void) {
    printf("\nХарактеристики вершин:\n");
    for (int i = 0; i < n; i++) {
        int deg = 0;
        for (int j = 0; j < n; j++)
            deg += adj[i][j];    // степень = сумма строки

        printf("Вершина %d: степень = %d", i + 1, deg);
        if (deg == 0) printf(" — изолированная");
        if (deg == 1) printf(" — концевая");
        if (deg == n - 1) printf(" — доминирующая");
        printf("\n");
    }
}


void build_inc(void) {
    m = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (adj[i][j] == 1) {
                // создаём ребро m
                for (int k = 0; k < n; k++) inc[k][m] = 0;
                inc[i][m] = 1;
                inc[j][m] = 1;
                m++;
            }
    printf("Матрица инцидентности построена. Рёбер: %d\n", m);
}


void print_inc(void) {
    if (m == 0) {
        printf("Рёбер нет — матрица инцидентности пустая\n");
        return;
    }
    printf("\nМатрица инцидентности (вершины × рёбра):\n     ");
    for (int j = 0; j < m; j++) printf("%3d", j + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("v%-3d ", i + 1);
        for (int j = 0; j < m; j++)
            printf("%3d", inc[i][j]);
        printf("\n");
    }
}


int graph_size_inc(void) {
    return m;
}


void find_vertices_inc(void) {
    printf("\nХарактеристики вершин (по матрице инцидентности):\n");
    for (int i = 0; i < n; i++) {
        int deg = 0;
        for (int j = 0; j < m; j++)
            deg += inc[i][j];    // степень = сумма строки

        printf("Вершина %d: степень = %d", i + 1, deg);
        if (deg == 0) printf(" — изолированная");
        if (deg == 1) printf(" — концевая");
        if (deg == n - 1) printf(" — доминирующая");
        printf("\n");
    }
}


int main() {
    setlocale(LC_ALL, "Russian");
    int choice;

    do {
        printf("\n=== Графы ===\n");
        printf("1 - сгенерировать матрицу смежности\n");
        printf("2 - вывести матрицу смежности\n");
        printf("3 - размер графа (по смежности)\n");
        printf("4 - характеристики вершин (по смежности)\n");
        printf("5 - построить матрицу инцидентности\n");
        printf("6 - вывести матрицу инцидентности\n");
        printf("7 - размер графа (по инцидентности)\n");
        printf("8 - характеристики вершин (по инцидентности)\n");
        printf("0 - выход\n");
        printf("Ваш выбор: ");
        (void)scanf("%d", &choice);

        switch (choice) {
        case 1: generate_adj(); break;
        case 2: print_adj(); break;
        case 3:
            printf("Размер графа = %d\n", graph_size());
            break;
        case 4: find_vertices(); break;
        case 5: build_inc(); break;
        case 6: print_inc(); break;
        case 7:
            printf("Размер графа = %d\n", graph_size_inc());
            break;
        case 8: find_vertices_inc(); break;
        case 0: printf("Выход.\n"); break;
        default: printf("Неверный выбор\n");
        }
    } while (choice != 0);

    return 0;
}
// динамическое выделение памяти и для смежности и для инцедентности так же пользователь вводит кол во вершин определяме по нему размер а затем происходит выделение памяти для матрицы инцедентности памят под нее выделяется после смежности добавить возможность появление петли 