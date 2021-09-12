# How to enable sprintf float functionality

<https://community.platformio.org/t/arm-mbed-serial-printf-fails-to-emit-floats-when-using-platformio/389/5>

Add this line into platformio.ini:

    build_flags = -Wl,-u,_printf_float,-u,_scanf_float
