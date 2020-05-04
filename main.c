#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
char GetAllowedChar(Symbols* symbols);
int RndNum(int to);

#pragma endregion



//Program jest napisany w C11, ISO/IEC 9899:2011 (https://en.wikipedia.org/wiki/C11_(C_standard_revision))
int main(int argc, char *argv[])
{
    setlocale(0, "");
    //Pobieranie długości wyrażenia matematycznego
    puts("Podaj długość wyrażenia matematycznego");
    int Length;
    scanf("%d", &Length);

    //liczba jest zbyt mała
    if (Length < 1) {
        puts("Wprowadzono zbyt małą liczbę, lub nie porowadzono liczby");
        return -1;
    }
    char* wyr;

    wyr = GenerateExpression(Length);
    wyr = replace_char(wyr, " ", "");

    puts(wyr);

    FILE* f;

    char filename[] = "./dane.txt";

    f = fopen(filename, "w");
    fprintf(f, wyr, 0);
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

int OpenedBrackets = 0;

char* GenerateExpression(int Length) {
    //Definiowanie wyrażenia
    char* wyr;
    wyr = malloc((sizeof(char) * (Length)));
    srand((unsigned int)time(NULL));

    int i;
    for (i = 0; i < Length; i++) {
        char l;

        Symbols lastChar = none;

        if(i - 1 != -1){
            lastChar = GetSymbol(wyr[i - 1]);
        }

        OpenedBrackets = 0;

        for (int j = 0; j < i; j++) {
            if (wyr[j] == '(')
                OpenedBrackets += 1;
            else if (wyr[j] == ')')
                OpenedBrackets -= 1;
        }

        if (i == 0 && Length <= 3) {
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
        else if (Length - i > 3) {
            if (OpenedBrackets > 0 && Length - i - 1 == OpenedBrackets) {
                l = GetSingleChar(bracRight);
            }
            else if (lastChar == bracLeft) {
                Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, bracLeft };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }

            else if ((lastChar == times || lastChar == divide || lastChar == plus || lastChar == minus))
            {
                Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine, bracLeft };
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
                Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine };
                int size = sizeof(Allowed) / sizeof(Allowed[0]);
                l = GetAllowedChar(Allowed, size);
            }

            else if ((lastChar == times || lastChar == divide || lastChar == plus || lastChar == minus))
            {
                Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine };
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

            Symbols Allowed[] = { zero, one, two, three, four, five, six, seven, eight, nine };
            int size = sizeof(Allowed) / sizeof(Allowed[0]);
            l = GetAllowedChar(Allowed, size);
        }


        wyr[i] = l;
    }

    for (int j = i - 1; j >= 0; j--) {
        if (j > 0 && wyr[j] >= '0' && wyr[j] <= '9' && wyr[j - 1] == ')')
        {
            wyr[j - 1] = wyr[j];
            wyr[j] = ')';
        }
    }

    wyr[i] = '\0';
    return wyr;
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




