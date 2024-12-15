#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "util.h"

struct Data
{
    char* field;
    size_t width;
    size_t height;
    char* moves;
    size_t num_moves;
};

struct Data parse(char* input)
{
    size_t num_lines;
    char** lines = split_lines_in_place(input, &num_lines);

    char** move_lines = lines;
    while(move_lines[0][0] != 0)
    {
        move_lines++;
    }
    move_lines++;

    size_t field_width = strlen(lines[0]);
    size_t field_height = move_lines - lines - 1;

    char* field = malloc(sizeof(char)*field_height*field_height);

    for(size_t i=0; i<field_height; i++)
    {
        memcpy(&field[i*field_width], lines[i], sizeof(char)*field_width);
    }

    size_t num_moves = 0;
    char* moves = malloc(sizeof(char));

    for(size_t i=0; i<num_lines - field_height - 1; i++)
    {
        size_t line_length = strlen(move_lines[i]);
        num_moves += line_length;
        moves = realloc(moves, sizeof(char)*num_moves);
        memcpy(moves + num_moves - line_length, move_lines[i], sizeof(char)*line_length);
    }

    free(lines);
    return (struct Data) { .field = field, .height = field_height, .width = field_width, .moves = moves, .num_moves = num_moves };
}

void find_start(struct Data data, size_t xy[2])
{
    const char* cur = data.field;
    for(size_t i=0; i<data.height; i++)
    {
        for(size_t j=0; j<data.width; j++)
        {
            if (*cur == '@')
            {
                xy[0] = j;
                xy[1] = i;
                return;
            }
            cur++;
        }
    }
}

bool find_gap(struct Data data, char dir, size_t pos[2])
{
    size_t dx = 0;
    size_t dy = 0;

    switch(dir)
    {
        case 'v':
            dy++;
            break;
        case '^':
            dy--;
            break;
        case '<':
            dx--;
            break;
        case '>':
            dx++;
            break;
    }

    while(data.field[pos[1]*data.width + pos[0]] != '.')
    {
        if(data.field[pos[1]*data.width + pos[0]] == '#')
        {
            return false;
        }
        pos[0] += dx;
        pos[1] += dy;
    }

    return true;
}

uint64_t part1(struct Data data)
{
    size_t pos[2];
    find_start(data, pos);

    data.field[pos[1]*data.width + pos[0]] = '.';

    for(const char* move=data.moves; move<data.moves+data.num_moves; move++)
    {
        switch(*move)
        {
            case 'v':
            {
                char* c = &data.field[(pos[1]+1)*data.width + pos[0]];
                if (*c == '.')
                {
                    pos[1] +=1;
                }
                else if (*c == 'O')
                {
                    size_t gap[2] = { pos[0], pos[1]+1 };
                    if (find_gap(data, 'v', gap))
                    {
                        data.field[gap[1]*data.width + gap[0]] = 'O';
                        *c = '.';
                        pos[1] +=1;
                    }
                }
                break;
            }
            case '>':
            {
                char* c = &data.field[pos[1]*data.width + pos[0] + 1];
                if (*c == '.')
                {
                    pos[0] +=1;
                }
                else if (*c == 'O')
                {
                    size_t gap[2] = { pos[0] + 1, pos[1] };
                    if (find_gap(data, '>', gap))
                    {
                        data.field[gap[1]*data.width + gap[0]] = 'O';
                        *c = '.';
                        pos[0] +=1;
                    }
                }
                break;
            }
            case '<':
            {
                char* c = &data.field[pos[1]*data.width + pos[0] - 1];
                if (*c == '.')
                {
                    pos[0] -=1;
                }
                else if (*c == 'O')
                {
                    size_t gap[2] = { pos[0]-1, pos[1] };
                    if (find_gap(data, '<', gap))
                    {
                        data.field[gap[1]*data.width + gap[0]] = 'O';
                        *c = '.';
                        pos[0] -=1;
                    }
                }
                break;
            }
            case '^':
            {
                char* c = &data.field[(pos[1]-1)*data.width + pos[0]];
                if (*c == '.')
                {
                    pos[1] -=1;
                }
                else if (*c == 'O')
                {
                    size_t gap[2] = { pos[0], pos[1]-1 };
                    if (find_gap(data, '^', gap))
                    {
                        data.field[gap[1]*data.width + gap[0]] = 'O';
                        *c = '.';
                        pos[1] -=1;
                    }
                }
                break;
            }
        }

        /*for (size_t i=0; i<data.height; i++)
        {
            for(size_t j=0; j<data.width; j++)
            {
                printf("%c", data.field[i*data.width + j]);
            }
            printf("\n");
        }
        printf("\n");*/

    }

    uint64_t result = 0;
    for (size_t i=0; i<data.height; i++)
    {
        for(size_t j=0; j<data.width; j++)
        {
            if (data.field[i*data.width + j] == 'O')
            {
                result += i*100+j;
            }            
        }
    }

    return result;
}

void expand_field(struct Data* data)
{
    char* new_field = malloc(sizeof(char)*data->width*data->height*2);

    for (size_t i=0; i<data->height; i++)
    {
        for (size_t j=0; j<data->width; j++)
        {
            switch(data->field[i*data->width + j])
            {
                case '.':
                    new_field[i*data->width*2 + 2*j] = '.';
                    new_field[i*data->width*2 + 2*j + 1] = '.';
                    break;
                case 'O':
                    new_field[i*data->width*2 + 2*j] = '[';
                    new_field[i*data->width*2 + 2*j + 1] = ']';
                    break;
                case '@':
                    new_field[i*data->width*2 + 2*j] = '@';
                    new_field[i*data->width*2 + 2*j + 1] = '.';
                    break;
                case '#':
                    new_field[i*data->width*2 + 2*j] = '#';
                    new_field[i*data->width*2 + 2*j + 1] = '#';
                    break;
            }
        }
    }

    free(data->field);
    data->width *= 2;
    data->field = new_field;
}

typedef struct {
    size_t l;
} Box;

bool Box_compare(Box* a, Box* b)
{
    return (a->l == b->l);
}

DECLARE_ARRAY(Box, Box)
DEFINE_ARRAY(Box, Box)

typedef struct BoxArray BoxArray;
bool Front_compare(BoxArray* a, BoxArray* b)
{
    (void)a;
    (void)b;
    return false;
}

DECLARE_ARRAY(Front, BoxArray);
DEFINE_ARRAY(Front, BoxArray);

bool build_boxes_path(struct Data data, int dy, size_t y, struct FrontArray* fronts)
{
    bool found_blocker = true;

    while(found_blocker)
    {
        found_blocker = false;
        y += dy;

        struct BoxArray* cur_front = fronts->data + fronts->size - 1;
        struct BoxArray next_front = Box_array_init();

        bool could_overlap = false;
        char* prev_l = 0;

        for (const Box* box = cur_front->data; box<cur_front->data + cur_front->size; box++)
        {
            char* next_l = &data.field[y*data.width + box->l];
            char* next_r = next_l + 1;

            if (*next_l == '[')
            {
                Box_array_add(&next_front, (Box){.l=box->l});
                found_blocker = true;
            }
            else if (*next_l == ']' && (!could_overlap || next_l > prev_l + 2))
            {
                Box_array_add(&next_front, (Box){.l=box->l - 1});
                found_blocker = true;
            }
            else if (*next_l == '#')
            {
                return false;
            }

            if (*next_r == '[')
            {
                Box_array_add(&next_front, (Box){.l=box->l + 1});
                found_blocker = true;

                could_overlap = true;
                prev_l = next_l;
            }
            else if(*next_r == '#')
            {
                return false;
            }
            else
            {
                could_overlap = false;
                prev_l = 0;
            }
        }

        if (found_blocker)
        {
            Front_array_add(fronts, next_front);
        }
    }

    return !found_blocker;
}

void do_move_boxes(struct Data data, size_t y, int dy, struct FrontArray fronts)
{
    y = y + dy*fronts.size;

    for(struct BoxArray* front=fronts.data + fronts.size - 1; front >= fronts.data; front--)
    {
        for(Box* box=front->data; box < front->data + front->size; box++)
        {
            data.field[y*data.width + box->l] = '[';
            data.field[y*data.width + box->l + 1] = ']';
            data.field[(y-dy)*data.width + box->l] = '.';
            data.field[(y-dy)*data.width + box->l + 1] = '.';
        }

        y -= dy;
    }
}

bool try_move_boxes(struct Data data, size_t x, size_t y, int dx, int dy)
{
    if (dy == 0)
    {
        char* cur = &data.field[y*data.width + x];
        while(*cur != '.')
        {
            if (*cur == '#')
            {
                return false;
            }
            cur += dx;
        }
        if (dx > 0)
        {
            memmove(&data.field[y*data.width + x]+1, &data.field[y*data.width + x], sizeof(char)*(cur - &data.field[y*data.width + x]));
        }
        else
        {
            memmove(cur, cur - dx, sizeof(char)*(&data.field[y*data.width + x] - cur));
        } 
        data.field[y*data.width + x] = '.';
        return true;
    }
    else
    {
        struct FrontArray fronts = Front_array_init();
        struct BoxArray front = Box_array_init();

        const char* cur = &data.field[y*data.width + x];
        if (*cur == '[')
        {
            Box_array_add(&front, (Box){.l = x});
        }
        else
        {
            Box_array_add(&front, (Box){.l = x-1});
        }

        Front_array_add(&fronts, front);

        bool can_move = build_boxes_path(data, dy, y, &fronts);
        if (can_move)
        {
            do_move_boxes(data, y, dy, fronts);
        }

        for (size_t i=0; i<fronts.size; i++)
        {
            Box_array_free(&fronts.data[i]);
        }
        Front_array_free(&fronts);

        return can_move;
    }
    return false;
}

uint64_t part2(struct Data data)
{
    expand_field(&data);

    size_t pos[2];
    find_start(data, pos);    
    data.field[pos[1]*data.width + pos[0]] = '.';

    for (char* move = data.moves; move<data.moves+data.num_moves; move++)
    {
        int dx = 0;
        int dy = 0;

        switch(*move)
        {
            case '>':
                dx++;
                break;
            case '<':
                dx--;
                break;
            case 'v':
                dy++;
                break;
            case '^':
                dy--;
                break;
        }

        char* next_c = &data.field[(pos[1] + dy)*data.width + pos[0] + dx];

        if (*next_c == '.')
        {
            pos[0] += dx;
            pos[1] += dy;
        }
        else if (*next_c == '[' || *next_c == ']')
        {
            if(try_move_boxes(data, pos[0] + dx, pos[1] + dy, dx, dy))
            {
                pos[0] += dx;
                pos[1] += dy;
            }
        } // *next_c == '#' -> do nothing

    }

    uint64_t result = 0;
    
    for (size_t i=0; i<data.height; i++)
    {
        for (size_t j=0; j<data.width; j += 2)
        {
            if (data.field[i*data.width + j] == '[')
            {
                result += i*100 + j;
            }
            else if (data.field[i*data.width + j] == ']')
            {
                result += i*100 + (j-1);
            }
        }
    }

    free(data.field);
    free(data.moves);
    return result;
}

int main()
{
    clock_t start = clock();

    char* input = readInput("../day15/sample.txt");
    char* input2 = readInput("../day15/input.txt");

    struct Data data = parse(input);
    struct Data data2 = parse(input2);

    uint64_t res1 = part1(data);
    uint64_t res2 = part2(data2);

    printf("Part 1: %llu\n", res1);
    printf("Part 2: %lld\n", res2);
    
    free(input);
    free(input2);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);
    return 0;
}