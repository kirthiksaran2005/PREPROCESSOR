#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void h_i(char *s)
{
    int i, j, k;
    int temp;
    int len_s, len_p;
    int start, end;
    
    char t[50];
    char p[500];

    FILE *fp;
    char ch;

    for(i = 0; s[i]; i++)
    {
        if(s[i]=='#' && s[i+1]=='i' && s[i+2]=='n' &&
           s[i+3]=='c' && s[i+4]=='l' && s[i+5]=='u' &&
           s[i+6]=='d' && s[i+7]=='e')
        {
            /*
             * Find <
             */
            j = i + 8;

            while(s[j] != '<')
                j++;

            /*
             * Move after <
             */
            j++;

            /*
             * Extract header filename
             */
            temp = 0;

            while(s[j] != '>')
            {
                t[temp] = s[j];
                temp++;
                j++;
            }

            t[temp] = '\0';

            /*
             * Open header file
             */
            fp = fopen(t, "r");

            if(fp == NULL)
            {
                printf("Cannot open %s\n", t);
                return;
            }

            /*
             * Read header contents into p[]
             */
            k = 0;

            while((ch = fgetc(fp)) != EOF)
            {
                p[k] = ch;
                k++;
            }

            p[k] = '\0';

            fclose(fp);

            /*
             * Find end of #include line
             */
            while(s[j] != '\n' && s[j] != '\0')
                j++;

            if(s[j] == '\n')
                j++;

            /*
             * i  = beginning of #include
             * j  = beginning of remaining source
             */

            start = i;
            end = j;

            /*
             * Find length of complete source
             */
            len_s = 0;

            while(s[len_s])
                len_s++;

            /*
             * Find length of header
             */
            len_p = 0;

            while(p[len_p])
                len_p++;

            /*
             * Move remaining source
             * to make space for header
             */
            if(len_p > (end - start))
            {
                for(k = len_s; k >= end; k--)
                {
                    s[k + (len_p - (end - start))] = s[k];
                }
            }
            else
            {
                for(k = end; k <= len_s; k++)
                {
                    s[k - (end - start) + len_p] = s[k];
                }
            }

            /*
             * Copy header contents
             * into #include position
             */
            for(k = 0; k < len_p; k++)
            {
                s[start + k] = p[k];
            }

            /*
             * Continue scanning after
             * the inserted header
             */
            i = start + len_p - 1;
        }
    }
}
