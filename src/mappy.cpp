#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

// gcc -shared -fPIC -o libMappy.so Mappy.c
// nm -D libMappy.so Pour check 
struct hashstu32; 

struct hashstu32 
{
    char* key;
    uint32_t value;
    struct hashstu32 *next;  // Liste chaînée pour gérer les collisions
};

struct HashMapstu32
{
    struct hashstu32** buckets;
    size_t nb_bucket;
}; 

struct SymbolesST
{
    int* Safe;
    int* Test;
};

void clean_Mapstu32 (struct HashMapstu32* M)
{
    for (int i = 0; i < M->nb_bucket; i++)
        {
            if (M->buckets[i] == NULL) {continue;}
            struct hashstu32* h1 = M->buckets[i];
            struct hashstu32* htemp = NULL;
            while (h1)
                {   
                    htemp = h1;
                    h1 = h1->next;
                    free(htemp->key);
                    free(htemp);
                }
        }
    free(M->buckets);
    free(M);    
}

uint32_t jenkins_hashu32(char *key) 
{
    uint32_t hash = 0;
    while (*key) {
        hash += *key++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

void insert_stu32(struct HashMapstu32 *map, char *key, int value) 
{
    unsigned int hash = jenkins_hashu32(key);
    unsigned int index = hash % map->nb_bucket;
    struct hashstu32 *new_item = malloc(sizeof(struct hashstu32));
    new_item->key = strdup(key);
    new_item->value = value;
    new_item->next = map->buckets[index];  // Insert at the beginning of the list
    map->buckets[index] = new_item;
}


struct hashstu32* find_stu32(struct HashMapstu32* map, char* key) 
{
    uint32_t hash = jenkins_hashu32(key);
    uint32_t index = hash % (uint32_t)(map->nb_bucket);
    struct hashstu32 *item = map->buckets[index];
    while (item) 
    {
        if (strcmp(item->key, key) == 0) 
        {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

struct HashMapstu32* create_map_stu32(size_t nb_bucket) 
{
    struct HashMapstu32 *map = malloc(sizeof(struct HashMapstu32));
    map->nb_bucket = nb_bucket;
    map->buckets = calloc(nb_bucket, sizeof(struct HashItem*));  // All buckets point to NULL initially
    return map;
}
