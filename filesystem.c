#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTENT_SIZE 1024

// 1.1 Функция открытия/создания файловой системы
FILE* open_or_create_filesystem(const char* filename) {
    FILE* fs = fopen(filename, "a+"); // Открываем для чтения и записи, создаем если не существует
    if (fs == NULL) {
        perror("Error opening filesystem");
        exit(EXIT_FAILURE);
    }
    return fs;
}

// 1.2 Функция просмотра файла в файловой системе
char* view_file_in_filesystem(FILE* fs, const char* filename) {
    fseek(fs, 0, SEEK_SET);
    char line[MAX_CONTENT_SIZE];
    char* content = NULL;
    int found = 0;
    size_t content_size = 0;

    while (fgets(line, sizeof(line), fs) {
        line[strcspn(line, "\n")] = 0; // Удаляем символ новой строки

        if (found) {
            if (line[0] == '/') { // Конец файла
                break;
            }
            // Добавляем строку к содержимому
            content = realloc(content, content_size + strlen(line) + 2);
            strcpy(content + content_size, line);
            content_size += strlen(line);
            strcpy(content + content_size, "\n");
            content_size += 1;
        } else if (strcmp(line, filename) == 0) {
            found = 1;
        }
    }

    if (!found) {
        free(content);
        return NULL;
    }
    return content;
}

// 1.3 Функция удаления файла из файловой системы
int delete_file_from_filesystem(FILE* fs, const char* filename) {
    fseek(fs, 0, SEEK_SET);
    char** lines = NULL;
    int line_count = 0;
    int in_target_file = 0;
    char line[MAX_CONTENT_SIZE];

    // Читаем все строки
    while (fgets(line, sizeof(line), fs)) {
        line[strcspn(line, "\n")] = 0;

        if (!in_target_file && strcmp(line, filename) == 0) {
            in_target_file = 1;
            continue;
        }

        if (in_target_file) {
            if (line[0] == '/') {
                in_target_file = 0;
            } else {
                continue;
            }
        }

        // Добавляем строку в массив
        lines = realloc(lines, (line_count + 1) * sizeof(char*));
        lines[line_count] = strdup(line);
        line_count++;
    }

    // Перезаписываем файл
    freopen(NULL, "w", fs);
    for (int i = 0; i < line_count; i++) {
        fprintf(fs, "%s\n", lines[i]);
        free(lines[i]);
    }
    free(lines);

    return 0;
}
