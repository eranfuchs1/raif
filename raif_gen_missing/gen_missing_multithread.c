#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


int block_size;

struct byting
{
    char *bytes;
    FILE *ptr;
    int status_code;
};


void *work(void *param)
{
    struct byting *data_point = (struct byting *) param;
    data_point->status_code = fread(data_point->bytes,sizeof(char),block_size,data_point->ptr);
    return data_point;
}

int main (int argc, char **argv)
{
    struct byting **data;
    int file_num = argc - 3;
    int missing_index;
    sscanf(argv[2], "%i", &missing_index);
    if (missing_index < 0 || missing_index > file_num - 1)
    {
        fputs("bad missing index value\n", stderr);
        return 1;
    }
    sscanf(argv[1], "%i", &block_size);
    missing_index--;
    data = malloc(sizeof(struct byting*)*file_num);
    for (size_t i = 0; i < file_num; i++)
    {
        data[i] = (struct byting *) malloc(sizeof(struct byting));
        data[i][0] = (struct byting) {.bytes = (char*)malloc(sizeof(char)*block_size)};
    }
    int looping = 1;
    for (size_t i = 3; i < argc; i++)
    {
        if (i == missing_index + 3)
        {
            data[i - 3]->ptr = fopen(argv[i], "w");
            continue;
        }
        data[i - 3]->ptr = fopen(argv[i], "r");
    }
    int **indices = malloc(sizeof(int*)*file_num);
    int **answers = malloc(sizeof(int)*file_num);
    for (size_t i = 0; i < file_num; i++)
    {
        indices[i] = malloc(sizeof(int));
        answers[i] = malloc(sizeof(int));
        indices[i][0] = i;
    }
    pthread_t cThreads[file_num];
    
    while (looping)
    {
        for (size_t i = 0; i < file_num; i++)
        {
            if (i == missing_index)
            {
                continue;
            }
            pthread_create(&cThreads[i],NULL,work,(void *)data[i]);
        }
        int adder = 0;
        if (missing_index < 0)
        {
            adder = 1;
        }
        
        for (size_t i = 0; i < file_num + adder; i++)
        {
            if (i == missing_index)
            {
                continue;
            }
            void *answer;
            pthread_join(cThreads[i],&answer);
            data[i] = (struct byting *) answer;
            if (data[i]->status_code != block_size)
            {
                looping = 0;
            }
        }
        if (missing_index >= 0)
        {
            for (size_t j = 0; j < block_size; j++)
            {
                data[missing_index]->bytes[j] = data[file_num-1]->bytes[j];
            }
            for (size_t i = 0; i < file_num - 1; i++)
            {
                if (i == missing_index)
                {
                    continue;
                }
                if (missing_index < file_num - 1)
                {
                    for (size_t j = 0; j < block_size; j++)
                    {
                        data[missing_index]->bytes[j] -= data[i]->bytes[j];
                    }
                }
                if (missing_index == file_num - 1)
                {
                    for (size_t j = 0; j < block_size; j++)
                    {
                        data[missing_index]->bytes[j] += data[i]->bytes[j];
                    }
                }
            }
        }
        fwrite(data[missing_index]->bytes, sizeof(char), block_size, data[missing_index]->ptr);
    }
    for (size_t i = 0; i < file_num; i++)
    {
        fclose(data[i]->ptr);
    }
    return 0;
}