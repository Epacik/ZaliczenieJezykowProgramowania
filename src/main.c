#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "duthomhas/csprng.h"


#pragma region Defs

typedef enum {
    none = -1, zero = 0, one = 1, two = 2, three = 3, four = 4, five = 5, six = 6, seven = 7, eight = 8, nine = 9, plus = 10, minus = 11, times = 12, divide = 13, bracLeft = 14, bracRight = 15
} Symbols;

Symbols Syms[17] = {
    none, zero, one, two, three, four, five, six, seven, eight, nine, plus, minus, times, divide, bracLeft, bracRight
};

char SymbolChars[17] = {
    ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '(', ')'
};

Symbols GetSymbol(char sym);
char* GenerateExpression(int Length);
char GetSingleChar(int ID);
char* replace_char(char* str, char find, char replace);
char GetAllowedChar(Symbols* symbols, int size);
int RndNum(int to);

#pragma endregion



//Program jest napisany w C11, ISO/IEC 9899:2011 (https://en.wikipedia.org/wiki/C11_(C_standard_revision))
int main(int argc, char *argv[])
{

    setlocale(0, "");

    CSPRNG rng = csprng_create();        // Constructor
    if (!rng)
    {
        fprintf(stderr, "%s\n", "No CSPRNG! Fooey!");
        return 1;
    }

    long n = csprng_get_int(rng);           // Get an int
    srand((unsigned int)n);


    int Length;
    if (argc == 1)
    {
        //Pobieranie długości wyrażenia matematycznego
        puts("Podaj długość wyrażenia matematycznego");

        scanf("%d", &Length);
    }
    else {
        Length = atoi(argv[1]);
    }

    //liczba jest zbyt mała
    if (Length < 1) {
        puts("Wprowadzono zbyt małą liczbę, lub nie porowadzono liczby");
        return -1;
    }
    char* expr;

    expr = GenerateExpression(Length);
    expr = replace_char(expr, " ", "");


    //Test generatora exprażeń
    /*for (int i = 0; i < 50000; i++)
    {
        puts(GenerateExpression(Length));
    }*/
    if (argc == 1) {
        puts(expr);
    }
    FILE* f;

    char filename[] = "./dane.txt";

    f = fopen(filename, "w");

    fprintf(f, expr, 0);

    fclose(f);


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



char* GenerateExpression(int Length) {
    //Definiowanie wyrażenia
    char* expr;
    expr = malloc((sizeof(char) * (Length)));

    int i;
    for (i = 0; i < Length; i++) {
        char l;

        //Sprawdzanie jaki znak został wygenerowany jako ostatni
        Symbols lastChar = none;

        if(i - 1 != -1){
            lastChar = GetSymbol(expr[i - 1]);
        }

        //Zliczanie ilości niedomkniętych nawiasów
        int OpenedBrackets = 0;

        for (int j = 0; j < i; j++) {
            if (expr[j] == '(')
                OpenedBrackets += 1;
            else if (expr[j] == ')')
                OpenedBrackets -= 1;
        }

        if (i == 0) {
            //Poprawne znaki na pierwszej pozycji:
            // 1, 2, 3, 4, 5, 6, 7, 8, 9, (
            Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine, minus, bracLeft };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);

            if (Length <= 3) {
                //Pomija 2 ostatnie elementy dla wyrażenia o długości 3
                l = GetAllowedChar(Allowed, size - 2);
            }
            else {
                l = GetAllowedChar(Allowed, size);
            }
        }
        else if (Length - i > 3) {
            if (OpenedBrackets > 0 && Length - i - 1 == OpenedBrackets) {
                //do końca ciągu jest tyle znaków ile niedomkniętych nawiasów
                l = GetSingleChar(bracRight);
            }
            else if (lastChar == bracLeft) {
                //Poprawne znaki po otwarciu nawiasu:
                // 1, 2, 3, 4, 5, 6, 7, 8, 9, -, (
                Symbols Allowed[] = {  one, two, three, four, five, six, seven, eight, nine, minus, bracLeft };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }

            else if ((lastChar == times || lastChar == divide || lastChar == plus || lastChar == minus))
            {
                //Symbole dozwolone po znaku działania
                // 1, 2, 3, 4, 5, 6, 7, 8, 9, (
                Symbols Allowed[] = {  one, two, three, four, five, six, seven, eight, nine, bracLeft };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }

            else if (lastChar >= zero && lastChar <= nine) {

                if (OpenedBrackets > 0) {
                    if (i + 1 == Length) {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, bracRight };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                    else {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, plus, minus, times, divide, bracRight };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                }
                else {
                    if (i + 1 == Length) {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                    else {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, plus, minus, times, divide };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                }

            }
            else if (lastChar == bracRight) {
                Symbols Allowed[] = { plus, minus, times, divide };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }
        }
        else {
            if (OpenedBrackets > 0 && Length - i - 1 == OpenedBrackets) {
                l = GetSingleChar(bracRight);
            }
            else if (lastChar == bracLeft) {
                Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }

            else if ((lastChar == times || lastChar == divide || lastChar == plus || lastChar == minus))
            {
                Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }
            else if (lastChar >= zero && lastChar <= nine) {

                if (OpenedBrackets > 0 && Length - i - 1 > OpenedBrackets) {
                    if (i + 1 == Length) {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, bracRight };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                    else {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, plus, minus, times, divide, bracRight };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                }
                else {
                    if (i + 1 == Length) {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                    else {
                        Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, plus, minus, times, divide };
                        int size = sizeof(Allowed) / sizeof(Allowed[0]);
                        l = GetAllowedChar(Allowed, size);
                    }
                }

            }
            else if (lastChar == bracRight) {
                Symbols Allowed[] = { plus, minus, times, divide };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }
        }

        if ((Length - i - OpenedBrackets == 3 && (l == ')')) || (Length - i == 1 && (l == '*' || l == '/' || l == '-' || l == '+'))) {

            Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);
            l = GetAllowedChar(Allowed, size);
        }


        expr[i] = l;
    }

    for (int j = i - 1; j >= 0; j--) {
        if (j > 0 && expr[j] >= '0' && expr[j] <= '9' && expr[j - 1] == ')')
        {
            expr[j - 1] = expr[j];
            expr[j] = ')';
        }
    }

    int OpenedBrackets = 0;

    for (int j = 0; j < i; j++) {
        if (expr[j] == '(')
            OpenedBrackets += 1;
        else if (expr[j] == ')')
            OpenedBrackets -= 1;
    }
    //Zamiana niedomkniętych nawiasów na końcu na cyfry
    for (int j = i - 1; j >= 0; j--) {
        if (OpenedBrackets > 0 && expr[j] == '(') {
            //Poprawne znaki po otwarciu nawiasu:
                // 1, 2, 3, 4, 5, 6, 7, 8, 9
            Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);
            expr[j] = GetAllowedChar(Allowed, size);
            OpenedBrackets -= 1;
        }
    }

    expr[i] = '\0';
    return expr;
}

int RndNum(int to) {
    return (int)(rand() % to);
}



char GetSingleChar(int ID) {
    return SymbolChars[ID + 1];
}

///
Symbols GetSymbol(char sym) {
    int i = 0;
    while(sym != SymbolChars[i])
    {
        i++;
    }
    return Syms[i];
};


char GetAllowedChar(Symbols symbols[], int size) {
    int index = RndNum(size);
    char ch = GetSingleChar(symbols[index]);
    return ch;
}










