#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

struct IntArray* to_numbers(char** input, size_t size)
{
    struct IntArray* result = malloc(sizeof(struct IntArray)*size);

    for(size_t i = 0; i<size; i++)
    {
        struct IntArray nums = int_array_init();
        char* end_ptr = input[i];
        while(*end_ptr != 0)
        {
            int n = strtol(end_ptr, &end_ptr, 10);
            int_array_add(&nums, n);
            if (*end_ptr == ',')
            {
                end_ptr++;
            }
        }

        result[i] = nums;
    }

    return result;
}

void build_rules(struct IntArray* rules, char** rule_lines, size_t num_rules)
{
    for (size_t i=0; i<num_rules; i++)
    {
        char* end_ptr;
        int a = strtol(rule_lines[i], &end_ptr, 10);
        end_ptr++;
        int b = strtol(end_ptr, NULL, 10);

        int_array_add(&rules[a], b);
    }
}

bool checks_rules(struct IntArray const* nums, struct IntArray const* rules)
{
    bool valid = true;

    /*
    by induction it can be proven that for a sequence a_0...a_n
    it is succifient to show that a_i is allowed to come before a_(i+1)
    for all i in [0, n-1]
    */
    for(size_t i = 0; i<nums->size-1; i++)
    {
        const int num_to_check = nums->data[i];

        valid = valid && int_array_contains(&rules[num_to_check], nums->data[i+1]);
    }

    return valid;
}

int get_middle(struct IntArray const* arr)
{
    return arr->data[(arr->size/2)];
}

int part1(struct IntArray const* input, struct IntArray const* rules, size_t num_inputs)
{
    int result = 0;
    for (size_t i = 0; i<num_inputs; i++)
    {
        if (checks_rules(&input[i], rules))
        {
            result += get_middle(&input[i]);
        }
    }

    return result;
}

int compare(void* context, void const* a, void const* b)
{
    struct IntArray* rules = context;
    int na = *((int*)a);
    int nb = *((int*)b);

    if (int_array_contains(&rules[na], nb))
    {
        return -1;
    }
    if (int_array_contains(&rules[nb], na))
    {
        return 1;
    }

    return 0;
}

int part2(struct IntArray const* input, struct IntArray* rules, size_t num_inputs)
{
    int result = 0;
    for (size_t i = 0; i<num_inputs; i++)
    {
        if (!checks_rules(&input[i], rules))
        {
            int* tmp_array = malloc(sizeof(int)*input[i].size);
            memcpy(tmp_array, input[i].data, sizeof(int)*input[i].size);
            qsort_s(tmp_array, input[i].size, sizeof(int), compare, rules);

            struct IntArray arr;
            arr.data = tmp_array;
            arr.size = input[i].size;
            result += get_middle(&arr);
            free(tmp_array);
        }
    }

    return result;
}

int main(int argc, char** argv)
{
    char* input = readInput("../day5/input.txt");
    size_t num_lines = 0;
    char** lines = split_lines_in_place(input, &num_lines);
    char** rule_lines = lines;
    char** input_lines;

    size_t i = 0;
    while (lines[i][0] != 0)
    {
        i++;
    }

    size_t num_rules = i;
    input_lines = lines+i+1;
    size_t num_inputs = num_lines - 1 - i;

    struct IntArray* input_number_lists = to_numbers(input_lines, num_inputs);
    const size_t MAX_PAGE_NUMBER = 100;
    struct IntArray* rules = malloc(sizeof(struct IntArray)*MAX_PAGE_NUMBER);
    for (size_t i = 0; i<MAX_PAGE_NUMBER; i++)
    {
        struct IntArray r = int_array_init();
        rules[i] = r;
    }

    build_rules(rules, rule_lines, num_rules);

    int res1 = part1(input_number_lists, rules, num_inputs);
    int res2 = part2(input_number_lists, rules, num_inputs);

    printf("Part 1: %d\n", res1);
    printf("Part 2: %d\n", res2);

    for (size_t i=0; i<num_inputs; i++)
    {
        int_array_free(&input_number_lists[i]);
    }
    free(input_number_lists);

    for(size_t i=0; i<MAX_PAGE_NUMBER; i++)
    {
        int_array_free(&rules[i]);
    }
    free(rules);

    free(lines);
    free(input);
    return 0;
}