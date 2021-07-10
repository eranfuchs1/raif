#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv)
{
    int file_num = argc - 2;
    int block_size;
    sscanf(argv[1], "%i", &block_size);
    char bytes[file_num][block_size];
    char block[block_size];
    char parity = 0;
    int looping = 1;
    FILE *ptrs[file_num];
    char cmd[50];
    cmd[0] = '\0';
    for (size_t i = 2; i < argc; i++)
    {
        sprintf(cmd, "touch %s", argv[i]);
        system(cmd);
        cmd[0] = '\0';
        ptrs[i-2] = fopen(argv[i], "w");
    }
    while (looping)
    {
        for (size_t i = 0; i < file_num - 1; i++)
        {
            int status = fread(block, sizeof(char), block_size, stdin);
            if (status != block_size)
            {
                looping = 0;
            }
            //memcpy(bytes[i], block, block_size);
            for (size_t j = 0; j < block_size; j++)
            {
                bytes[i][j] = block[j];
                bytes[file_num-1][j] += block[j];
            }
            fwrite(block, sizeof(char), block_size, ptrs[i]);
        }
        fwrite(bytes[file_num-1], sizeof(char), block_size, ptrs[file_num-1]);
        memset(bytes[file_num-1],0,block_size);
    }
    for (size_t i = 0; i < file_num; i++)
    {
        fclose(ptrs[i]);
    }
    return 0;
}