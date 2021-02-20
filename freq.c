// COPYRIGHT 2020 STANCA ADELIN-NICOLAE
#include <stdio.h>
#include <stdlib.h>

#define NMAX 2000001

int main() {
  int *ap, i, x, mx = -1;
  // initializare vector de aparitii cu 0
  ap = calloc(NMAX, sizeof(int));
  while (fscanf(stdin, "%d", &x) > 0) {
    ap[x]++;
    if (x > mx) {
      mx = x;
    }
  }
  // parcurg pana la maximul din fisier pentru eficienta timp
  for (i = 0; i <= mx; i++) {
    if (ap[i] != 0) {
      printf("%d %d\n", i, ap[i]);
    }
  }
  free(ap);
  return 0;
}
