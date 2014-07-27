/* strtok example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[] ="ldc";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  sscanf(str, "%[^;]s", str);
  pch = strtok (str," ");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ");
  }
  return 0;
}