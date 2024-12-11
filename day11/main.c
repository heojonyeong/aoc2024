#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "util.h"

struct KeyValue
{
    uint64_t key;
    uint64_t value;
};

struct Map 
{
    struct KeyValue** data;
    size_t size;
    size_t capacity;

    struct UInt64Array keys;
};

struct Map map_init()
{
    struct Map map;

    map.data = malloc(16*sizeof(struct KeyValue*));
    memset(map.data, 0, sizeof(struct KeyValue*)*16);

    map.size=0;
    map.capacity = 16;
    map.keys = UInt64_array_init();

    return map;
}

void map_free(struct Map* map)
{
    for (size_t i=0; i<map->capacity; i++)
    {
        if (map->data[i] != NULL)
        {
            free(map->data[i]);
        }
    }

    free(map->data);
    UInt64_array_free(&map->keys);
}

size_t hash(uint64_t n, uint64_t mod)
{
    return n % mod;
}

uint64_t* map_at(struct Map*, uint64_t);

void rehash(struct Map* map)
{
    struct Map tmp_map = map_init();
    uint64_t new_cap = map->capacity*2;
    struct KeyValue** new_data = malloc(new_cap*sizeof(struct KeyValue*));
    memset(new_data, 0, sizeof(struct KeyValue*)*new_cap);
    tmp_map.capacity = new_cap;
    tmp_map.data = new_data;

    for (size_t i=0; i<map->capacity; i++)
    {
        struct KeyValue* e = map->data[i];
        if (e != NULL)
        {   
            uint64_t* new_e = map_at(&tmp_map, e->key);
            *new_e = e->value;
        }
    }

    map_free(map);
    map->data = tmp_map.data;
    map->size = tmp_map.size;
    map->capacity = tmp_map.capacity;
    map->keys = tmp_map.keys;
}

uint64_t* map_at(struct Map* map, uint64_t key)
{
    struct KeyValue** it = map->data + hash(key, map->capacity);

    while(*it != NULL && (*it)->key != key)
    {
        it++;
        if (it >= map->data + map->capacity)
        {
            it -= map->capacity;
        }
    }

    if (*it == NULL)
    {
        if (map->size > map->capacity/2)
        {
            rehash(map);
            return map_at(map, key);
        }
        else
        {
            *it = malloc(sizeof(struct KeyValue));
            (*it)->key = key;
            (*it)->value = 0;

            map->size++;
            UInt64_array_add(&map->keys, key);

            return &(*it)->value;
        }

    }
    else
    {
        return &(*it)->value;
    }
}

uint64_t calc(struct Map* map, size_t iterations)
{
    if (iterations == 0)
    {
        uint64_t result = 0;
        for (size_t i=0; i<map->keys.size; i++)
        {
            uint64_t key = map->keys.data[i];
            result += *map_at(map, key);
        }

        map_free(map);
        return result;
    }
    
    struct Map next = map_init();
    for (size_t i=0; i<map->keys.size; i++)
    {
        uint64_t key = map->keys.data[i];
        uint64_t count = *map_at(map, key);
        uint64_t num_digits = log_int(key, 10) + 1;
        
        if (key == 0)
        {
            uint64_t* new_count = map_at(&next, 1);
            (*new_count) += count;
        }
        else if(num_digits%2 == 0)
        {
            uint64_t x = pow_int(10, num_digits/2);
            uint64_t left = key/x;
            uint64_t right = key%x;

            uint64_t* newc1 = map_at(&next, left);
            (*newc1) += count;
            uint64_t* newc2 = map_at(&next, right);
            (*newc2) += count;
        } 
        else
        {
            uint64_t* new_count = map_at(&next, key*2024);
            (*new_count) += count;
        }
    }

    map_free(map);
    return calc(&next, iterations-1);
}

int main()
{
    clock_t start = clock();

    char* input = readInput("../day11/input.txt");

    struct Map map1 = map_init();
    struct Map map2 = map_init();
    char* end_ptr = input;
    while(*end_ptr != 0)
    {
        uint64_t n = strtoull(end_ptr, &end_ptr, 10);
        uint64_t* e = map_at(&map1, n);
        (*e)++;
        uint64_t* e2 = map_at(&map2, n);
        (*e2)++;
    }
    free(input);

    uint64_t res1 = calc(&map1, 25);
    uint64_t res2 = calc(&map2, 75);

    printf("Part 1: %llu\n", res1);
    printf("Part 2: %llu\n", res2);

    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);

    return 0;
}