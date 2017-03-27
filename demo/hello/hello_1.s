.data
hello_msg:      .asciz "Hello "
world_msg:      .asciz "World"
your_inp_msg:   .asciz "Your input: "
new_line_msg:   .asciz "\n"

.set BUFFER_SIZE, 512
buffer:         .zero BUFFER_SIZE

.text
.global _start
_start:
    mov $hello_msg, %rdi
    call write_string

    cmp $1, (%rsp)
    jle 1f

    mov 16(%rsp), %rdi
    call write_string
    jmp 2f

1:  mov $world_msg, %rdi
    call write_string

2:  mov $new_line_msg, %rdi
    call write_string

    call read_into_buffer

    mov $buffer, %rsi
    movb $0, (%rsi, %rax)

    mov $your_inp_msg, %rdi
    call write_string

    mov $buffer, %rdi
    call write_string

    mov $0, %rdi
    call exit

exit:
    # mov %rdi, %rdi        # exit code
    mov $60, %rax           # sys_exit is 60
    syscall

write_string:
    push %rdi
    call length_of_string
    pop %rdi

    mov %rax, %rdx          # number of bytes
    mov %rdi, %rsi          # address of the string
    mov $1, %rdi            # file descriptor, stdout is 1
    mov $1, %rax            # sys_write is 1
    syscall

    ret

read_into_buffer:
    mov $0, %rdi            # file descriptor, stdin is 0
    mov $buffer, %rsi       # address of the buffer
    mov $BUFFER_SIZE, %rdx  # size of buffer
    mov $0, %rax            # sys_read is 0
    syscall
    ret

length_of_string:
    mov $0, %rax
1:  cmpb $0, (%rdi)
    je 2f
    inc %rdi
    inc %rax
    jmp 1b
2:  ret
