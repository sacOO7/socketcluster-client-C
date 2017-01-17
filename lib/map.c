#include <string.h>
#include <stdlib.h>

#define NR_BUCKETS 1024

struct StrHashNode {
    char *key;
    void *value;
    struct StrHashNode *next;

};

struct StrHashTable {
    struct StrHashNode *buckets[NR_BUCKETS];
    void (*free_key)(char *);
    void (*free_value)(void*);
    unsigned int (*hash)(const char *key);
    int (*cmp)(const char *first,const char *second);
};

void *get(struct StrHashTable *table,const char *key)
{
    unsigned int bucket = table->hash(key)%NR_BUCKETS;
    struct StrHashNode *node;
    node = table->buckets[bucket];
    while(node) {
        if(table->cmp(key,node->key) == 0)
            return node->value;
        node = node->next;
    }
    return NULL;
}
int insert(struct StrHashTable *table,char *key,void *value)
{
    unsigned int bucket = table->hash(key)%NR_BUCKETS;
    struct StrHashNode **tmp;
    struct StrHashNode *node ;

    tmp = &table->buckets[bucket];
    while(*tmp) {
        if(table->cmp(key,(*tmp)->key) == 0)
            break;
        tmp = &(*tmp)->next;
    }
    if(*tmp) {
        if(table->free_key != NULL)
            table->free_key((*tmp)->key);
        if(table->free_value != NULL)
            table->free_value((*tmp)->value);
        node = *tmp;
    } else {
        node = malloc(sizeof *node);
        if(node == NULL)
            return -1;
        node->next = NULL;
        *tmp = node;
    }
    node->key = key;
    node->value = value;

    return 0;
}

unsigned int foo_strhash(const char *str)
{
    unsigned int hash = 0;
    for(; *str; str++)
        hash = 31*hash + *str;
    return hash;
}

#include <stdio.h>
int main(int argc,char *argv[])
{
    struct StrHashTable tbl = {{0},NULL,NULL,foo_strhash,strcmp};

    insert(&tbl,"Test","TestValue");
    insert(&tbl,"Test2","TestValue2");
    puts(get(&tbl,"Test"));
    insert(&tbl,"Test","TestValueReplaced");
    puts(get(&tbl,"Test"));

    return 0;
}