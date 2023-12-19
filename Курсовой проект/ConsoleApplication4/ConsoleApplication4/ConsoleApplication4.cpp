#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>
#include <string.h>
#include <time.h>

#define MAX_VERTICES 100
#define MAX_FILENAME_LENGTH 100

int parent[MAX_VERTICES];

// Прототипы функций
void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, int start, int end, int* shortestPathLength);
void fillRandomGraph(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, int isDirected);
void fillUndirectedGraphRandom(int graph[MAX_VERTICES][MAX_VERTICES], int vertices);
int loadGraphFromFile(int graph[MAX_VERTICES][MAX_VERTICES], int* vertices, char* filename);
void saveGraphToFile(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, char* filename);
void printShortestPath(int graph[MAX_VERTICES][MAX_VERTICES], int start, int end, int shortestPathLength);
void printGraph(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, int printWeights);
int getInputInteger(const char* prompt);
char* getInputString(const char* prompt, char* buffer, size_t bufferSize);

int main() {
    setlocale(LC_ALL, "RUS");
    int graph[MAX_VERTICES][MAX_VERTICES] = { 0 };
    int vertices = 0;
    int choice;
    int graphFilled = 0;
    int shortestPathLength = INT_MAX;
    char filename[MAX_FILENAME_LENGTH];

    printf("Пензенский государственный университет\n");
    printf("Кафедра «Вычислительная техника»\n");
    printf("Приняли: Юрова О.В. и Акифьев И.В.\n");
    printf("Курсовая работа\n");
    printf("По курсу \"Логика и основы алгоритмизации в инженерных задачах\"\n");
    printf("На тему \"Реализация алгоритма Дейкстры\"\n");
    printf("Выполнил: Тельнов Илья Владимирович, группа: 22ВВВ3 (22ВВП2)\n");

    while (1) {
        printf("\nМеню:\n");
        printf("1. Заполнение графа\n");
        printf("2. Загрузка графа из файла\n");
        printf("3. Сохранение графа в файл\n");
        printf("4. Выполнение алгоритма Дейкстры\n");
        printf("5. Просмотр связей вершин в графе\n");
        printf("0. Выход\n");
        printf("\nВаш выбор: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
            fflush(stdin); // Очистка буфера ввода
            continue;
        }

        switch (choice) {
        case 1: {
            printf("Выберите тип графа:\n");
            printf("1. Ориентированный\n");
            printf("2. Неориентированный\n");
            printf("\nВаш выбор: ");
            int graphTypeChoice;
            if (scanf("%d", &graphTypeChoice) != 1) {
                printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
                fflush(stdin);
                break;
            }
            vertices = getInputInteger("Введите количество вершин (max 100): ");
            if (vertices <= 0 || vertices > MAX_VERTICES) {
                printf("Ошибка: количество вершин должно быть в пределах от 1 до %d\n", MAX_VERTICES);
                break;
            }
            if (graphTypeChoice == 1 || graphTypeChoice == 2) {
                if (graphTypeChoice == 1) {
                    fillRandomGraph(graph, vertices, 1);
                }
                else {
                    fillUndirectedGraphRandom(graph, vertices);
                }
                graphFilled = 1;
                printGraph(graph, vertices, 1);
            }
            else {
                printf("Ошибка: неверный выбор типа графа.\n");
            }
            break;
        }
        case 2:
            if (!graphFilled) {
                // Оставьте запрос на имя файла
                char filename_load[MAX_FILENAME_LENGTH];
                printf("Введите имя файла для загрузки: ");
                scanf("%s", filename_load);
                if (loadGraphFromFile(graph, &vertices, filename_load)) {
                    graphFilled = 1;
                }
                else {
                    printf("Ошибка при загрузке графа из файла.\n");
                }
            }
            else {
                printf("Граф уже заполнен. Выберите другую опцию.\n");
            }
            break;
        case 3:
            if (graphFilled) {
                saveGraphToFile(graph, vertices, getInputString("Введите имя файла для сохранения: ", filename, MAX_FILENAME_LENGTH));
            }
            else {
                printf("Граф не был заполнен. Выберите способ заполнения графа перед сохранением в файл.\n");
            }
            break;
        case 4:
            if (!graphFilled) {
                printf("Граф не был заполнен. Выберите способ заполнения графа перед выполнением алгоритма Дейкстры.\n");
            }
            else {
                int startVertex = getInputInteger("Введите начальную вершину для алгоритма Дейкстры: ");
                dijkstra(graph, vertices, startVertex, vertices - 1, &shortestPathLength);
                printf("Рекомендуемый самый короткий путь от вершины %d до вершины %d: ", startVertex, vertices - 1);
                printShortestPath(graph, startVertex, vertices - 1, shortestPathLength);
                printf("\n");
            }
            break;
        case 5:
            if (graphFilled) {
                printGraph(graph, vertices, 0);
            }
            else {
                printf("Граф не был заполнен. Выберите способ заполнения графа перед просмотром связей.\n");
            }
            break;
        case 0:
            return 0; // Завершение программы при выборе "0"
        default:
            printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
            break;
        }
    }

    return 0;
}

void printGraph(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, int printWeights) {
    if (printWeights) {
        printf("Веса рёбер графа:\n");
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                printf("%4d ", graph[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("Связи вершин в графе:\n");
        for (int i = 0; i < vertices; ++i) {
            printf("Вершина %d связана с: ", i);
            int first = 1;
            for (int j = 0; j < vertices; ++j) {
                if (graph[i][j]) {
                    if (!first) {
                        printf(", ");
                    }
                    printf("%d", j);
                    first = 0;
                }
            }
            printf("\n");
        }
    }
}

void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, int start, int end, int* shortestPathLength) {
    int visited[MAX_VERTICES] = { 0 };
    int distance[MAX_VERTICES];

    for (int i = 0; i < vertices; ++i) {
        distance[i] = INT_MAX;
        visited[i] = 0;
    }

    distance[start] = 0;
    parent[start] = -1;

    for (int i = 0; i < vertices - 1; ++i) {
        int minDistance = INT_MAX;
        int minIndex = -1;

        for (int j = 0; j < vertices; ++j) {
            if (!visited[j] && distance[j] < minDistance) {
                minDistance = distance[j];
                minIndex = j;
            }
        }

        visited[minIndex] = 1;

        for (int j = 0; j < vertices; ++j) {

            if (!visited[j] && graph[minIndex][j] && distance[minIndex] != INT_MAX
                && distance[minIndex] + graph[minIndex][j] < distance[j]) {
                distance[j] = distance[minIndex] + graph[minIndex][j];
                parent[j] = minIndex;
            }
        }
    }

    *shortestPathLength = distance[end];
}

void fillRandomGraph(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, int isDirected) {
    srand(time(NULL));  // Инициализация генератора случайных чисел

    // Вероятность установки связи между вершинами (в процентах)
    int connectionProbability = 20; // Например, 30% вероятность

    // Заполняем граф значениями от 0 до 100
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j && (rand() % 50) < connectionProbability) {
                graph[i][j] = rand() % 101; // Задаем случайный вес связи от 0 до 10
                if (!isDirected) {
                    graph[j][i] = graph[i][j]; // Для неориентированного графа отражаем изменения
                }
            }
            else {
                graph[i][j] = 0;
            }
        }
    }
}

void fillUndirectedGraphRandom(int graph[MAX_VERTICES][MAX_VERTICES], int vertices)
{
    srand(time(NULL));
    int connectionProbability = 20; // Например, 30% вероятность 

    // Заполнение графа нулями и единицами на главной диагонали 
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j && (rand() % 50) < connectionProbability) {
                if (i == j) {
                    graph[i][j] = 0; // Главная диагональ остается нулевой

                }
                else {
                    graph[i][j] = rand() % 101; // Заполнение ребер 0 или 1
                    graph[j][i] = graph[i][j]; // Отражение изменений для неориентированного графа

                }
            }
        }
    }
// Замена единиц случайными числами 
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (i != j && graph[i][j] == 1) {
            graph[i][j] = rand() % 100; // Заполнение случайным числом вместо единицы 
            graph[j][i] = graph[i][j]; // Отражение изменений для неориентированного

            }
        }
    }
}


// Загрузка графа из файла
int loadGraphFromFile(int graph[MAX_VERTICES][MAX_VERTICES], int* vertices, char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Не удалось открыть файл %s\n", filename);
        return 0;
    }

    fscanf(file, "%d", vertices);

    for (int i = 0; i < *vertices; ++i) {
        for (int j = 0; j < *vertices; ++j) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);
    return 1;
}

// Функция сохранения графа в файл
void saveGraphToFile(int graph[MAX_VERTICES][MAX_VERTICES], int vertices, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Не удалось создать файл %s\n", filename);
        return;
    }

    fprintf(file, "%d\n", vertices);

    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            fprintf(file, "%d ", graph[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file); // Закрываем файл после записи данных
}

// Функция вывода кратчайшего пути


// Функция вывода кратчайшего пути
void printShortestPath(int graph[MAX_VERTICES][MAX_VERTICES], int start, int end, int shortestPathLength) {
    if (parent[end] == -1) {
        printf("Путь не найден.\n");
        return;
    }

    printf("%d", start);
    int current = end;
    int totalWeight = 0; // Суммарный вес кратчайшего пути

    while (current != -1) {
        printf(" -> %d", current);
        if (parent[current] != -1) {
            totalWeight += graph[parent[current]][current];
        }
        current = parent[current];
    }

    printf(" (суммарный вес: %d)", totalWeight);
}


int getInputInteger(const char* prompt) {
    int value;
    printf("%s", prompt);
    while (1) {
        if (scanf("%d", &value) == 1) {
            while (getchar() != '\n');  // Очистка буфера ввода
            return value;
        }
        else {
            printf("Ошибка ввода. Пожалуйста, введите целое число: ");
            while (getchar() != '\n');  // Очистка буфера ввода
        }
    }
}

char* getInputString(const char* prompt, char* buffer, size_t bufferSize) {
    printf("%s", prompt);
    while (1) {
        if (scanf("%s", buffer) == 1) {
            while (getchar() != '\n');  // Очистка буфера ввода
            return buffer;
        }
        else {
            printf("Ошибка ввода. Пожалуйста, введите строку: ");
            while (getchar() != '\n');  // Очистка буфера ввода
        }
    }
}