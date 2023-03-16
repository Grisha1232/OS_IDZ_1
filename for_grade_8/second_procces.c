#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

bool function(char string[]) {
    int len = strlen(string);
    int count = 0;
    for (int i = 0; i < len; ++i) {
        if (string[i] == '(') {
            count++;
        } else if (string[i] == ')') {
            count --;
        }
    }
    return count == 0;
}
const char* name_first = "first_pipe.fifo";
const char* name_second = "second_pipe.fifo";

const int mas_size = 5000;
int main(int argc, char *argv[]) {
    char str_buf[mas_size] = {0};
    char answer[5];
    
    int first_channel, second_channel, result;
    int size;

    mknod(name_first, S_IFIFO | 0666, 0);
    mknod(name_second, S_IFIFO | 0666, 0);

    second_channel = 0;

     // Второй процесс
    if((first_channel = open(name_first, O_RDONLY)) < 0)
    {
        printf("cant open FIFO for reading\n");
        exit(-1);
    }
    size = read(first_channel, str_buf, mas_size);
    if (size < 0) {
        printf("cant read from FIFO\n");
    }
    bool r = function(str_buf);
    if (close(first_channel) < 0) {
        printf("Cant close the FIFO\n");
        exit(-1);
    }
    if ((second_channel = open(name_second, O_WRONLY)) < 0) {
        printf("cant open for reading 2_FIFO\n");
        exit(-1);
    }
    if (r) {
        answer[0] = 't';
        answer[1] = 'r';
        answer[2] = 'u';
        answer[3] = 'e';
        answer[4] = ' ';
    } else {
        answer[0] = 'f';
        answer[1] = 'a';
        answer[2] = 'l';
        answer[3] = 's';
        answer[4] = 'e';
    }
    size = write(second_channel, answer, 5);
    printf("\n%s\n", answer);
    if (size != 5) {
        printf("Cant write all in 2_FIFO\n");
        exit(-1);
    }
    if (close(second_channel)) {
        printf("cant close 2_FIFO\n");
        exit(-1);
    }
    exit(0);
    
    return 0;
}
