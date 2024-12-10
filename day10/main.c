#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "util.h"

void get_neighbors(int x, int y, Pos2DInt neighbors[4])
{
    neighbors[0].x = x+1;
    neighbors[0].y = y;
    neighbors[1].x = x-1;
    neighbors[1].y = y;
    neighbors[2].x = x;
    neighbors[2].y = y+1;
    neighbors[3].x = x;
    neighbors[3].y = y-1;
}

bool in_bounds(Pos2DInt pos, int width, int height)
{
    return (pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height);
}

uint64_t dfs(char* const* field, int width, int height, char* memory, int x, int y, bool part2)
{
    uint64_t result = 0;

    if (!part2)
    {
        if (memory[y*width + x] != 0)
        {
            return 0;
        }
    }

    // mark current position
    memory[y*width + x] = 1;

    if (field[y][x] == '9')
    {
        return 1;
    }

    Pos2DInt neighbors[4];
    get_neighbors(x, y, neighbors);

    for (size_t i=0; i<4; i++)
    {
        Pos2DInt n = neighbors[i];
        if (in_bounds(n, width, height))
        {
            if (field[y][x] + 1 == field[n.y][n.x])
            {
                result += dfs(field, width, height, memory, n.x, n.y, part2);
            }
        }
    }

    return result;
}

uint64_t calc(char* const* field, size_t width, size_t height, bool part2)
{
    uint64_t result = 0;
    char* memory = calloc(width*height, sizeof(char));
    for (size_t i=0; i<height; i++)
    {
        for (size_t j=0; j<width; j++)
        {
            if (field[i][j] == '0')
            {
                memset(memory, 0, width*height*sizeof(char));
                result += dfs(field, width, height, memory, j, i, part2);
            }
        }
    }
    free(memory);

    return result;
}

int main()
{
    clock_t start = clock();

    char* input = readInput("../day10/input.txt");
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