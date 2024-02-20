.data
    input_format:  .asciz " "
    output_format: .asciz "Sum: %d\n"
    buffer:        .space 64

.text
.global main

.extern atoi
.extern gets
.extern printf
.extern strchr

main:
    la a0, buffer
    li a1, 64
    call gets       // ввод строки с клавиатуры

    la a0, buffer
    li a1, ' '
    call strchr     // разбиение строки на числа

    la a5, buffer
    mv t0, a0
    mv a0, a5
    call atoi       // конвертирование первого числа
    mv t1, a0       // сохраняем в t1

    sb zero, 0(t0) // '\0'
    addi t0, t0, 1

    mv a0, t0
    call atoi       // конвертирование второго числа
    mv t2, a0       // сохраняем в t2

    add t3, t1, t2  // вычисляем сумму

    la a0, output_format
    mv a1, t3
    call printf

    li a7, 10
    li a0, 0
    ecall
