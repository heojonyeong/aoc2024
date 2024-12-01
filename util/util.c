#include <stdio.h>
#include <stdlib.h>

#include "util.h"

char* readInput(const char* path)
{
    FILE* f = fopen(path, "r");
    if (!f)
    {
        printf("Could not open file\n");
        return NULL;
    }

    const size_t BUF_SIZE = 100000;
    char* buf = malloc(sizeof(char)*BUF_SIZE);
    const size_t readBytes = fread(buf, sizeof(char), BUF_SIZE, f);

    if (!feof(f) || readBytes >= BUF_SIZE)
    {
        printf("Buffer was too small for input.\n");
        free(buf);
        fclose(f);
        return NULL;
    }

    if (ferror(f))
    {
        printf("Error when reading file.\n");
        free(buf);
        fclose(f);
        return NULL;
    }

    fclose(f);

    // turn into proper c string
    buf[readBytes] = 0;
    return buf;
}

void bubble_sort(void* array, size_t num_elements, size_t element_size, bool(*less)(void*, void*), void(*swap)(void*, void*))
{
    size_t end = num_elements;

    while(end > 1)
    {
        for (size_t i=0; i<end-1; i++)
        {
            if (less(array + i*element_size, array + (i + 1)*element_size))
            {
                swap(array + i*element_size, array + (i + 1)*element_size);
            }
        }
        end--;
    }
}

bool less_ull(void* a, void* b)
{
    return *((unsigned long long*)a) < *((unsigned long long*)b);
}

void swap_ull(void* a, void* b)
{
    const unsigned long long tmp = *((unsigned long long*)a);
    unsigned long long* a_typed = a;
    *a_typed = *((unsigned long long*)b);
    unsigned long long* b_typed = b;
    *b_typed = tmp; 
}

void bubble_sort_ull(unsigned long long* array, size_t size)
{
    bubble_sort(array, size, sizeof(unsigned long long), &less_ull, &swap_ull);
}