#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* GenerateExpression(int Dlugosc);
char GetSingleChar(int ID);

//Program jest napisany w C11, ISO/IEC 9899:2011 (https://en.wikipedia.org/wiki/C11_(C_standard_revision))
int main(int argc, char *argv[])
{
    setlocale(0, "");
    //Pobieranie d³ugoœci wyra¿enia matematycznego
    puts("Podaj d³ugoœæ wyra¿enia matematycznego");
    char tekst[8], *n;
    n = gets(tekst);

    //konwersja char[] do int
    int Dlugosc = atoi(tekst);

    //liczba jest zbyt ma³a
    if (Dlugosc < 1) {
        puts("Wprowadzono zbyt ma³¹ liczbê, lub nie porowadzono liczby");
        return -1;
    }
    char* wyr;
    wyr = GenerateExpression(Dlugosc);

    printf(wyr);

    return 0;

}

enum Symbols
{
    zero = 0,
    one = 1,
    two = 2,
    three = 3,
    four = 4,
    five = 5,
    six = 6,
    seven = 7,
    eight = 8,
    nine = 9,
    plus = 10,
    minus = 11,
    times = 12,
    divide = 13,
    bracLeft = 14,
    bracRight = 15
};

char* GenerateExpression(int Dlugosc) {
    //Definiowanie wyra¿enia
    char* wyr;
    wyr = malloc(sizeof(char) * Dlugosc);
    srand((unsigned int)time(NULL));

    
    for (int i = 0; i < Dlugosc; i++) {
        char l = GetSingleChar(RndNum());
        wyr[i] = l;
    }

    
    return wyr;
}

int RndNum() {
    return (int)(rand() % 16);
}

char GetSingleChar(int ID) {
    enum Symbols s = ID;

    char sym = '0';
    switch (s) {
        case zero: 
            sym = '0';
            break;
        case one:
            sym = '1';
            break;
        case two:
            sym = '2';
            break;
        case three:
            sym = '3';
            break;
        case four:
            sym = '4';
            break;
        case five:
            sym = '5';
            break;
        case six:
            sym = '6';
            break;
        case seven: 
            sym = '7';
            break;
        case eight:
            sym = '8';
            break;
        case nine:
            sym = '9';
            break;
        case plus:
            sym = '+';
            break;
        case minus:
            sym = '-';
            break;
        case times:
            sym = '*';
            break;
        case divide:
            sym = '/';
            break;
        case bracLeft:
            sym = '(';
            break;
        case bracRight:
            sym = ')';
            break;
    }

    return sym;
}
