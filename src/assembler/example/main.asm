my_string: "Hello World!";
start:
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
        if (i < 0xff) goto loop;

    push(0x0);
    pop();
    exit();