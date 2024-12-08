#include <stdio.h>
#include <stdlib.h>

#include "util.h"

long long* to_long_long(char*  string, size_t* size)
{
    char* start = string;
    *size = 0;
    size_t capacity = 16;
    long long* result = malloc(sizeof(long long)*capacity);
    while(*start != 0)
    {
        long long num = strtoll(start, &start, 10);
        if (*size == capacity)
        {
            capacity *= 2;
            result = realloc(result, sizeof(long long)*capacity);
        }

        result[*size] = num;
        (*size)++;
    }

    return result;
}

long long* make_diff_array(const long long* nums, size_t size)
{
    long long* result = malloc(sizeof(long long)*size-1);
    
    for(size_t i=0; i<size-1; i++)
    {
        result[i] = nums[i] - nums[i+1];
    }

    return result;
}

bool check_valid(const long long* diffs, size_t size)
{
    int sign = (diffs[0] < 0) ? -1 : 1;
    for(size_t i=0; i<size; i++)
    {
        if (diffs[i]*sign > 3 || diffs[i]*sign < 1)
        {
            return false;
        }
    }

    return true;
}

int part1(char** const lines, size_t num_lines)
{
    int result = 0;

    for(size_t i = 0; i<num_lines; i++)
    {
        size_t size = 0;
        long long* nums = to_long_long(lines[i], &size);
        long long* diff_array = make_diff_array(nums, size);
        free(nums);

        if (diff_array[0] != 0)
        {
            result += check_valid(diff_array, size-1);
        }

        free(diff_array);
    }

    return result;
}

bool check_valid_2(long long* nums, size_t size)
{
    {
        long long* diffs = make_diff_array(nums, size);
        if (check_valid(diffs, size-1))
        {
            free(diffs);
            return true;
        }
        free(diffs);
    }

    size_t drop_idx = 0;
    while(drop_idx < size)
    {
        long long* reduced_nums = malloc(sizeof(long long)*(size-1));
        size_t j = 0;
        for(size_t i=0; i<size; i++)
        {
            if (i != drop_idx)
            {
                reduced_nums[j] = nums[i];
                j++;
            }
        }

        long long* diffs = make_diff_array(reduced_nums, size-1);

        if (check_valid(diffs, size-2))
        {
            free(reduced_nums);
            free(diffs);
            return true;
        }

        free(diffs);
        free(reduced_nums);
        drop_idx++;
    }

    return false;
}

int part2(char** const lines, size_t num_lines)
{
    // brute force...
    int result = 0;

    for(size_t i = 0; i<num_lines; i++)
    {
        size_t size = 0;
        long long* nums = to_long_long(lines[i], &size);

        result += check_valid_2(nums, size);
        free(nums);
    }
   return result;
}

int main()
{
    char* input = readInput("../day2/sample.txt");

    size_t num_lines = 0;
    char** lines = split_lines_in_place(input, &num_lines);

    int result1 = part1(lines, num_lines);
    int result2 = part2(lines, num_lines);

    printf("Part1: %d\n", result1);
    printf("Part2: %d\n", result2);

    free(lines);
    free(input);
    return 0;
}