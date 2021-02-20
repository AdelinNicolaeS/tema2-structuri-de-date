// COPYRIGHT 2020 STANCA ADELIN-NICOLAE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// obtin reprezentarea pe 32 de biti a lui n
void bit_representation(int n, int *v) {
  int i, aux;
  for (i = 31; i >= 0; i--) {
    if (n & (1 << i)) {
      v[i] = 1;
    } else {
      v[i] = 0;
    }
  }
  for (i = 0; i < 16; i++) {
    aux = v[i];
    v[i] = v[31 - i];
    v[31 - i] = aux;
  }
}
unsigned int hash_function_int(void *a) {
  /*
   * Credits: https://stackoverflow.com/a/12996028/7883884
   */
  unsigned int uint_a = *((unsigned int *)a);

  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
  uint_a = (uint_a >> 16u) ^ uint_a;
  return uint_a;
}
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "argumente insuficiente\n");
  }
  FILE *fin = fopen(argv[1], "r");
  int j, n, lg = 11, m, *M, i, x, ok, val;
  unsigned int index;
  double z = 0, val_real, E, factor;
  m = 1 << lg;
  factor = (double)(0.7213 / (1 + 1.079 / m));
  M = calloc(m, sizeof(int));
  while (fscanf(fin, "%d", &n) > 0) {
    // pastrez reprezentarea lui n in vectorul rep
    int rep[32] = {0};
    x = 1, ok = 1;
    index = hash_function_int(&n);
    // j = numarul obtinut din primii 11 biti din index
    j = index >> (32 - lg);
    bit_representation(index, rep);
    // calculez lungimea secventei initiale de 0
    for (i = lg; ok == 1 && i < 32; i++) {
      if (rep[i] == 0) {
        x++;
      } else {
        ok = 0;
      }
    }
    if (M[j] < x) {
      M[j] = x;
    }
  }
  // finalizare calcule
  z = 0;
  for (j = 0; j < m; j++) {
    val = 1 << M[j];
    val_real = (double)val;
    z += 1 / val_real;
  }
  z = 1 / z;
  E = factor * z * m * m;
  printf("%d\n", (int)E);
  fclose(fin);
  free(M);
  return 0;
}
