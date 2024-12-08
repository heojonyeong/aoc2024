#include <stdbool.h>
#include <stdint.h>

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
bool NAME##_array_contains(struct NAME##Array const* arr, TYPE e);

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