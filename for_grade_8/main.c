#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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

     // Первый процесс
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
