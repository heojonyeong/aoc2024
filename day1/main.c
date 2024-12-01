#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "util.h"

uint64_t part1(uint64_t* arr1, uint64_t* arr2, size_t arrSize)
{
    uint64_t sum = 0;
    for(size_t i=0; i<arrSize; i++)
    {
        uint64_t diff;
        if (arr1[i] > arr2[i])
        {
            diff = arr1[i] - arr2[i];
        }
        else
        {
            diff = arr2[i] - arr1[i];        
        }

        sum += diff;
    }

    return sum;
}

uint64_t part2(uint64_t* arr1, uint64_t* arr2, size_t arrSize)
{
    uint64_t score = 0;

    size_t idx2 = 0;

    for (size_t i = 0; i<arrSize; i++)
    {
        while(idx2 < arrSize && arr2[idx2] > arr1[i])
        {
            idx2++;
        }

        if (idx2 == arrSize)
        {
            return score;
        }

        if (arr2[idx2] < arr1[i])
        {
            continue;
        }

        size_t j = idx2;
        while(j < arrSize && arr2[j] == arr1[i])
        {
            score += arr1[i];
            j++;
        }
    }

    return score;
}

int compare_ull(const void* a, const void* b)
{
    unsigned long long a_typed = *((unsigned long*)a);
    unsigned long long b_typed = *((unsigned long*)b);

    if (a_typed < b_typed)
    {
        return 1;
    }
    if (a_typed > b_typed)
    {
        return -1;
    }
    return 0;
}

int main(int argc, char** argv)
{
    clock_t start = clock();
    char* input = readInput("../day1/input.txt");
    const size_t BUF_SIZE = 10000;
    char* cur = input;
    size_t arrSize = 0;
    uint64_t* arr1 = malloc(sizeof(uint64_t)*BUF_SIZE);
    uint64_t* arr2 = malloc(sizeof(uint64_t)*BUF_SIZE);

    while(*cur != 0)
    {
        arr1[arrSize] = strtoull(cur, &cur, 10);
        arr2[arrSize] = strtoull(cur, &cur, 10);
        arrSize++;
    }

    free(input);

    qsort(arr1, arrSize, sizeof(uint64_t), &compare_ull);
    qsort(arr2, arrSize, sizeof(uint64_t), &compare_ull);

    uint64_t sum1 = part1(arr1, arr2, arrSize);
    uint64_t sum2 = part2(arr1, arr2, arrSize);

    printf("Array size is: %zu\n", arrSize);
    printf("Part1: %zu\n", sum1);
    printf("Part2: %zu\n", sum2);
    free(arr1);
    free(arr2);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);
    return 0;
}