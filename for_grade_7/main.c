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
    char str_buf[mas_size];
    char answer[5];
    if(argc != 3){
        printf("execute programm with three param: './prog.out input.txt output.txt'\n");
        return 0;
    }

    int first_channel, second_channel, result;
    int size;

    mknod(name_first, S_IFIFO | 0666, 0);
    mknod(name_second, S_IFIFO | 0666, 0);

    second_channel = 0;

    result = fork(); // создаем первый и второй процессы
    if (result < 0) {
        printf("Cant fork child\n");
        exit(-1);
    } else if (result > 0) { // Первый процесс
        int file_to_read = 0;

        if ((file_to_read = open(argv[1], O_RDONLY, 0666)) < 0) {
            printf("Cant open the file\n");
            exit(-1);
        }
        size = read(file_to_read, str_buf, mas_size);
        if (close(file_to_read) < 0) {
            printf("Cant close the file\n");
            exit(-1);
        }

        if ((first_channel = open(name_first, O_WRONLY)) < 0) {
            printf("Cant open FIFO for writing\n");
            exit(-1);
        }

        size = write(first_channel, str_buf, mas_size);
        if (size != mas_size) {
            printf("Cant write all string to FIFO\n");
            exit(-1);
        }

        if (close(first_channel) < 0) {
            printf("Cant close FIFO");
            exit(-1);
        }
    } else {
         // Второй процесс
        if((first_channel = open(name_first, O_RDONLY)) < 0)
        {
            printf("Child: cant open FIFO for reading\n");
            exit(-1);
        }
        size = read(first_channel, str_buf, mas_size);
        if (size < 0) {
            printf("child: cant read from FIFO\n");
        }
        bool r = function(str_buf);
        if (close(first_channel) < 0) {
            printf("child: Cant close the FIFO\n");
            exit(-1);
        }
        if ((second_channel = open(name_second, O_WRONLY)) < 0) {
            printf("child: cant open for reading 2_FIFO\n");
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
        // printf("\n%s\n", answer);
        if (size != 5) {
            printf("child: Cant write all in 2_FIFO\n");
            exit(-1);
        }
        if (close(second_channel)) {
            printf("child: cant close 2_FIFO\n");
            exit(-1);
        }
        exit(0);
    }
    // Первый процесс
    if ((second_channel = open(name_second, O_RDONLY)) < 0) {
        printf("child/child: cant open 2_FIFO\n");
        exit(-1);
    }
    
    size = read(second_channel, str_buf, 5);
    if (size < 0) {
        printf("child/child: cant read from 2_FIFO\n");
        exit(-1);
    }
    
    if (close(second_channel) < 0) {
        printf("child/child: cant close 2_FiFO\n");
        exit(-1);
    }
    
    int file_to_write = 0;
    
    if ((file_to_write = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("child/child: cant open out file to write\n");
        exit(-1);
    }

    printf("\n------\n%s\n------\n", str_buf);
    size = write(file_to_write, str_buf, 5);
    if (size != 5) {
        printf("child/child: cant write to out file\n");
        exit(-1);
    }
    if (close(file_to_write) < 0) {
        printf("child/child: cant close out file\n");
        exit(-1);
    }
    return 0;
}
