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

const int mas_size = 5000;
int main(int argc, char *argv[]) {
    char str_buf[mas_size] = {0};
    char answer[5];
    if(argc != 3){
        printf("execute programm with three param: './prog.out input.txt output.txt'\n");
        return 0;
    }

    int second_channel[2], first_channel[2], result;
    int size;

    if(pipe(first_channel) < 0) { // Создаем первый канал для передачи во второй процесс
        printf("Cant open pipe\n");
        exit(-1);
    }
    if(pipe(second_channel) < 0) { // Создаем второй канал для передачи в третий процесс
        printf("Cant open pipe\n");
        exit(-1);
    }

    result = fork(); // создаем первый и второй процессы
    if(result < 0) {
        printf("Cant fork child\n");
        exit(-1);
    } else if (result > 0) { // Первый процесс
        if(close(first_channel[0]) < 0){
            printf("parent: Cant close reading side of pipe\n");
            exit(-1);
        }

        int file_to_read = 0;
        if((file_to_read = open(argv[1], O_RDONLY , 0666)) < 0) {
            printf("Cant open file\n");
            exit(-1);
        }
        size = read(file_to_read, str_buf, mas_size); // считываем из файла
        if(close(file_to_read) < 0) {
            printf("Cant close file\n");
        }
        size = write(first_channel[1], str_buf, mas_size); // записываем в первый канал
        if(size != mas_size){
            printf("Cant write all string to pipe\n");
            exit(-1);
        }
        if(close(first_channel[1]) < 0) {
            printf("parent: Cant close writing side of pipe\n");
            exit(-1);
        }
    } else {  // второй процесс
        int result1 = fork(); // создаем 3-ий процесс из второго
        if(result1 < 0) {
            printf("Cant fork child\n");
            exit(-1);
        } else if(result1 > 0){ // второй процесс
            if(close(first_channel[1]) < 0){
                printf("child: Cant close writing side of pipe\n");
                exit(-1);
            }
            size = read(first_channel[0], str_buf, mas_size); // считываем из первого канала
            if(size < 0){
                printf("Cant read string from pipe\n");
                exit(-1);
            }
            bool r = function(str_buf); // проверяем строку на правильность скобочной последовательности
            if(close(first_channel[0]) < 0){
                printf("child: Cant close reading side of pipe\n");
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

            size = write(second_channel[1], answer, 5); // записываем во второй канал

            if(size != 5){
                printf("Cant write all string to pipe\n (size = %d)",size);
                exit(-1);
            }
            if(close(second_channel[1]) < 0) {
                printf("parent: Cant close writing side of pipe\n");
                exit(-1);
            }
        } else{ // 3-ий процесс
            if(close(second_channel[1]) < 0){
                printf("child: Cant close writing side of pipe\n");
                exit(-1);
            }

            size = read(second_channel[0], str_buf, 5);// считываем из 2-го канала
            if(size < 0){
                printf("Cant read string from pipe\n");
                exit(-1);
            }

            if(close(second_channel[0]) < 0){
                printf("child: Cant close reading side of pipe\n");
                exit(-1);
            }
            int file_to_write = 0;

            if((file_to_write = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
                printf("Cant open file\n");
                exit(-1);
            }
            printf("%s \n", str_buf);
            size = write(file_to_write, str_buf, 5); // записываем в файл
            if(size != 5) {
                printf("Cant write all string\n");
                exit(-1);
            }
            if(close(file_to_write) < 0) {
                printf("Cant close file\n");
            }
        }
    }
    return 0;
}
