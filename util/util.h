#include <stdbool.h>

char* readInput(const char* path);

void bubble_sort(void* array, size_t bnum_elements, size_t element_size, bool(*less)(void*, void*), void(*swap)(void*, void*));

void bubble_sort_ull(unsigned long long* array, size_t size);

/**
* @brief Split an input string into lines. This does modify the input string.
         The returned array is only valid as long as the original string is valid.
*/
char** split_lines_in_place(char* input, size_t* size);

int llsign(long long a);

struct IntArray
{
    int* data;
    size_t size;
    size_t capacity;
};

struct IntArray int_array_init();
void int_array_add(struct IntArray* arr, int n);
void int_array_free(struct IntArray* arr);
bool int_array_contains(struct IntArray const* arr, int n);