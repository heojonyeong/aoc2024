#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "util.h"

struct Operation
{
    unsigned _BitInt(3) instr;
    unsigned _BitInt(3) operand;
};

struct Program
{
    uint64_t A;
    uint64_t B;
    uint64_t C;
    size_t IP;

    struct Operation* code;
    size_t length;
};

struct Program parse_prog(char* input)
{
    char* end_ptr = input;
    uint64_t regA;
    uint64_t regB;
    uint64_t regC;

    while(*end_ptr < '0' || *end_ptr > '9')
    {
        end_ptr++;
    }
    regA = strtoull(end_ptr, &end_ptr, 10);

    while(*end_ptr < '0' || *end_ptr > '9')
    {
        end_ptr++;
    }
    regB = strtoull(end_ptr, &end_ptr, 10);
    
    while(*end_ptr < '0' || *end_ptr > '9')
    {
        end_ptr++;
    }
    regC = strtoull(end_ptr, &end_ptr, 10);

    while(*end_ptr < '0' || *end_ptr > '9')
    {
        end_ptr++;
    }

    size_t length = strlen(end_ptr);
    size_t num_ops = (length+1)/4;
    struct Operation* ops = malloc(sizeof(struct Operation)*num_ops);

    for (size_t i=0; i<num_ops; i++)
    {
        ops[i].instr = *end_ptr - '0';
        end_ptr += 2;
        ops[i].operand = *end_ptr - '0';
        end_ptr += 2;   
    }

    return (struct Program){.A = regA, .B = regB, .C = regC, .IP = 0, .code = ops, .length = num_ops};
}

void run_prog(struct Program p, char* out)
{
    size_t out_length = 0;
    while (p.IP < p.length)
    {
        struct Operation op = p.code[p.IP];
        uint64_t literal = op.operand;
        uint64_t combo = 0;

        //printf("\nA: %llu, B: %llu, C: %llu, IP: %llu\n", p.A, p.B, p.C, p.IP);
        //printf("Operation: %u, %u\n", (unsigned)op.instr, (unsigned)op.operand);

        switch (op.operand)
        {
            case 0:
                combo = 0;
                break;
            case 1:
                combo = 1;
                break;
            case 2:
                combo = 2;
                break;
            case 3:
                combo = 3;
                break;
            case 4:
                combo = p.A;
                break;
            case 5:
                combo = p.B;
                break;
            case 6:
                combo = p.C;
                break;
            case 7:
        }

        switch (op.instr)
        {
            case 0:
            {
                p.A = p.A >> combo;
                p.IP++;
                break;
            }
            case 1:
            {
                p.B ^= literal;
                p.IP++;
                break;
            }
            case 2:
            {
                p.B = combo%8;
                p.IP++;
                break;
            }
            case 3:
            {
                if (p.A == 0)
                {
                    p.IP++;
                }
                else
                {
                    p.IP = literal/2;
                }
                break;
            }
            case 4:
            {
                p.B ^= p.C;
                p.IP++;
                break;
            }
            case 5:
            {
                size_t written = snprintf(out+out_length, 100, "%c,", (unsigned int)(combo%8 + '0'));
                out_length += written;
                p.IP++;
                break;
            }
            case 6:
            {
                p.B = p.A >> combo;
                p.IP++;
                break;
            }
            case 7:
            {
                p.C = p.A >> combo;
                p.IP++;
                break;
            }
        }
    }
    out[out_length] = 0;
}


bool try(struct Program orig, char const* target, uint64_t A, size_t final_length, size_t current_length)
{
    if (final_length == current_length)
    {
        return true;
    }

    for (size_t i=0; i<8; i++)
    {
        struct Program p = orig;
        p.A = (A << 3) + i;

        char buffer[1000];
        run_prog(p, buffer);

        if (strcmp(target, buffer) == 0)
        {
            if (try(orig, target - 2, (A<<3)+i, final_length, current_length + 2))
            {
                printf("A: %llu\nOut: %s\n\n", p.A, buffer);
                return true;
            }
        }
    }
    return false;
}

void part2(struct Program orig, char const* target)
{
    size_t target_length = strlen(target);
    try(orig, target + target_length - 2, 0, target_length, 2);
}

int main()
{
    clock_t start = clock();
    char* input = readInput("../day17/input.txt");

    struct Program p = parse_prog(input);
    char target[100];

    for(size_t i=0; i<p.length; i++)
    {
        target[4*i] = (char)p.code[i].instr+'0'; 
        target[4*i + 1] = ',';
        target[4*i + 2] = (char)p.code[i].operand+'0';
        target[4*i + 3] = ',';
    }
    target[4*p.length] = 0;

    char buffer[1000];
    run_prog(p, buffer);
    part2(p, target);

    printf("Part1: %s\n", buffer);

    free(input);
    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);
    return 0;
}