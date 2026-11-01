#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void c_r(char *);
void h_i(char *);
void m_s(char *);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./mypreprocessor filename.c\n");
        return 1;
    }

    FILE *fp, *w;
    int c;
    int i = 0;
    int size = 1000;
    char *s;
    char output[256];
    char *dot;

    s = malloc(size * sizeof(char));

    if (s == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Input file not found\n");
        free(s);
        return 1;
    }

    while ((c = fgetc(fp)) != EOF)
    {
        if (i >= size - 1)
        {
            size = size * 2;

            char *temp = realloc(s, size * sizeof(char));

            if (temp == NULL)
            {
                printf("Memory allocation failed\n");
                fclose(fp);
                free(s);
                return 1;
            }

            s = temp;
        }

        s[i] = c;
        i++;
    }

    s[i] = '\0';
    fclose(fp);

    // Remove comments
    c_r(s);

    // Include header files
    h_i(s);

    // Substitute macros
    m_s(s);

    if (strlen(argv[1]) >= sizeof(output))
    {
        printf("Filename is too long\n");
        free(s);
        return 1;
    }

    strcpy(output, argv[1]);

    dot = strrchr(output, '.');

    if (dot != NULL)
    {
        strcpy(dot, ".i");
    }
    else
    {
        strcat(output, ".i");
    }

    w = fopen(output, "w");

    if (w == NULL)
    {
        printf("Output file error\n");
        free(s);
        return 1;
    }

    fputs(s, w);

    fclose(w);
    free(s);

    printf("Preprocessing completed: %s\n", output);

    return 0;
}
