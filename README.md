# Uruchomienie

Aby uruchomić projekt należy użyć pliku '**zaliczeniejezykowprogramowania.sln**' (dla Visual Studio) lub **CMakeLists.txt**. Można również użyć z plików źródłowych znajdujących się w folderze **src**, jednakże w tym przypadku należy pamiętać o skompilowaniu zarówno pliku **main.c** jak i **csprng.c**. 


## Kod źródłowy

Cały napisany przezemnie kod znajduje się w pliku **main.c**. Plik **csprng.c** ( https://github.com/Duthomhas/CSPRNG ) został dołączony wyłącznie w celu zwiększenia losowości ziarna dla funkcji *srand*, a co za tym idzie, zwiększenia różnorodności generowanych wyrażeń matematycznych. Głównym powodem zwiększenia tej różnorodności było umożliwienie szybszego testowania generatora wyrażeń matematycznych. 

## Testy generatora

Testowanie odbywało się za pomocą skryptu w języku Python, znajdującego się w folderze *tests*. Skrypt ten po uruchomieniu będzie uruchamiał program tworzony na potrzeby zaliczenia generując wyrażenia od 3 do 1000 znaków, po 30 wyrażeń dla każdej kolejnej długości, po czym odczytuje każde z nich z pliku i ewaluuje stworzone wyrażenie wbudowaną w Pythona funkcją. Każde wyrażenie zostanie zapisane do pliku tekstowego generowanego obok skryptu pythona. Wyrażenia w tym pliku są poprzedzone jednym z 2 słów: 'passed', dla wyrażeń które przeszły test pomyślnie, lub 'FAILED', dla wyrażeń które nie przeszły testu. Ponadto, wyrażenia które nie przeszły testu pomyślnie, zostają wypisane do okna konsoli




