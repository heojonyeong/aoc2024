#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "util.h"

const char MARKER = 0b10000000;

struct Fence 
{
    uint64_t area;
    uint64_t circ;
    uint64_t sides;
};

bool matches(char a, char c)
{
    return (~MARKER & a) == c;
}

uint64_t count_corners(char* field, size_t width, char color, size_t x, size_t y)
{
    uint64_t result = 0;
    bool tl = matches(field[(y-1)*width + x - 1], color);
    bool t = matches(field[(y-1)*width + x], color);
    bool tr = matches(field[(y-1)*width + x + 1], color);
    bool l = matches(field[y*width + x - 1], color);
    bool r = matches(field[y*width + x + 1], color);
    bool bl = matches(field[(y+1)*width + x - 1], color);
    bool b = matches(field[(y+1)*width + x], color);
    bool br = matches(field[(y+1)*width + x + 1], color);

    if (tl)
    {
        if (!(t && l))
        {
            result +=1;
        }
    }
    else
    {
        if (t && l)
        {
            result -= 1;
        }
        else if (!(t || l))
        {
            result += 1;
        }
    }
    if (tr)
    {
        if (!(t && r))
        {
            result +=1;
        }
    }
    else
    {
        if (t && r)
        {
            result -= 1;
        }
        else if (!(t || r))
        {
            result += 1;
        }
    }
    if (bl)
    {
        if (!(b && l))
        {
            result +=1;
        }
    }
    else
    {
        if (b && l)
        {
            result -= 1;
        }
        else if (!(b || l))
        {
            result += 1;
        }
    }
    if (br)
    {
        if (!(b && r))
        {
            result +=1;
        }
    }
    else
    {
        if (b && r)
        {
            result -= 1;
        }
        else if (!(b || r))
        {
            result += 1;
        }
    }

    return result;
}

uint64_t circ(char* field, size_t width, char color, size_t x, size_t y)
{
    uint64_t res = 0;

    if (!matches(field[y*width + x - 1], color))
    {
        res++;
    }
    if (!matches(field[y*width + x - 1], color))
    {
        res++;
    }
    if (!matches(field[(y-1)*width + x], color))
    {
        res++;
    }
    if (!matches(field[(y+1)*width + x], color))
    {
        res++;
    }

    return res;
}

void fill_recurse(char* field, size_t width, char color, size_t x, size_t y, struct Fence* fence)
{
    if (field[y*width + x] & MARKER)
    {
        return;
    }

    if (!matches(field[y*width + x], color))
    {
        return;
    }

    field[y*width + x] |= MARKER;

    uint64_t corners = count_corners(field, width, color, x, y);
    fence->sides += corners;

    fence->area++;
    fence->circ += circ(field, width, color, x, y);

    fill_recurse(field, width, color, x, y+1, fence);
    fill_recurse(field, width, color, x+1, y, fence);
    fill_recurse(field, width, color, x-1, y, fence);
    fill_recurse(field, width, color, x, y-1, fence);
}

struct Fence fill(char* field, size_t width, char color, size_t x, size_t y)
{
    struct Fence fence = {0,0,0};
    fill_recurse(field, width, color, x, y, &fence);
    return fence;
}

uint64_t calc(char* const* field, size_t width, size_t height, bool part2)
{
    char* f = calloc((width+2)*(height+2), sizeof(char));
    for (size_t i=0; i<height; i++)
    {
        memcpy(&f[(i+1)*(width+2)+1], field[i], sizeof(char)*width);
    }
    uint64_t result = 0;
    
    for(size_t i=1; i<height+1; i++)
    {
        for (size_t j=1; j<width+1; j++)
        {
            if (! (f[i*(width+2) + j] & MARKER))
            {   
                struct Fence fence = fill(f, width+2, f[i*(width+2) + j], j, i);
                if (part2)
                {
                    result += fence.area*fence.sides;
                }
                else
                {
                    result += fence.area*fence.circ;
                }
            }
        }
    }

    free(f);
    return result;
}

int main()
{
    clock_t start = clock();

    char* input = readInput("../day12/input.txt");
    size_t height;
    char** field = split_lines_in_place(input, &height); 
    size_t width = strlen(field[0]);

    uint64_t res1 = calc(field, width, height, false);
    uint64_t res2 = calc(field, width, height, true);

    printf("Part 1: %llu\n", res1);
    printf("Part 2: %llu\n", res2);

    free(field);
    free(input);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);

    return 0;
}