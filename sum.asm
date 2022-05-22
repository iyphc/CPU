awesome_func:
pop dx
pop cx
push 11
push dx
ret


start:
push 10
push 15
pop ax
pop bx
push ax
push bx
push 10
call awesome_func
pop cx
pop bx
pop ax





