#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compile(char* data, FILE* file, int isppfile) {
    if (!data) return;

    fprintf(file, "#include <stdio.h>\n\n");
    fprintf(file, "int main() {\n");
    fprintf(file, "long ptr[30000] = {0};\n");
    fprintf(file, "long index = 0;\n");

    while (*data) {
        if (*data < 32) {
            ++data;
            continue;
        }
        switch (*data) {
            case '>': fprintf(file, "++index;\n"); break;
            case '<': fprintf(file, "--index;\n"); break;
            case '+': fprintf(file, "++ptr[index];\n"); break;
            case '-': fprintf(file, "--ptr[index];\n"); break;
            case '?':
                if (isppfile) {
                    fprintf(file, "ptr[index] = index;\n");
                } else {
                    fprintf(stderr, "Set Pointer is only supported in bf++\n");
                }
                break;
            case '!':
                if (isppfile) {
                    fprintf(file, "index = ptr[index];\n");
                } else {
                    fprintf(stderr, "Set Pointer is only supported in bf++\n");
                }
                break;
            case '=':
                if (isppfile) {
                    int num = 0;
                    int sign;
                    ++data;
                    sign = *data == '-' ? -1 : 1;
                    if (*data == '-' || *data == '+') ++data;
                    while (*data >= '0' && *data <= '9') {
                        num = num * 10 + *data - '0';
                        ++data;
                    }
                    fprintf(file, "ptr[index] = %d;\n", sign * num);
                    --data;
                } else {
                    fprintf(stderr, "Set Cell is only supported in bf++\n");
                }
                break;
            case '#':
            case ':':
            case ';':
                if (isppfile) {
                    char op = (*data == '#' ? ' ' : (*data == ':' ? '+' : '-'));
                    int num = 0;
                    int sign;
                    ++data;
                    sign = *data == '-' ? -1 : 1;
                    if (*data == '-' || *data == '+') ++data;
                    while (*data >= '0' && *data <= '9') {
                        num = num * 10 + *data - '0';
                        ++data;
                    }
                    fprintf(file, "index %c= %d;\n", op, sign * num);
                    --data;
                } else {
                    fprintf(stderr, "Move Pointer is only supported in bf++\n");
                }
                break;
            case '/':
                if (isppfile) {
                    fprintf(file, "return ptr[index];\n");
                } else {
                    fprintf(stderr, "Return is only supported in bf++\n");
                }
                break;
            case '.': fprintf(file, "putchar(ptr[index]);\n"); break;
            case ',': fprintf(file, "ptr[index] = getchar();\n"); break;
            case '[':
                fprintf(file, "while (ptr[index]) {\n");
                break;
            case '(':
                if (isppfile) {
                    fprintf(file, "if (ptr[index]) {\n");
                } else {
                    fprintf(stderr, "If is only supported in bf++\n");
                }
                break;
            case ']':
                fprintf(file, "}\n");
                break;
            case ')':
                if (isppfile) {
                    fprintf(file, "}\n");
                } else {
                    fprintf(stderr, "If is only supported in bf++\n");
                }
                break;
        }
        ++data;
    }

    fprintf(file, "}\n");

    return;
}

int strends(const char* str, const char* end) {
    size_t len = strlen(str);
    size_t endlen = strlen(end);
    if (len < endlen) return 0;
    return strcmp(str + len - endlen, end) == 0;
}

int main(int argc, char const *argv[]) {
    FILE* file;
    FILE* out;
    long size;
    char* data;

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: cannot open file %s\n", argv[1]);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    data = malloc(size);

    fread(data, sizeof(char), size, file);
    fclose(file);

    out = fopen("out.c", "w");
    compile(data, out, strends(argv[1], ".bfpp") || strends(argv[1], ".bf++"));
    fclose(out);

#if defined(__clang__)
    return system("clang -O3 -o out out.c");
#elif defined(__GNUC__)
    return system("gcc -O3 -o out out.c");
#elif defined(_MSC_VER)
    return system("cl /O2 /Feout out.c");
#else
#error "Unsupported compiler"
#endif
}
