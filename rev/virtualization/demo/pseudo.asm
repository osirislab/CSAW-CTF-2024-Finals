section .data
    prompt db "Please enter your password", 10  ; Prompt message
    fail_message db "bad password", 10           ; Failure message
    correct_password db "I_LOVE_KOTLIN"          ; Correct password for checking
    xor_key db 181, 182, 39, 21, 160, 184, 179, 236, 30, 233, 42, 47, 1

section .bss
    buffer resb 32                               ; 32-byte buffer for password input

section .text

_start:
    ; for sake of buffer, set rbp = rsp
    mov rbp, rsp    

    ; Print prompt message
    mov rax, 1                                  ; syscall number for sys_write
    mov rdi, 1                                  ; file descriptor 1 (stdout)
    mov rsi, prompt                             ; address of prompt message
    mov rdx, 27                         ; length of prompt message
    syscall

    ; Read input from stdin
    mov rax, 0                                  ; syscall number for sys_read
    mov rdi, 0                                  ; file descriptor 0 (stdin)
    mov rsi, buffer                             ; address of buffer to store input
    mov rdx, 32                                 ; maximum number of bytes to read
    syscall

    ; Remove newline if present
    mov rcx, rax                                ; save the number of bytes read
    dec rcx                                     ; point to last byte read
    mov al, byte [buffer+rcx]                 ; load last byte into al
    cmp al, 10                                  ; check if it's a newline character
    jne .no_newline                             ; if not, skip removal
    mov byte [buffer+rcx], 0                  ; replace newline with null byte
.no_newline:

    ; XOR the first 20 bytes with the provided key, this is meant to throw player off
    ; i.e. this challenge is not solvable at all without patching
    mov rcx, 20                                 ; number of bytes to XOR
    xor rbx, rbx                                ; reset rbx to 0 (index)

.xor_loop:
    mov al, byte [buffer+rbx]                 ; load byte from buffer
    xor al, byte [xor_key+rbx]                ; XOR with corresponding byte in xor_key
    mov byte [buffer+rbx], al                 ; store back in buffer
    inc rbx                                     ; increment index
    loop .xor_loop                              ; repeat for next byte

    ; Compare the first 20 bytes with the correct password
    mov rsi, buffer                             ; address of the buffer
    mov rdi, correct_password                   ; address of correct password
    mov rcx, 20                                 ; length of password to check
    repe cmpsb                                  ; compare each byte
    jne .fail                                   ; if not equal, go to failure

    ; If password matches, call win (placeholder)
    call win
    jmp .exit                                   ; exit after calling win

.fail:
    ; Print failure message
    mov rax, 1                                  ; syscall number for sys_write
    mov rdi, 1                                  ; file descriptor 1 (stdout)
    mov rsi, fail_message                       ; address of failure message
    mov rdx, 13                   ; length of failure message
    syscall

.exit:
    ; Exit the program
    mov rax, 60                                 ; syscall number for sys_exit
    xor rdi, rdi                                ; exit code 0
    syscall


decrypt:
    push    rbp
    mov     rbp, rsp
    mov     [rbp-0x18], rdi
    mov     rax, [rbp-0x18]
    mov     [rbp-0x8], rax
    mov     rax, 0x5A5A5A5A5A5A5A5A
    xor     [rbp-0x8], rax
    ror     QWORD [rbp-0x8], 5
    mov     rax, 0x0A5A5A5A5A5A5A5A5
    xor     [rbp-0x8], rax
    mov     rax, [rbp-0x8]
    pop     rbp
    ret




win:
    ; Prime return addr
    push rbp
    mov rbp, rsp    

    ; Call decrypt on each key and store on stack
    mov rdi, 16232600778930717838                ; first key
    call decrypt
    push rax                                    ; push decrypted value onto stack

    mov rdi, 14197361187156977544                ; second key
    call decrypt
    push rax                                    ; push decrypted value onto stack

    mov rdi, 9376602726126879361                ; third key
    call decrypt
    push rax                                    ; push decrypted value onto stack

    ; Print it out
    ; Print prompt message
    mov rax, 1                                  ; syscall number for sys_write
    mov rdi, 1                                  ; file descriptor 1 (stdout)
    mov rsi, rsp                                ; address of prompt message
    mov rdx, 24                         ; length of prompt message
    syscall        

    ; Restore
    mov rsp, rbp
    pop rbp
    ret