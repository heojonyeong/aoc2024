#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "util.h"

typedef struct {
    size_t start;
    uint8_t length;
} FileBlock;

typedef FileBlock EmptyBlock;

bool FileBlock_compare(FileBlock const* a, FileBlock const* b)
{
    return (a->start == b->start) && (a->length == b->length);
}

bool EmptyBlock_compare(EmptyBlock const* a, EmptyBlock const* b)
{
    return (a->start == b->start) && (a->length == b->length);
}

DECLARE_ARRAY(FileBlock, FileBlock)
DECLARE_ARRAY(EmptyBlock, EmptyBlock)
DEFINE_ARRAY(FileBlock, FileBlock)
DEFINE_ARRAY(EmptyBlock, EmptyBlock)

void build_structs(char const* input, struct FileBlockArray* files, struct EmptyBlockArray* gaps)
{
    size_t start = 0;
    for (size_t i = 0; *input != 0; i++, input++)
    {
        uint8_t length = *input - '0';

        if (i%2 == 0)
        {
            FileBlock block;
            block.start = start;
            block.length = length;

            FileBlock_array_add(files, block);
        }
        else if (length > 0) // ignore gaps of size 0
        {
            EmptyBlock block;
            block.start = start;
            block.length = length;

            EmptyBlock_array_add(gaps, block);
        }

        start += length;
    }
}

size_t getTotalFileSize(struct FileBlockArray const* files)
{
    size_t res = 0;
    for (size_t i = 0; i<files->size; i++)
    {
        res += files->data[i].length;
    }

    return res;
}

uint64_t part1(struct FileBlockArray const* files_i, struct EmptyBlockArray const* gaps_i)
{
    struct FileBlockArray files = FileBlock_array_copy(files_i);
    struct EmptyBlockArray gaps = EmptyBlock_array_copy(gaps_i);

    uint64_t result = 0;

    size_t file_idx_s = 0;
    size_t gap_idx_s = 0;
    size_t file_idx_e = files.size - 1;

    size_t total_file_size = getTotalFileSize(&files);

    for (size_t position = 0; position < total_file_size; position++)
    {
        FileBlock const* file = &files.data[file_idx_s];
        if (file->start <= position && file->start + file->length-1 >= position)
        {
            // current position is at an original file position
            result += position*file_idx_s;
            if (file->start + file->length-1 == position)
            {
                file_idx_s++;
            }
        }
        else
        {
            // current position is at a gap
            EmptyBlock* gap = &gaps.data[gap_idx_s];
            FileBlock* file_to_move = &files.data[file_idx_e];

            result += position*file_idx_e;
            gap->start++;
            gap->length--;
            file_to_move->length--;

            if (gap->length == 0)
            {
                gap_idx_s++;
            }
            if (file_to_move->length == 0)
            {
                file_idx_e--;
            }
        }
    }

    FileBlock_array_free(&files);
    EmptyBlock_array_free(&gaps);

    return result;
}

uint64_t part2(struct FileBlockArray const* files_i, struct EmptyBlockArray const* gaps_i)
{
    struct FileBlockArray files = FileBlock_array_copy(files_i);
    struct EmptyBlockArray gaps = EmptyBlock_array_copy(gaps_i);

    uint64_t result = 0;

    for (size_t i=files.size - 1; i>0; i--) // ignore first file
    {
        FileBlock* file = &files.data[i];
        // check for gap that is big enough
        for (size_t j = 0; j<gaps.size; j++)
        {
            EmptyBlock * gap = &gaps.data[j];
            if (gap->start > file->start)
            {
                break;
            }

            if (gap->length >= file->length)
            {
                // gap is big enough -> move file
                file->start = gap->start;
                gap->start += file->length;
                gap->length -= file->length;
                break;
            }
        }

        // file is now in final position
        for (size_t pos=file->start; pos<file->start + file->length; pos++)
        {
            result += pos*i;
        }
    }

    FileBlock_array_free(&files);
    EmptyBlock_array_free(&gaps);

    return result;
}

int main()
{
    clock_t start = clock();

    char* input = readInput("../day9/input.txt");

    struct FileBlockArray files = FileBlock_array_init();
    struct EmptyBlockArray gaps = EmptyBlock_array_init();
    build_structs(input, &files, &gaps);
    free(input);

    uint64_t res1 = part1(&files, &gaps);
    uint64_t res2 = part2(&files, &gaps);

    printf("Part 1: %llu\n", res1);
    printf("Part 2: %llu\n", res2);

    FileBlock_array_free(&files);
    EmptyBlock_array_free(&gaps);

    clock_t end = clock();
    double time_used = ((double)(end-start))/CLOCKS_PER_SEC;
    printf("Time used: %fms\n", time_used*1000);

    return 0;
}