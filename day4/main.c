#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

static const size_t MASK_LENGTH = 4;

bool check_xmas_horz(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    (void)height;
    if (i > width-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'X' &&
           lines[j][i+1] == 'M' &&
           lines[j][i+2] == 'A' &&
           lines[j][i+3] == 'S';
}

bool check_xmas_vert(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    (void)width;
    if (j > height-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'X' &&
           lines[j+1][i] == 'M' &&
           lines[j+2][i] == 'A' &&
           lines[j+3][i] == 'S';
}

bool check_xmas_diag1(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    if (j > height-MASK_LENGTH || i > width-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'X' &&
           lines[j+1][i+1] == 'M' &&
           lines[j+2][i+2] == 'A' &&
           lines[j+3][i+3] == 'S';
}

bool check_xmas_diag2(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    (void)height;
    if (j < MASK_LENGTH-1 || i > width-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'X' &&
           lines[j-1][i+1] == 'M' &&
           lines[j-2][i+2] == 'A' &&
           lines[j-3][i+3] == 'S';
}

bool check_samx_horz(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    (void)height;
    if (i > width-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'S' &&
           lines[j][i+1] == 'A' &&
           lines[j][i+2] == 'M' &&
           lines[j][i+3] == 'X';
}

bool check_samx_vert(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    (void)width;
    if (j > height-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'S' &&
           lines[j+1][i] == 'A' &&
           lines[j+2][i] == 'M' &&
           lines[j+3][i] == 'X';
}

bool check_samx_diag1(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    if (j > height-MASK_LENGTH || i > width-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'S' &&
           lines[j+1][i+1] == 'A' &&
           lines[j+2][i+2] == 'M' &&
           lines[j+3][i+3] == 'X';
}

bool check_samx_diag2(char* const* lines, size_t width, size_t height, size_t i, size_t j)
{
    (void)height;
    if (j < MASK_LENGTH-1 || i > width-MASK_LENGTH)
    {
        return false;
    }

    return lines[j][i] == 'S' &&
           lines[j-1][i+1] == 'A' &&
           lines[j-2][i+2] == 'M' &&
           lines[j-3][i+3] == 'X';
}

int part1(char* const* lines, size_t width, size_t height)
{
    int count = 0;
    for(size_t j = 0; j<height; j++)
    {
        for (size_t i=0; i<width; i++)
        {
            count += check_xmas_horz(lines, width, height, i, j);
            count += check_xmas_vert(lines, width, height, i, j);
            count += check_xmas_diag1(lines, width, height, i, j);
            count += check_xmas_diag2(lines, width, height, i, j);
            count += check_samx_horz(lines, width, height, i, j);
            count += check_samx_vert(lines, width, height, i, j);
            count += check_samx_diag1(lines, width, height, i, j);
            count += check_samx_diag2(lines, width, height, i, j);
        }
    }
    return count;
}

bool check_x_mas(char* const* lines, size_t i, size_t j)
{
    if (lines[j][i] != 'A')
    {
        return false;
    }

    bool diag1A = lines[j-1][i-1] == 'M' &&
                  lines[j+1][i+1] == 'S';
    bool diag1B = lines[j-1][i-1] == 'S' &&
                  lines[j+1][i+1] == 'M';
    
    bool diag2A = lines[j-1][i+1] == 'M' &&
                  lines[j+1][i-1] == 'S';
    bool diag2B = lines[j-1][i+1] == 'S' &&
                  lines[j+1][i-1] == 'M';

    return (diag1A || diag1B) && (diag2A || diag2B);
}

int part2(char* const* lines, size_t width, size_t height)
{
    int count = 0;
    for(size_t j = 1; j<height-1; j++)
    {
        for (size_t i=1; i<width-1; i++)
        {
            count += check_x_mas(lines, i, j);
        }
    }
    return count;
}

int main()
{
    char* input = readInput("../day4/input.txt");
    size_t num_lines = 0;
    char** lines = split_lines_in_place(input, &num_lines);
    size_t line_length = strlen(lines[0]);

    int res1 = part1(lines, line_length, num_lines);
    int res2 = part2(lines, line_length, num_lines);

    printf("Part 1: %d\n", res1);
    printf("Part 2: %d\n", res2);
    free(lines);
    free(input);
    return 0;
}