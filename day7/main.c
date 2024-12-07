#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "util.h"

struct Sample 
{
    uint64_t target;
    struct UInt64Array nums;
};

struct Sample* build_struct(char* const* lines, size_t num_lines)
{
    struct Sample* result = malloc(sizeof(struct Sample)*num_lines);
    for (size_t i=0; i<num_lines; i++)
    {
        struct Sample s;
        s.nums = uint64_array_init();

        char* end_ptr = lines[i];
        s.target = strtoull(lines[i], &end_ptr, 10);

        end_ptr++;

        while(*end_ptr != 0)
        {
            uint64_array_add(&s.nums, strtoull(end_ptr, &end_ptr, 10));
        }

        result[i] = s;
    }

    return result;
}

bool check_sample(uint64_t target, uint64_t cur, const uint64_t* rest, size_t num_rest)
{
    if (cur > target)
    {
        return false;
    }

    if (num_rest == 0)
    {
        return target == cur;
    }

    const uint64_t* remaining = rest + 1;
    return check_sample(target, cur*(*rest), remaining, num_rest-1) ||
           check_sample(target, cur+(*rest), remaining, num_rest-1);
}

uint64_t concat(uint64_t a, uint64_t b)
{
    if (b==0)
    {
        return a*10;
    }

    size_t mult = 1;
    while(b >= mult)
    {
        a *= 10;
        mult *= 10;
    }

    return a + b;
}

bool check_sample_2(uint64_t target, uint64_t cur, const uint64_t* rest, size_t num_rest)
{
    if (cur > target)
    {
        return false;
    }

    if (num_rest == 0)
    {
        return target == cur;
    }

    const uint64_t* remaining = rest + 1;
    uint64_t concatenated = concat(cur, *rest);
    return check_sample_2(target, cur*(*rest), remaining, num_rest-1) ||
           check_sample_2(target, concatenated, remaining, num_rest-1) ||
           check_sample_2(target, cur+(*rest), remaining, num_rest-1);
}

uint64_t part1(struct Sample const* samples, size_t num_samples)
{
    uint64_t result = 0;

    for (size_t i=0; i<num_samples; i++)
    {
        if (check_sample(samples[i].target, samples[i].nums.data[0], samples[i].nums.data + 1, samples[i].nums.size - 1))
        {
            result += samples[i].target;
        }
    }

    return result;
}

uint64_t part2(struct Sample const* samples, size_t num_samples)
{
    uint64_t result = 0;

    for (size_t i=0; i<num_samples; i++)
    {
        if (check_sample_2(samples[i].target, samples[i].nums.data[0], samples[i].nums.data + 1, samples[i].nums.size - 1))
        {
            result += samples[i].target;
        }
    }

    return result;
}

int main()
{
    clock_t start = clock();
    char* input = readInput("../day7/input.txt");
    size_t num_lines = 0;
    char** lines = split_lines_in_place(input, &num_lines);
    
    struct Sample* samples = build_struct(lines, num_lines);
    free(lines);
    free(input);

    uint64_t res1 = part1(samples, num_lines);
    uint64_t res2 = part2(samples, num_lines);

    printf("Part 1: %llu\n", res1);
    printf("Part 2: %llu\n", res2);

    for (size_t i=0; i<num_lines; i++)
    {
        uint64_array_free(&samples[i].nums);
    }
    free(samples);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);

    return 0;
}