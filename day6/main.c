#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

void find_start(char* const* lines, size_t width, size_t height, size_t* x, size_t* y)
{
    for(size_t j = 0; j<height; j++)
    {
        for (size_t i = 0; i<width; i++)
        {
            if (lines[j][i] == '^')
            {
                *x = i;
                *y = j;

                return;
            }
        }
    }
}

enum Direction
{
    UP = 1,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 8
};

// additional flag for position that was already confirmed to cause loop
const char LOOP_MARKER = 16;

void step_dir(enum Direction dir, size_t* x, size_t* y)
{
    switch(dir)
    {
        case UP:
            (*y)--;
            break;
        case DOWN:
            (*y)++;
            break;
        case LEFT:
            (*x)--;
            break;
        case RIGHT:
            (*x)++;
            break;
    }
}

void step_back(enum Direction dir, size_t* x, size_t* y)
{
    switch(dir)
    {
        case UP:
            (*y)++;
            break;
        case DOWN:
            (*y)--;
            break;
        case LEFT:
            (*x)++;
            break;
        case RIGHT:
            (*x)--;
            break;
    }
}


void rotate(enum Direction* dir)
{
    switch(*dir)
    {
        case UP:
            *dir = RIGHT;
            break;
        case DOWN:
            *dir = LEFT;
            break;
        case LEFT:
            *dir = UP;
            break;
        case RIGHT:
            *dir = DOWN;
            break;
    }

}

int part1(char* const* lines, size_t width, size_t height)
{
    size_t cur_x = 0;
    size_t cur_y = 0;
    find_start(lines, width, height, &cur_x, &cur_y);

    enum Direction dir = UP;
    int result = 1;
    lines[cur_y][cur_x] = 'X';
    step_dir(dir, &cur_x, &cur_y);

    while (cur_x >= 0 && cur_x < width && cur_y >= 0 && cur_y < height)
    {
        if (lines[cur_y][cur_x] == '#')
        {
            // step backwards, switch dir and step again
            step_back(dir, &cur_x, &cur_y);
            rotate(&dir);
            step_dir(dir, &cur_x, &cur_y);
        }
        else 
        {
            if (lines[cur_y][cur_x] != 'X')
            {
                lines[cur_y][cur_x] = 'X';
                result++;
            }

            step_dir(dir, &cur_x, &cur_y);
        }
    }

    return result;
}

bool check_loop(char* field, size_t width, size_t height, size_t cur_x, size_t cur_y, enum Direction dir)
{
    while(cur_x >= 0 && cur_x < width && cur_y >= 0 && cur_y < height)
    {
        size_t next_x = cur_x;
        size_t next_y = cur_y;
        step_dir(dir, &next_x, &next_y);

        if (!(next_x >= 0 && next_x < width && next_y >= 0 && next_y < height))
        {
            return false;
        }

        if (field[next_y*width + next_x] == '#')
        {
            rotate(&dir);
        }
        else
        {
            if (field[cur_y*width + cur_x] & dir)
            {
                return true;
            }

            field[cur_y*width + cur_x] |= dir;
            step_dir(dir, &cur_x, &cur_y);
        }
    }

    return false;
}

int part2(char** lines, size_t width, size_t height)
{
    int result = 0;

    size_t cur_x = 0;
    size_t cur_y = 0;
    enum Direction dir = UP;
    find_start(lines, width, height, &cur_x, &cur_y);
    size_t start_x = cur_x;
    size_t start_y = cur_y;

    lines[start_y][start_x] = 0;

    char* field = malloc(sizeof(char)*width*height);
    for(size_t i = 0; i<height; i++)
    {
        memcpy(&field[i*width], lines[i], sizeof(char)*width);
    }

    while(cur_x >= 0 && cur_x < width && cur_y >= 0 && cur_y < height)
    {
        size_t next_x = cur_x;
        size_t next_y = cur_y;
        step_dir(dir, &next_x, &next_y);

        if (!(next_x >= 0 && next_x < width && next_y >= 0 && next_y < height))
        {
            break;
        }

        if (lines[next_y][next_x] == '#')
        {
            rotate(&dir);
        }
        else
        {
            if (!(lines[next_y][next_x] & LOOP_MARKER))
            {
                lines[next_y][next_x] |= LOOP_MARKER;

                if (next_y != start_y || next_x != start_x)
                {
                    // check for loop
                    char* field_copy = malloc(sizeof(char)*width*height);
                    memcpy(field_copy, field, sizeof(char)*width*height);

                    field_copy[next_y*width + next_x] = '#';

                    if (check_loop(field_copy, width, height, start_x, start_y, UP))
                    {
                        result++;
                    }

                    free(field_copy);
                }
            }

            lines[cur_y][cur_x] |= dir;
            step_dir(dir, &cur_x, &cur_y);
        }
    }

    free(field);
    return result;
}

int main()
{
    char* input = readInput("../day6/input.txt");
    size_t height = 0;
    char** lines = split_lines_in_place(input, &height);

    size_t width = strlen(lines[0]);

    // copy of lines because part 1 modifies
    char** lines2 = malloc(sizeof(char*)*height);
    for(size_t i = 0; i<height; i++)
    {
        char* l = malloc(sizeof(char)*width);
        memcpy(l, lines[i], width);
        lines2[i] = l;
        for(size_t j=0; j<width; j++)
        {
            if (lines2[i][j] == '.')
            {
                lines2[i][j] = 0;
            }
        }
    }

    int res1 = part1(lines, width, height);
    int res2 = part2(lines2, width, height);

    printf("Part 1: %d\n", res1);
    printf("Part 2: %d\n", res2);
    free(lines);
    free(input);
    for(size_t i = 0; i<height; i++)
    {
        free(lines2[i]);
    }
    free(lines2);
    return 0;
}