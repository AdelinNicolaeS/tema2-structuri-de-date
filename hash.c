// COPYRIGHT 2020 STANCA ADELIN-NICOLAE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 101

struct info {
  char *key;
  int value;
};

struct Hashtable {
  struct info *buckets;
  int hmax; /* Nr. de bucket-uri. */
  unsigned int (*hash_function)(void *);
  int (*compare_function)(void *, void *);
};
int compare_function_strings(void *a, void *b) {
  char *str_a = (char *)a;
  char *str_b = (char *)b;

  return strcmp(str_a, str_b);
}
unsigned int hash_function_string(void *a) {
  /*
   * Credits: http://www.cse.yorku.ca/~oz/hash.html
   */
  unsigned char *puchar_a = (unsigned char *)a;
  int64_t hash = 5381;
  int c;

  while ((c = *puchar_a++))
    hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

  return hash;
}
// initializez valorile initiale ale Hashtable-ului
void init_ht(struct Hashtable *ht, int hmax,
             unsigned int (*hash_function)(void *),
             int (*compare_function)(void *, void *)) {
  ht->hmax = hmax;
  ht->buckets = calloc(hmax, sizeof(struct info));
  ht->hash_function = hash_function;
  ht->compare_function = compare_function;
}
void put(struct Hashtable *ht, char *key, int key_size_bytes) {
  char *key_copy = malloc(key_size_bytes);
  int ok = 0, i;
  memcpy(key_copy, key, key_size_bytes);
  int index = ht->hash_function(key_copy) % ht->hmax;
  // verific daca indexul e liber, continui in caz contrar
  for (i = index; ok == 0 && i < ht->hmax; i++) {
    if (ht->buckets[i].key == NULL) {
      // prima aparitie a stringului
      ht->buckets[i].value = 1;
      ht->buckets[i].key = malloc(key_size_bytes);
      snprintf(ht->buckets[i].key, key_size_bytes, "%s", key);
      ok = 1;
    } else if (ht->compare_function((void *)key,
                                    (void *)(ht->buckets[i].key)) == 0) {
      // doar maresc numarul de aparitii
      ht->buckets[i].value++;
      ok = 1;
    }
  }
  // index si vector ocupat, caut o pozitie libera de la 0
  for (i = 0; ok == 0 && i < index; i++) {
    if (ht->buckets[i].value == 0) {
      ht->buckets[i].value = 1;
      ht->buckets[i].key = malloc(key_size_bytes);
      snprintf(ht->buckets[i].key, key_size_bytes, "%s", key);
      ok = 1;
    } else if (ht->buckets[i].key == key) {
      ht->buckets[i].value++;
      ok = 1;
    }
  }
  free(key_copy);
}
int main() {
  struct Hashtable *ht;
  int i, hmax = 0;
  char s[MAX_CHAR];
  // aflarea numarului de string-uri din fisier
  while (fgets(s, MAX_CHAR, stdin) != 0) {
    hmax++;
  }
  ht = malloc(sizeof(struct Hashtable));
  fseek(stdin, 0, SEEK_SET);
  init_ht(ht, hmax, hash_function_string, compare_function_strings);
  // citire si adaugare
  while (fgets(s, MAX_CHAR, stdin) != 0) {
    s[strlen(s) - 1] = '\0';
    put(ht, s, strlen(s) + 1);
  }
  for (i = 0; i < ht->hmax; i++) {
    if (ht->buckets[i].value != 0) {
      printf("%s %d\n", ht->buckets[i].key, ht->buckets[i].value);
    }
    free(ht->buckets[i].key);
  }
  // eliberare memorie
  free(ht->buckets);
  free(ht);
  return 0;
}
