my_string: "Hello World!";
start(0x100):
    push(my_string);
    puts();
    pop();

    using i = AL;
    i = 0;
    loop:
        push(i);
        putc();
        pop();
        i++;
        if (i < 0xfF) goto loop;

    push(0x0);
    pop();
    exit();