#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "kernel/param.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    procinfo_t plist[10];
    int lim = 10;

    // Вызываем системный вызов с недопустимым адресом буфера
    fprintf(1, "Testing with invalid buffer address:\n");
    int ret = ps_listinfo((struct procinfo *)0xFFFFFFFFFFFFF000, lim);
    fprintf(1, "ps_listinfo returned: %d\n\n", ret);
    if (ret > 0) {
        fprintf(2, "Error: invalid buffer address failed\n");
        exit(1);
    }

    // Вызываем системный вызов с недопустимым размером буфера
    fprintf(1, "Testing with insufficient buffer size:\n");
    ret = ps_listinfo(plist, 1); // Передаем буфер размером на один элемент
    fprintf(1, "ps_listinfo returned: %d\n\n", ret);
    if (ret <= 1) {
        fprintf(2, "Error: insufficient buffer size failed\n");
        exit(1);
    }

    // Вызываем системный вызов с пустым буфером
    fprintf(1, "Testing with empty buffer:\n");
    ret = ps_listinfo(0, lim);
    fprintf(1, "ps_listinfo returned: %d\n\n", ret);
    if (ret < 0) {
        fprintf(2, "Error: valid buffer failed\n");
        exit(1);
    }

    // Вызываем системный вызов с допустимым буфером
    fprintf(1, "Testing with valid buffer:\n");
    ret = ps_listinfo(plist, lim);
    fprintf(1, "ps_listinfo returned: %d\n\n", ret);
    if (ret < 0) {
        fprintf(2, "Error: valid buffer failed\n");
        exit(1);
    }

    exit(0);
}