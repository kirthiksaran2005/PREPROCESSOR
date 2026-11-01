#include <stdio.h>
//single line comment removal

void c_r(char *s)
{
int j, temp, i;
for(i=0;s[i];i++)
{
temp = i;
if(s[temp] == '/' && s[temp+1] == '/')
{
    j = temp;

    while(s[j] != '\n' && s[j] != '\0')
        j++;

    if(s[j] == '\n')
        j++;

    while(s[j] != '\0')
    {
        s[temp] = s[j];
        temp++;
        j++;
    }

    s[temp] = '\0';

    i--;
}
}
for(i=0;s[i];i++)
{
temp = i;
if(s[temp] == '/' && s[temp+1] == '*')
{
    j = temp;

    while(!(s[j] == '*' && s[j+1]=='/'))
        j++;

    j+=2;

    while(s[j] != '\0')
    {
        s[temp] = s[j];
        temp++;
        j++;
    }

    s[temp] = '\0';

    i--;
}
}
}

