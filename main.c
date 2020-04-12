#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma region Defs

typedef enum {
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
} Symbols;
#pragma endregion

char* GenerateExpression(int Dlugosc);
char GetSingleChar(int ID);
char* replace_char(char* str, char find, char replace);
char GetAllowedChar(Symbols* symbols);
int RndNum(int to);






//Program jest napisany w C11, ISO/IEC 9899:2011 (https://en.wikipedia.org/wiki/C11_(C_standard_revision))
int main(int argc, char *argv[])
{
    setlocale(0, "");
    //Pobieranie d³ugoœci wyra¿enia matematycznego
    puts("Podaj d³ugoœæ wyra¿enia matematycznego");
    int Dlugosc;
    scanf("%d", &Dlugosc);

    //liczba jest zbyt ma³a
    if (Dlugosc < 1) {
        puts("Wprowadzono zbyt ma³¹ liczbê, lub nie porowadzono liczby");
        return -1;
    }
    char* wyr;
    wyr = GenerateExpression(Dlugosc);
    wyr = replace_char(wyr, "ý", "\0");

    printf(wyr);

    FILE* f;

    f = fopen("./test.txt", "w");
    fprintf(f, wyr, 0);


    return 0;

}

char* replace_char(char* str, char find, char replace) {
    char* current_pos = strchr(str, find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos, find);
    }
    return str;
}



char* GenerateExpression(int Dlugosc) {
    //Definiowanie wyra¿enia
    char* wyr;
    wyr = malloc((sizeof(char) * (Dlugosc)));
    srand((unsigned int)time(NULL));
    

    int i;
    for (i = 0; i < Dlugosc; i++) {
        char l;

        if (i == 0 && Dlugosc <= 3) {
            //Poprawne znaki na pierwszej pozycji:
            // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
            Symbols Allowed[] = {zero, one, two, three, four, five, six, seven, eight, nine, minus };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);
            
            l = GetAllowedChar(Allowed, size);
        }
        else if (i == 0) {
            //Poprawne znaki na pierwszej pozycji:
            // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, (
            Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, minus, bracLeft };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);
            l = GetAllowedChar(Allowed, size);
        }

        wyr[i] = l;
    }

    wyr[i] = '\0';
    return wyr;
}

int RndNum(int to) {
    return (int)(rand() % to);
}



char GetSingleChar(int ID) {
    Symbols s = ID;

    char sym = '0';
    switch (s) {
        case zero: 
            return '0';
            break;
        case one:
            return '1';
            break;
        case two:
            return '2';
            break;
        case three:
            return '3';
            break;
        case four:
            return '4';
            break;
        case five:
            return '5';
            break;
        case six:
            return '6';
            break;
        case seven: 
            return '7';
            break;
        case eight:
            return '8';
            break;
        case nine:
            return '9';
            break;
        case plus:
            return '+';
            break;
        case minus:
            return '-';
            break;
        case times:
            return '*';
            break;
        case divide:
            return '/';
            break;
        case bracLeft:
            return '(';
            break;
        case bracRight:
            return ')';
            break;
    }

    return sym;
}


char GetAllowedChar(Symbols symbols[], int size) {
    int index = RndNum(size);
    char ch = GetSingleChar(symbols[index]);
    return ch;
}



