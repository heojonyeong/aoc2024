#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define DECLARE_ARRAY(NAME, TYPE) \
struct NAME##Array \
{ \
    TYPE * data; \
    size_t size; \
    size_t capacity; \
}; \
\
struct NAME##Array NAME##_array_init(); \
void NAME##_array_add(struct NAME##Array* arr, TYPE e); \
void NAME##_array_free(struct NAME##Array* arr); \
bool NAME##_array_contains(struct NAME##Array const* arr, TYPE e);\
struct NAME##Array NAME##_array_copy(struct NAME##Array const* arr);

#define DEFINE_ARRAY(NAME, TYPE) \
\
struct NAME##Array NAME##_array_init()\
{\
    struct NAME##Array a;\
    a.capacity = 1;\
    a.size = 0;\
    a.data = malloc(sizeof(TYPE));\
\
    return a;\
}\
\
void NAME##_array_add(struct NAME##Array* arr, TYPE n)\
{\
    if (arr->size >= arr->capacity)\
    {\
        arr->capacity *= 2;\
        arr->data = realloc(arr->data, sizeof(TYPE)*arr->capacity);\
    }\
\
    arr->data[arr->size] = n;\
    arr->size++;\
}\
\
void NAME##_array_free(struct NAME##Array* arr)\
{\
    free(arr->data);\
}\
\
bool NAME##_array_contains(struct NAME##Array const* arr, TYPE n)\
{\
    for (size_t i=0; i<arr->size; i++)\
    {\
        if (NAME##_compare(&(arr->data[i]), &n))\
        {\
            return true;\
        }\
    }\
\
    return false;\
}\
\
struct NAME##Array NAME##_array_copy(struct NAME##Array const* arr)\
{\
    struct NAME##Array cpy;\
    cpy.size = arr->size;\
    cpy.capacity = arr->capacity;\
    cpy.data = malloc(sizeof(*arr->data)*arr->capacity);\
    memcpy(cpy.data, arr->data, sizeof(*arr->data)*arr->size);\
    return cpy;\
}

typedef struct
{
    int x;
    int y;
} Pos2DInt;

char* readInput(const char* path);

void bubble_sort(void* array, size_t bnum_elements, size_t element_size, bool(*less)(void*, void*), void(*swap)(void*, void*));

void bubble_sort_ull(unsigned long long* array, size_t size);

/**
* @brief Split an input string into lines. This does modify the input string.
         The returned array is only valid as long as the original string is valid.
*/
char** split_lines_in_place(char* input, size_t* size);

int llsign(long long a);

DECLARE_ARRAY(Int, int)
DECLARE_ARRAY(UInt64, uint64_t)
DECLARE_ARRAY(Pos2DInt, Pos2DInt)

uint64_t log_int(uint64_t n, uint64_t base);
uint64_t pow_int(uint64_t n, uint64_t exponent);