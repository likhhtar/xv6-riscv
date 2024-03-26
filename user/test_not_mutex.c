#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Wrong input\n");
        exit(-1);
    }

    int pipe_fd1[2], pipe_fd2[2];
    if (pipe(pipe_fd1) == -1 || pipe(pipe_fd2) == -1) { // Создаем два канала
        fprintf(2, "Error in pipe\n");
        exit(-1);
    }

    int pid = fork(); // Создаем новый процесс
    if (pid == -1) {
        fprintf(2, "Error in fork\n");
        exit(-1);
    }

    if (pid == 0) { // Дочерний процесс
        close(pipe_fd2[0]); // Закрываем конец канала для чтения в дочернем процессе
        close(pipe_fd1[1]); // Закрываем конец канала для записи в дочернем процессе
        char buffer;
        while (read(pipe_fd1[0], &buffer, 1) > 0) { // Читаем из канала данные, пока они доступны
            printf("%d: received '%c'\n", getpid(), buffer); // Выводим принятый символ и PID дочернего процесса
            write(pipe_fd2[1], &buffer, 1); // Отправляем принятый символ обратно в родительский процесс
        }
        close(pipe_fd2[0]); // Закрываем конец канала для чтения в дочернем процессе
        close(pipe_fd1[1]); // Закрываем конец канала для записи в дочернем процессе
        exit(0); // Завершаем дочерний процесс
    } else { // Родительский процесс
        close(pipe_fd2[1]); // Закрываем конец канала для записи в родительском процессе
        close(pipe_fd1[0]); // Закрываем конец канала для чтения в родительском процессе
        write(pipe_fd1[1], argv[1], strlen(argv[1])); // Записываем сообщение в канал для дочернего процесса
        close(pipe_fd1[1]); // Закрываем конец канала для записи в родительском процессе
        char buffer;
        while (read(pipe_fd2[0], &buffer, 1) > 0) { // Читаем из канала данные, пока они доступны
            printf("%d: received '%c'\n", getpid(), buffer); // Выводим принятый символ и PID родительского процесса
        }
        close(pipe_fd2[0]); // Закрываем конец канала для чтения в родительском процессе
        wait(0); // Ожидаем завершения дочернего процесса
        exit(0); // Завершаем родительский процесс
    }
}
