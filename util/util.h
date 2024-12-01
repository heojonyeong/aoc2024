#include <stdbool.h>

char* readInput(const char* path);

void bubble_sort(void* array, size_t bnum_elements, size_t element_size, bool(*less)(void*, void*), void(*swap)(void*, void*));

void bubble_sort_ull(unsigned long long* array, size_t size);