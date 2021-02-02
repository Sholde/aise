#include <stdlib.h>

int main(void)
{
  int **p = malloc(sizeof(int *) * 10);
  for (int i = 0; i < 10; i++)
    {
        p[i] = malloc(sizeof(int) * 10);
        free(p[i]);
    }
  free(p);
  return 0;
}
