#include <stdio.h>
#include <string.h>
#include <ctype.h>

void m_s(char *s)
{
    int i, j, k, temp;
    int m, n, v, p, q, a, w, idx, len, st;
    int len_s, old_len, new_len;
    int pass, changed, ok;
    int depth, ac, an, pc;
    int mcount = 0;
    char quote;

    char mname[50][50];
    char mparam[50][6][30];
    int mpc[50];
    int mfunc[50];
    char mbody[50][200];

    char word[100];
    char word2[100];
    char rep[500];
    char args[6][100];

    //find and store all #define, and remove those lines

    for(i = 0; s[i]; i++)
    {
        if(strncmp(&s[i], "#define", 7) == 0 &&
           (i == 0 || s[i-1] == '\n' || s[i-1] == ' ' || s[i-1] == '\t') &&
           (s[i+7] == ' ' || s[i+7] == '\t'))
        {
            j = i + 7;

            while(s[j] == ' ' || s[j] == '\t')
                j++;

            if(mcount < 50)
            {
                //macro name

                n = 0;

                while((isalnum(s[j]) || s[j] == '_') && n < 49)
                {
                    mname[mcount][n] = s[j];
                    n++;
                    j++;
                }

                mname[mcount][n] = '\0';
                mpc[mcount] = 0;
                mfunc[mcount] = 0;

                //'(' immediately after name means function type macro

                if(s[j] == '(')
                {
                    mfunc[mcount] = 1;
                    j++;

                    pc = 0;

                    while(s[j] != ')' && s[j] != '\n' && s[j] != '\0')
                    {
                        while(s[j] == ' ' || s[j] == '\t')
                            j++;

                        n = 0;

                        while((isalnum(s[j]) || s[j] == '_') && n < 29)
                        {
                            mparam[mcount][pc][n] = s[j];
                            n++;
                            j++;
                        }

                        mparam[mcount][pc][n] = '\0';

                        if(n > 0 && pc < 5)
                            pc++;

                        while(s[j] == ' ' || s[j] == '\t')
                            j++;

                        if(s[j] == ',')
                            j++;
                        else if(s[j] != ')' && s[j] != '\n' &&
                                s[j] != '\0' && n == 0)
                            j++;
                    }

                    if(s[j] == ')')
                        j++;

                    mpc[mcount] = pc;
                }

                //skip spaces before body

                while(s[j] == ' ' || s[j] == '\t')
                    j++;

                //macro body

                temp = 0;

                while(s[j] != '\n' && s[j] != '\0' && temp < 199)
                {
                    mbody[mcount][temp] = s[j];
                    temp++;
                    j++;
                }

                mbody[mcount][temp] = '\0';

                //remove trailing spaces and \r

                while(temp > 0 &&
                      (mbody[mcount][temp-1] == ' ' ||
                       mbody[mcount][temp-1] == '\t' ||
                       mbody[mcount][temp-1] == '\r'))
                {
                    temp--;
                    mbody[mcount][temp] = '\0';
                }

                mcount++;
            }

            //removing this #define line

            j = i;

            while(s[j] != '\n' && s[j] != '\0')
                j++;

            if(s[j] == '\n')
                j++;

            temp = i;

            while(s[j])
            {
                s[temp] = s[j];
                temp++;
                j++;
            }

            s[temp] = '\0';

            //checking the same position again

            i--;
        }
    }

    //replacing macros, repeated so that macros inside macros also work

    for(pass = 0; pass < 10; pass++)
    {
        changed = 0;
        quote = 0;

        for(j = 0; s[j]; j++)
        {
            //skipping string and character literals

            if(quote == 0 && (s[j] == '"' || s[j] == '\''))
            {
                quote = s[j];
                continue;
            }

            if(quote != 0)
            {
                if(s[j] == quote && s[j-1] != '\\')
                    quote = 0;

                continue;
            }

            //start of a complete word

            if((isalpha(s[j]) || s[j] == '_') &&
               (j == 0 || !(isalnum(s[j-1]) || s[j-1] == '_')))
            {
                n = 0;

                while((isalnum(s[j+n]) || s[j+n] == '_') && n < 99)
                {
                    word[n] = s[j+n];
                    n++;
                }

                word[n] = '\0';

                //checking whether this word is a macro (latest definition wins)

                for(m = mcount - 1; m >= 0; m--)
                {
                    if(strcmp(word, mname[m]) == 0)
                        break;
                }

                if(m < 0)
                {
                    j = j + n - 1;
                    continue;
                }

                ok = 1;

                //simple macro

                if(mfunc[m] == 0)
                {
                    strcpy(rep, mbody[m]);
                    old_len = n;
                }

                //function type macro

                else
                {
                    p = j + n;

                    while(s[p] == ' ' || s[p] == '\t')
                        p++;

                    if(s[p] != '(')
                        ok = 0;
                    else
                    {
                        p++;

                        depth = 0;
                        ac = 0;
                        an = 0;

                        //reading actual arguments, commas inside () are ignored

                        while(s[p] != '\0')
                        {
                            if(s[p] == '(')
                                depth++;

                            if(s[p] == ')')
                            {
                                if(depth == 0)
                                    break;

                                depth--;
                            }

                            if(s[p] == ',' && depth == 0)
                            {
                                args[ac][an] = '\0';

                                if(ac < 5)
                                    ac++;

                                an = 0;
                                p++;
                                continue;
                            }

                            if(an < 99)
                            {
                                args[ac][an] = s[p];
                                an++;
                            }

                            p++;
                        }

                        args[ac][an] = '\0';

                        if(s[p] != ')')
                            ok = 0;
                    }

                    if(ok == 1)
                    {
                        //removing extra spaces at start and end of each argument

                        for(a = 0; a <= ac; a++)
                        {
                            len = strlen(args[a]);

                            while(len > 0 &&
                                  (args[a][len-1] == ' ' ||
                                   args[a][len-1] == '\t' ||
                                   args[a][len-1] == '\n'))
                            {
                                len--;
                                args[a][len] = '\0';
                            }

                            st = 0;

                            while(args[a][st] == ' ' ||
                                  args[a][st] == '\t' ||
                                  args[a][st] == '\n')
                            {
                                st++;
                            }

                            if(st > 0)
                            {
                                for(q = 0; args[a][st+q]; q++)
                                    args[a][q] = args[a][st+q];

                                args[a][q] = '\0';
                            }
                        }

                        //building replacement text from the macro body

                        v = 0;
                        k = 0;

                        while(mbody[m][k] != '\0' && v < 480)
                        {
                            if((isalpha(mbody[m][k]) || mbody[m][k] == '_') &&
                               (k == 0 ||
                                !(isalnum(mbody[m][k-1]) || mbody[m][k-1] == '_')))
                            {
                                w = 0;

                                while((isalnum(mbody[m][k+w]) ||
                                       mbody[m][k+w] == '_') && w < 99)
                                {
                                    word2[w] = mbody[m][k+w];
                                    w++;
                                }

                                word2[w] = '\0';

                                //is this word a parameter

                                idx = -1;

                                for(q = 0; q < mpc[m]; q++)
                                {
                                    if(strcmp(word2, mparam[m][q]) == 0)
                                        idx = q;
                                }

                                if(idx >= 0)
                                {
                                    if(idx <= ac)
                                    {
                                        for(q = 0; args[idx][q] && v < 490; q++)
                                        {
                                            rep[v] = args[idx][q];
                                            v++;
                                        }
                                    }
                                }
                                else
                                {
                                    for(q = 0; q < w; q++)
                                    {
                                        rep[v] = word2[q];
                                        v++;
                                    }
                                }

                                k = k + w;
                            }
                            else
                            {
                                rep[v] = mbody[m][k];
                                v++;
                                k++;
                            }
                        }

                        rep[v] = '\0';

                        //length of NAME(...) in source

                        old_len = p - j + 1;
                    }
                }

                //not a real macro call, so leave it as it is

                if(ok == 0)
                {
                    j = j + n - 1;
                    continue;
                }

                new_len = strlen(rep);
                len_s = strlen(s);

                //making space to insert

                if(new_len > old_len)
                {
                    for(k = len_s; k >= j + old_len; k--)
                        s[k + new_len - old_len] = s[k];
                }

                //removing extra space

                else if(new_len < old_len)
                {
                    for(k = j + old_len; k <= len_s; k++)
                        s[k - old_len + new_len] = s[k];
                }

                //copy replacement

                for(k = 0; k < new_len; k++)
                    s[j + k] = rep[k];

                changed = 1;

                //continue after the replacement

                j = j + new_len - 1;
            }
        }

        if(changed == 0)
            break;
    }
}
