#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "util.h"

int64_t solve(int64_t ax, int64_t ay, int64_t bx, int64_t by, int64_t x, int64_t y)
{
    // ax*M + bx*N = x
    // ay*M + by*N = y
    // 
    // ax*M = x - bx*N
    // M = (x - bx*N)/ax
    //
    // ay*(x-bx*N)/ax + by*N = y
    // (ay*x)/ax - (ay*bx*N)/ax + by*N = y
    // - (ay*bx*N)/ax + by*N = y - (ay*x)/ax 
    // (ay*bx*N)/ax - by*N = (ay*x)/ax  - y
    // (ay*bx*N - ax*by*N)/ax = (ay*x)/ax  - y
    // (ay*bx*N - ax*by*N) = (ay*x) - y*ax
    // N*(ay*bx - ax*by) = (ay*x) - y*ax
    // N = ((ay*x) - y*ax)/(ay*bx - ax*by)
    // N = ((ax*y - ay*x)/(ax*by - ay*bx))

    if (ax == 0)
    {
        return 0;
    }

    if (ax*by - ay*bx == 0)
    {
        return 0;
    }

    if ((ax*y - ay*x)%(ax*by - ay*bx) != 0)
    {
        return 0;
    }
    
    int64_t N = (ax*y - ay*x)/(ax*by - ay*bx);

    if ((x-bx*N)%ax != 0)
    {
        return 0;
    }

    int64_t M = (x-bx*N)/ax;

    return 3*M + N;
}

int64_t calc(char* input, bool part2)
{
    int64_t result = 0;

    char* endptr = input;
    while (*endptr != 0)
    {
        while(*endptr != '+')
        {
            endptr++;
        }
        endptr++;

        int64_t ax = strtoll(endptr, &endptr, 10);

        while(*endptr != '+')
        {
            endptr++;
        }
        endptr++;

        int64_t ay = strtoll(endptr, &endptr, 10);
        
        while(*endptr != '+')
        {
            endptr++;
        }
        endptr++;

        int64_t bx = strtoll(endptr, &endptr, 10);

        while(*endptr != '+')
        {
            endptr++;
        }
        endptr++;

        int64_t by = strtoll(endptr, &endptr, 10);

        while(*endptr != '=')
        {
            endptr++;
        }
        endptr++;

        int64_t x = strtoll(endptr, &endptr, 10);

        while(*endptr != '=')
        {
            endptr++;
        }
        endptr++;

        int64_t y = strtoll(endptr, &endptr, 10);

        if (part2)
        {
            result += solve(ax, ay, bx, by, x+10000000000000, y+10000000000000);
        }
        else
        {
            result += solve(ax, ay, bx, by, x, y);
        }
    }

    return result;
}

int main()
{
    clock_t start = clock();

    char* input = readInput("../day13/input.txt");

    int64_t res1 = calc(input, false);
    int64_t res2 = calc(input, true);

    printf("Part 1: %lld\n", res1);
    printf("Part 2: %lld\n", res2);

    free(input);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);

    return 0;
}