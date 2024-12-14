#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "util.h"

struct Robot
{
    int64_t x;
    int64_t y;
    int64_t vx;
    int64_t vy;
};

struct Robot* parse_robots(char** lines, size_t num_lines)
{
    struct Robot* robots = malloc(num_lines*sizeof(struct Robot));
    struct Robot* cur = robots;
    
    for (size_t i = 0; i<num_lines; cur++, i++)
    {
        char* end_ptr = lines[i];
        while (*end_ptr != '=')
            end_ptr++;
        end_ptr++;

        cur->x = strtoll(end_ptr, &end_ptr, 10);
        end_ptr++;
        cur->y = strtoll(end_ptr, &end_ptr, 10);

        while (*end_ptr != '=')
            end_ptr++;
        end_ptr++;
        
        cur->vx = strtoll(end_ptr, &end_ptr, 10);
        end_ptr++;
        cur->vy = strtoll(end_ptr, &end_ptr, 10);
    }

    return robots;
}

uint64_t part1(char** lines, size_t num_lines)
{
    struct Robot* robots = parse_robots(lines, num_lines);

    uint64_t q1 = 0;
    uint64_t q2 = 0;
    uint64_t q3 = 0;
    uint64_t q4 = 0;

    for(size_t i = 0; i<num_lines; i++)
    {
        struct Robot r = robots[i];

        int64_t final_x = ((r.x + 100*r.vx)%101 + 101)%101;
        int64_t final_y = ((r.y + 100*r.vy)%103 + 103)%103;

        if (final_x < 50 && final_y < 51)
        {
            q1++;
        }
        if (final_x > 50 && final_y < 51)
        {
            q2++;
        }
        if (final_x < 50 && final_y > 51)
        {
            q3++;
        }
        if (final_x > 50 && final_y > 51)
        {
            q4++;
        }
    }
    
    printf("q1: %llu, q2: %llu, q3: %llu, q4: %llu\n", q1, q2, q3, q4);

    free(robots);

    return q1*q2*q3*q4;
}

void draw_field(struct Robot* robots, size_t num)
{
    const size_t width = 101;
    const size_t height = 103;

    char* picture = malloc(width*height*sizeof(char));
    memset(picture, '.', sizeof(char)*width*height);

    for (size_t i=0; i<num; i++)
    {
        struct Robot r = robots[i];
        picture[r.y*width + r.x] = 'X';
    }

    for (size_t i=0; i<height; i++)
    {
        for(size_t j=0; j<width; j++)
        {
            printf("%c", picture[i*width + j]);
        }
        printf("\n");
    }
    printf("\n");

    free(picture);
}

uint64_t num_total_neighbors(struct Robot* robots, size_t num)
{
    uint64_t result = 0;
    for(size_t i = 0; i<num; i++)
    {
        for(size_t j=i; j<num; j++)
        {
            if (llabs(robots[i].x - robots[j].x) == 1 && llabs(robots[i].y - robots[j].y == 1))
            {
                result++;
            }
        }
    }

    return result;
}

void part2(char** lines, size_t num_lines)
{
    struct Robot* robots = parse_robots(lines, num_lines);

    for(size_t j=0; j<100000; j++)
    {
        uint64_t num_neighbors = num_total_neighbors(robots, num_lines);
        //printf("neighbors: %llu\n", num_neighbors);
        if (num_neighbors > 100)
        {
            printf("neighbors: %llu\n", num_neighbors);
            printf("Seconds: %llu\n", j);
            draw_field(robots, num_lines);
        }

        for(size_t i = 0; i<num_lines; i++)
        {
            robots[i].x += robots[i].vx;
            robots[i].x = ((robots[i].x % 101) + 101) % 101;
            robots[i].y += robots[i].vy;
            robots[i].y = ((robots[i].y % 103) + 103) % 103;
        }
    }

    free(robots);
}


int main()
{
    clock_t start = clock();

    char* input = readInput("../day14/input.txt");
    size_t num_lines;
    char** lines = split_lines_in_place(input, &num_lines);

    uint64_t res1 = part1(lines, num_lines);
    // print and pray
    part2(lines, num_lines);

    printf("Part 1: %llu\n", res1);
    //printf("Part 2: %lld\n", res2);

    free(input);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);

    return 0;
}