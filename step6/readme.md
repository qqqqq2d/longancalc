# Kirjeldus

Esimene soov on meil järjestikporti andmete saatmine tööle saada.

Leidsime internetist aadressilt https://www.sigmdel.ca/michel/ha/gd32v/longan_nano_01_en.html funktsiooni init_uart0 ja _put_char ja panime need eraldi faili uart.cpp ja uart.h.
Seejärel saime main.cpp failist välja kutsuda printf funktsiooni.
 
Teiseks tahame ühendada klaviatuuri ja klahvivajutusi näha arvuti ekraanil.

Ühendasime klaviatuuri juhtmetega arendusplaadi külge.

![klaviatuur](2021-10-05-18-24-52.png)

![paigutus](2021-10-05-18-25-52.png)

Leidsime internetist aadressilt https://ee-diary.blogspot.com/2020/10/How-to-Read-Keypad-with-Arduino-without-Library.html näidiskoodi ja panime selle enda klaviatuuri jaoks tööle.
