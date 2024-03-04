.data
    input_format:  .asciz " "
    output_format: .asciz "Sum: %d\n"
    buffer:        .space 512
    read_error:    .asciz "Read error\n"
    args_error:    .asciz "Wrong string type\n"

.text
.global main

.extern atoi
.extern gets
.extern printf
.extern strchr

main:
    la a0, buffer
    li a1, 512
    call gets       // ввод строки с клавиатуры

    beqz a0, read_fail

    la a0, buffer
    li a1, 0x20
    call strchr     // разбиение строки на числа

    beqz a0, args_fail

    sb zero, 0(a0) // '\0'
    addi t0, a0, 1

    la a0, buffer
    call atoi       // конвертирование первого числа
    mv t1, a0       // сохраняем в t1

    mv a0, t0
    call atoi        // конвертирование второго числа
    mv t2, a0       // сохраняем в t2

    add t3, t1, t2  // вычисляем сумму

    la a0, output_format
    mv a1, t3
    call printf

    li a7, 2
    li a0, 0
    ecall

read_fail:
    li a7, 16
    li a0, 2
    la a1, read_error
    li a2, 50
    ecall

    li a7, 2
    li a0, 1
    ecall

args_fail:
    li a7, 16
    li a0, 2
    la a1, args_error
    li a2, 50
    ecall

    li a7, 2
    li a0, 1
    ecall