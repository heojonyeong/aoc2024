#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "time.h"

void build_map(struct Pos2DIntArray* map, char** field, size_t width, size_t height)
{
    for (size_t i=0; i<height; i++)
    {
        for(size_t j=0; j<width; j++)
        {
            if (field[i][j] != 0)
            {
                Pos2DInt pos;
                pos.x = j;
                pos.y = i;
                Pos2DInt_array_add(&map[(int)(field[i][j])], pos);
            }
        }
    }
}

int part1(struct Pos2DIntArray* map, char** const field, size_t width, size_t height)
{
    // make copy of field
    char* copy = malloc(sizeof(char)*width*height);
    for(size_t i = 0; i<height; i++)
    {
        memcpy(&copy[i*width], field[i], sizeof(char)*width);
    }

    int result = 0;

    int width_s = width;
    int height_s = height;

    for (unsigned char i = 0; i<128; i++)
    {
        struct Pos2DIntArray const* arr = &map[i];
        for (size_t j=0; j<arr->size; j++)
        {
            Pos2DInt const* pos1 = &arr->data[j];
            for (size_t k=j+1; k<arr->size; k++)
            {
                Pos2DInt const* pos2 = &arr->data[k];

                const int diff_x = pos1->x - pos2->x;
                const int diff_y = pos1->y - pos2->y;

                Pos2DInt antinode1;
                antinode1.x = pos2->x - diff_x;
                antinode1.y = pos2->y - diff_y;

                Pos2DInt antinode2;
                antinode2.x = pos1->x + diff_x;
                antinode2.y = pos1->y + diff_y;

                if (antinode1.x >= 0 && antinode1.x < width_s && antinode1.y >= 0 && antinode1.y < height_s)
                {
                    if (copy[antinode1.y*width + antinode1.x] != 1)
                    {
                        copy[antinode1.y*width + antinode1.x] = 1;
                        result++;
                    }
                }
                if (antinode2.x >= 0 && antinode2.x < width_s && antinode2.y >= 0 && antinode2.y < height_s)
                {
                    if (copy[antinode2.y*width + antinode2.x] != 1)
                    {
                        copy[antinode2.y*width + antinode2.x] = 1;
                        result++;
                    }
                }
            }
        }
    }
    free(copy);
    return result;
}

int part2(struct Pos2DIntArray* map, char** const field, size_t width, size_t height)
{
    // make copy of field
    char* copy = malloc(sizeof(char)*width*height);
    for(size_t i = 0; i<height; i++)
    {
        memcpy(&copy[i*width], field[i], sizeof(char)*width);
    }

    int result = 0;

    int width_s = width;
    int height_s = height;

    for (unsigned char i = 0; i<128; i++)
    {
        struct Pos2DIntArray const* arr = &map[i];
        for (size_t j=0; j<arr->size; j++)
        {
            Pos2DInt const* pos1 = &arr->data[j];
            for (size_t k=j+1; k<arr->size; k++)
            {
                Pos2DInt const* pos2 = &arr->data[k];

                const int diff_x = pos1->x - pos2->x;
                const int diff_y = pos1->y - pos2->y;

                Pos2DInt antinode1;
                antinode1.x = pos2->x;
                antinode1.y = pos2->y;

                Pos2DInt antinode2;
                antinode2.x = pos1->x;
                antinode2.y = pos1->y;

                while (antinode1.x >= 0 && antinode1.x < width_s && antinode1.y >= 0 && antinode1.y < height_s)
                {
                    if (copy[antinode1.y*width + antinode1.x] != 1)
                    {
                        copy[antinode1.y*width + antinode1.x] = 1;
                        result++;
                    }
                    antinode1.x -= diff_x;
                    antinode1.y -= diff_y;
                }
                while (antinode2.x >= 0 && antinode2.x < width_s && antinode2.y >= 0 && antinode2.y < height_s)
                {
                    if (copy[antinode2.y*width + antinode2.x] != 1)
                    {
                        copy[antinode2.y*width + antinode2.x] = 1;
                        result++;
                    }
                    antinode2.x += diff_x;
                    antinode2.y += diff_y;
                }
            }
        }
    }
    free(copy);
    return result;
}

int main()
{
    clock_t start = clock();
    char* input = readInput("../day8/input.txt");
    size_t height = 0;
    char** lines = split_lines_in_place(input, &height);
    size_t width = strlen(lines[0]);
    
    struct Pos2DIntArray map[128];
    for (size_t i=0; i<128; i++)
    {
        map[i] = Pos2DInt_array_init();
    }

    for (size_t i = 0; i < height; i++)
    {
        for (size_t j=0; j<width; j++)
        {
            if (lines[i][j] == '.')
            {
                lines[i][j] = 0;
            }
        }
    }

    build_map(map, lines, width, height);

    int res1 = part1(map, lines, width, height);
    int res2 = part2(map, lines, width, height);

    printf("Part 1: %d\n", res1);
    printf("Part 2: %d\n", res2);

    free(lines);
    free(input);

    for (size_t i=0; i<128; i++)
    {
        Pos2DInt_array_free(&map[i]);
    } 
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);
    return 0;
}