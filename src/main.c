#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <regex>

//  Dla tworzenia ziarna do srand
#include "duthomhas/csprng.h"


#pragma region Defs

// enum zawierający symbole których może użyć generator
typedef enum {
    none = -1, zero = 0, one = 1, two = 2, three = 3, four = 4, five = 5, six = 6, seven = 7, eight = 8, nine = 9, plus = 10, minus = 11, times = 12, divide = 13, bracLeft = 14, bracRight = 15
} Symbols;

// Tablice ułatwiające szybkie konwersje pomiędzy enumem a znakami
Symbols Syms[17] = {
    none, zero, one, two, three, four, five, six, seven, eight, nine, plus, minus, times, divide, bracLeft, bracRight
};

char SymbolChars[17] = {
    ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '(', ')'
};

// Nie twięrdzę, że takie rozwiązanie jest najbardziej efektywne, ani choćby eleganckie, ale to podejście znacząco ułatwiło mi zaimplementowanie generatora wyrażeń matematycznych


// Definicje funkcji użytych poniżej

// Zwraca wartość enuma 'Symbols' odpowiadającą przekazanemu znakowi
Symbols GetSymbol(char sym);

// Funkcja generująca wyrażenia i implementująca większość mechanizmów zapobiegających generowaniu niepoprawnych wyrażeń
char* GenerateExpression(int Length);

// Zwraca znak odpowiadający przekazanej wartości enuma 'Symbols'. Jako argument jest użyty int, ponieważ nie chciało mi się szukać jak wstawić typ danych 'Symbols' jako typ argumentu, szczególnie, że to działa wystarczająco dobrze
char GetSingleChar(int ID);

//Funkcja służąca do zamiany jednego znaku na inny w ciągu znaków
char* replace_char(char* str, char find, char replace);

// Zwraca jeden znak z tablicy mogących wystąpić znaków. Do funkcji przekazywana jest tablica z symbolami które mogą zostać wylosowane (np.: zaraz po zamknięciu nawiasu nie powinna się znajdować liczba, więc cyfry nie znalazłyby się w tej tablicy), a także jej wielkość. Mimo, że to powoduje powstanie sporej ilości kodu którą można by uniknąć, przenosząc obliczanie wielkości tablicy do wnętrza funkcji, ale daje to możliwość łatwiejszego losowania z częściowej zawartości tablicy, np.: losowanie tylko z pierwszych 15 znaków podczas gdy tablica miałaby 17.
char GetAllowedChar(Symbols symbols[], int size);

//Generuje losową liczbę z przedziału od 0 do argumentu 'to'
int RndNum(int to);

#pragma endregion



// Program jest napisany w C11, ISO/IEC 9899:2011 (https://en.wikipedia.org/wiki/C11_(C_standard_revision))
int main(int argc, char *argv[])
{
    setlocale(0, "pl-PL");

    // Tworzenie ziarna do losowania
    CSPRNG rng = csprng_create();
    if (!rng)
    {
        fprintf(stderr, "%s\n", "No CSPRNG! Fooey!");
        return 1;
    }

    long n = csprng_get_int(rng);
    srand((unsigned int)n);


    int Length;
    // Pobieranie długości wyrażenia matematycznego z konsoli lub argumentu linii komend
    if (argc == 1)
    {
        puts("Podaj długość wyrażenia matematycznego");

        scanf("%d", &Length);
    }
    else {
        Length = atoi(argv[1]);
    }

    // liczba jest zbyt mała
    if (Length < 1) {
        puts("Wprowadzono zbyt małą liczbę, lub nie porowadzono liczby");
        return -1;
    }
    char* expr;

    expr = GenerateExpression(Length);
    expr = replace_char(expr, " ", "");

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
    // Definiowanie wyrażenia
    char* expr;
    expr = malloc((sizeof(char) * (Length)));

    int i;
    for (i = 0; i < Length; i++) {
        char l;

        // Sprawdzanie jaki znak został wygenerowany jako ostatni
        Symbols lastChar = none;

        if(i - 1 != -1){
            lastChar = GetSymbol(expr[i - 1]);
        }

        // Zliczanie ilości niedomkniętych nawiasów
        int OpenedBrackets = 0;

        for (int j = 0; j < i; j++) {
            if (expr[j] == '(')
                OpenedBrackets += 1;
            else if (expr[j] == ')')
                OpenedBrackets -= 1;
        }

        if (i == 0) {
            // Poprawne znaki na pierwszej pozycji:
            //  1, 2, 3, 4, 5, 6, 7, 8, 9, (
            Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine, minus, bracLeft };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);

            if (Length <= 3) {
                // Pomija 2 ostatnie elementy dla wyrażenia o długości 3
                l = GetAllowedChar(Allowed, size - 2);
            }
            else {
                l = GetAllowedChar(Allowed, size);
            }
        }
        else if (Length - i > 3) {
            if (OpenedBrackets > 0 && Length - i - 1 == OpenedBrackets) {
                // do końca ciągu jest tyle znaków ile niedomkniętych nawiasów
                l = GetSingleChar(bracRight);
            }
            else if (lastChar == bracLeft) {
                // Poprawne znaki po otwarciu nawiasu:
                //  1, 2, 3, 4, 5, 6, 7, 8, 9, -, (
                Symbols Allowed[] = {  one, two, three, four, five, six, seven, eight, nine, minus, bracLeft };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }

            else if ((lastChar == times || lastChar == plus || lastChar == minus))
            {
                // Symbole dozwolone po znaku działania
                //  1, 2, 3, 4, 5, 6, 7, 8, 9, (
                Symbols Allowed[] = {  one, two, three, four, five, six, seven, eight, nine, bracLeft };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }
            else if (lastChar == divide) {
                // Symbole dozwolone po znaku działania
                //  1, 2, 3, 4, 5, 6, 7, 8, 9
                Symbols Allowed[] = { one, two, three, four, five, six, seven, eight, nine};
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
    // Zamiana niedomkniętych nawiasów na końcu na cyfry
    for (int j = i - 1; j >= 0; j--) {
        if (OpenedBrackets > 0 && expr[j] == '(') {
            // Poprawne znaki po otwarciu nawiasu:
                //  1, 2, 3, 4, 5, 6, 7, 8, 9
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

// /
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















