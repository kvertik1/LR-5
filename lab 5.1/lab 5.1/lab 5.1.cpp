#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

int** adj = NULL;    // матрица смежности (динамическая)
int** inc = NULL;    // матрица инцидентности (динамическая)
int n = 0;           // число вершин (вводит пользователь)
int m = 0;           // число рёбер (определяется при генерации)

void print_adj(void);
void print_inc(void);
void experiment_isolated_loop(void);   // НОВОЕ: прототип

void allocate_adj(void) {
    adj = (int**)malloc(n * sizeof(int*));
    if (adj == NULL) {
        printf("Ошибка выделения памяти под матрицу смежности\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        adj[i] = (int*)malloc(n * sizeof(int));
        if (adj[i] == NULL) {
            printf("Ошибка выделения памяти под строку %d\n", i);
            exit(1);
        }
    }
}

void allocate_inc(void) {
    int cols = n * n;

    inc = (int**)malloc(n * sizeof(int*));
    if (inc == NULL) {
        printf("Ошибка выделения памяти под матрицу инцидентности\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        inc[i] = (int*)malloc(cols * sizeof(int));
        if (inc[i] == NULL) {
            printf("Ошибка выделения памяти под строку %d\n", i);
            exit(1);
        }
    }
}

void free_adj(void) {
    if (adj == NULL) return;
    for (int i = 0; i < n; i++) free(adj[i]);
    free(adj);
    adj = NULL;
}

void free_inc(void) {
    if (inc == NULL) return;
    for (int i = 0; i < n; i++) free(inc[i]);
    free(inc);
    inc = NULL;
}

//было
void generate_adj(void) {
    srand((unsigned)time(NULL));

    printf("Введите число вершин: ");
    (void)scanf("%d", &n);

    if (n < 2) {
        printf("Недопустимое число вершин (минимум 2)\n");
        exit(1);
    }

    free_adj();
    free_inc();

    allocate_adj();

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

    //Ген петли (с вер 50%)
    for (int i = 0; i < n; i++) {
        if (rand() % 2 == 1) {
            adj[i][i] = 1;
        }
    }

    //после выделяем память под матрицу инцидентности
    allocate_inc();

    printf("Матрицы созданы. Память выделена под обе.\n");

    print_adj();   // НОВОЕ: сразу выводим матрицу смежности
}

void print_adj(void) {
    if (adj == NULL) {
        printf("Матрица смежности не создана\n");
        return;
    }
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
    if (adj == NULL) return 0;
    int count = 0;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (adj[i][j] == 1) count++;
    for (int i = 0; i < n; i++)
        if (adj[i][i] == 1) count++;   // НОВОЕ: учёт петель в размере
    return count;
}

void find_vertices(void) {
    if (adj == NULL) {
        printf("Матрица смежности не создана\n");
        return;
    }
    printf("\nХарактеристики вершин:\n");
    for (int i = 0; i < n; i++) {
        int deg = 0;
        for (int j = 0; j < n; j++)
            deg += adj[i][j];

        if (adj[i][i] == 1) deg++;   // НОВОЕ: петля даёт 2

        printf("Вершина %d: степень = %d", i + 1, deg);
        if (deg == 0) printf(" — изолированная");
        if (deg == 1) printf(" — концевая");

        // доминирующая — есть рёбра ко всем остальным
        int isDominant = 1;
        for (int j = 0; j < n; j++) {
            if (i != j && adj[i][j] == 0) {
                isDominant = 0;
                break;
            }
        }
        if (isDominant) printf(" — доминирующая");

        if (adj[i][i] == 1) printf(" — с петлёй");
        if (adj[i][i] == 1 && deg == 2) printf(" (изолированная с петлёй)");   // НОВОЕ

        printf("\n");
    }
}

void build_inc(void) {
    if (adj == NULL || inc == NULL) {
        printf("Матрицы не выделены. Сначала сгенерируйте смежность\n");
        return;
    }

    m = 0;

    //рёбра
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (adj[i][j] == 1) {
                for (int k = 0; k < n; k++) inc[k][m] = 0;
                inc[i][m] = 1;
                inc[j][m] = 1;
                m++;
            }

    // петли — одна единица в столбце
    for (int i = 0; i < n; i++) {
        if (adj[i][i] == 1) {
            for (int k = 0; k < n; k++) inc[k][m] = 0;
            inc[i][m] = 1;
            m++;
        }
    }

    printf("Матрица инцидентности построена. Рёбер: %d\n", m);

    print_inc();   // НОВОЕ: сразу выводим матрицу инцидентности
}

void print_inc(void) {
    if (inc == NULL) {
        printf("Матрица инцидентности не выделена\n");
        return;
    }
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
    if (inc == NULL) {
        printf("Матрица инцидентности не выделена\n");
        return;
    }
    printf("\nХарактеристики вершин (по матрице инцидентности):\n");
    for (int i = 0; i < n; i++) {
        int deg = 0;
        for (int j = 0; j < m; j++)
            deg += inc[i][j];

        // доминирующая для инцедентности
        int isDominant = 1;
        for (int v = 0; v < n; v++) {
            if (i == v) continue;
            int found = 0;
            for (int e = 0; e < m; e++) {
                if (inc[i][e] == 1 && inc[v][e] == 1) {
                    found = 1;
                    break;
                }
            }
            if (!found) { isDominant = 0; break; }
        }

        // петля
        int hasLoop = 0;
        for (int e = 0; e < m; e++) {
            if (inc[i][e] == 1) {
                int cnt = 0;
                for (int k = 0; k < n; k++) cnt += inc[k][e];
                if (cnt == 1) { hasLoop = 1; break; }
            }
        }

        if (hasLoop) deg++;   // НОВОЕ: петля даёт 2

        printf("Вершина %d: степень = %d", i + 1, deg);
        if (deg == 0) printf(" — изолированная");
        if (deg == 1) printf(" — концевая");
        if (isDominant) printf(" — доминирующая");
        if (hasLoop) printf(" — с петлёй");
        if (hasLoop && deg == 2) printf(" (изолированная с петлёй)");   // НОВОЕ

        printf("\n");
    }
}

// НОВОЕ: эксперимент — короткая версия
void experiment_isolated_loop(void) {
    printf("\n=== ЭКСПЕРИМЕНТ: изолированная вершина с петлёй ===\n");

    free_adj();
    free_inc();

    n = 3;
    allocate_adj();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            adj[i][j] = 0;

    adj[0][0] = 1;
    adj[1][2] = 1;
    adj[2][1] = 1;

    allocate_inc();

    print_adj();
    build_inc();
    find_vertices();
    find_vertices_inc();
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice;

    do {
        printf("\n=== Графы ===\n");
        printf("1 - создать матрицы (ввод вершин + генерация + вывод)\n");   // НОВОЕ: вывод автоматически
        printf("2 - вывести матрицу смежности повторно\n");                   // НОВОЕ: "повторно"
        printf("3 - размер графа (по смежности)\n");
        printf("4 - характеристики вершин (по смежности)\n");
        printf("5 - построить матрицу инцидентности (+ вывод)\n");            // НОВОЕ: вывод автоматически
        printf("6 - вывести матрицу инцидентности повторно\n");               // НОВОЕ: "повторно"
        printf("7 - размер графа (по инцидентности)\n");
        printf("8 - характеристики вершин (по инцидентности)\n");
        printf("9 - ЭКСПЕРИМЕНТ: изолированная вершина с петлёй\n");          // НОВОЕ
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
        case 9: experiment_isolated_loop(); break;   // НОВОЕ
        case 0:
            free_adj();
            free_inc();
            printf("Выход. Память освобождена.\n");
            break;
        default: printf("Неверный выбор\n");
        }
    } while (choice != 0);

    return 0;
}