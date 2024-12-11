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

char** split_lines_in_place(char* input, size_t* size)
{
    size_t capacity = 16;
    char** result = malloc(sizeof(char*)*capacity);

    *size = 0;
    char* cur = input;
    char* start = input;

    while(*cur != 0)
    {
        if (*cur == '\n')
        {
            if (*size >= capacity)
            {
                capacity *= 2;
                result = realloc(result, sizeof(char*)*capacity);
            }

            *cur = 0;

            result[*size] = start;
            (*size)++;
            start = cur+1;
        }

        cur++;
    }

    // add final line
    if (*size >= capacity)
    {
        capacity *= 2;
        result = realloc(result, sizeof(char*)*capacity);
    }
    result[*size] = start;
    (*size)++;

    return result;
}

int llsign(long long a)
{
    if (a > 0)
    {
        return 1;
    }
    if (a < 0)
    {
        return -1;
    }
    return 0;
}

static inline bool Int_compare(int const* a, int const* b)
{
    return *a == *b;
}

static inline bool UInt64_compare(uint64_t const* a, uint64_t const* b)
{
    return *a == *b;
}

static inline bool Pos2DInt_compare(Pos2DInt const* a, Pos2DInt const* b)
{
    return (a->x == b->x) &&
           (a->y == b->y);
}

DEFINE_ARRAY(Int, int)
DEFINE_ARRAY(UInt64, uint64_t)
DEFINE_ARRAY(Pos2DInt, Pos2DInt)

uint64_t log_int(uint64_t n, uint64_t base)
{
    if (n < base)
    {
        return 0;
    }

    // not efficient but whatever
    uint64_t exp = 1;
    uint64_t threshold = base;

    while(n >= threshold)
    {
        exp++;
        threshold *= base;
    }

    return exp - 1;
}

uint64_t pow_int(uint64_t n, uint64_t exponent)
{
    if (exponent == 0)
    {
        return 1;
    }

    uint64_t result = 1;
    for(size_t i=0; i<exponent; i++)
    {
        result *= n;
    }

    return result;
}