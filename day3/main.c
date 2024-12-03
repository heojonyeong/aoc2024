#include <stdlib.h>
#include <stdio.h>

#include "util.h"

enum State
{
    NONE,
    M,
    MU,
    MUL,
    MUL_,
    MUL_1,
    MUL_1_,
    MUL_1_2,
    D,
    DO,
    DO_,
    DON,
    DON_,
    DON_T,
    DON_T_
};

int calc(const char* input, bool part2)
{
    int num1 = 0;
    int num2 = 0;
    int result = 0;
    enum State state = NONE;
    bool enabled = true;

    while (*input != 0)
    {
        switch(state)
        {
            case NONE:
                if (*input == 'm')
                {
                    state = M;
                }
                else if (*input == 'd')
                {
                    state = D;
                }
                input++;
                break;
            case M:
                if (*input == 'u')
                {
                    state = MU;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case MU:
                if (*input == 'l')
                {
                    state = MUL;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case MUL:
                if (*input == '(')
                {
                    state = MUL_;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case MUL_:
                if (*input >= '0' && *input <= '9')
                {
                    state = MUL_1;
                    num1 = *input - '0';
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case MUL_1:
                if (*input >= '0' && *input <= '9')
                {
                    state = MUL_1;
                    num1 = num1*10 + *input - '0';
                    input++;
                }
                else if (*input == ',')
                {
                    state = MUL_1_;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case MUL_1_:
                if (*input >= '0' && *input <= '9')
                {
                    state = MUL_1_2;
                    num2 = *input - '0';
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case MUL_1_2:
                if (*input >= '0' && *input <= '9')
                {
                    state = MUL_1_2;
                    num2 = num2*10 + *input - '0';
                    input++;
                }
                else if (*input == ')')
                {
                    state = NONE;
                    if (!part2 || enabled)
                    {
                        result += num1*num2;
                    }
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case D:
                if (*input == 'o')
                {
                    state = DO;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case DO:
                if (*input == '(')
                {
                    state = DO_;
                    input++;
                }
                else if (*input == 'n')
                {
                    state = DON;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case DO_:
                if (*input == ')')
                {
                    state = NONE;
                    input++;
                    enabled = true;
                }
                else
                {
                    state = NONE;
                }
                break;
            case DON:
                if (*input == '\'')
                {
                    state = DON_;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case DON_:
                if (*input == 't')
                {
                    state = DON_T;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case DON_T:
                if (*input == '(')
                {
                    state = DON_T_;
                    input++;
                }
                else
                {
                    state = NONE;
                }
                break;
            case DON_T_:
                if (*input == ')')
                {
                    state = NONE;
                    input++;
                    enabled = false;
                }
                else
                {
                    state = NONE;
                }
                break;
        }
    }

    return result;
}

int main(int argc, char** argv)
{
    char* input = readInput("../day3/input.txt");

    int res1 = calc(input, false);
    int res2 = calc(input, true);

    printf("Part 1: %d\n", res1);
    printf("Part 2: %d\n", res2);
    free(input);
    return 0;
}