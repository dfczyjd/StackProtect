# stackProtect
Программа предназаначена для защиты программ на C/C++ от внедрения вредоносного кода через уязвимость переполнения буфера

## Компиляция файлов с использованием программы:  
gcc -S -fno-stack-protector файл.c  
protect файл.s  
gcc файл.s  
Возможно добавление других флагов компиляции в соответствующие команды gcc и использование g++ для компиляции кода на C++.
Также можно компилировать несколько файлов одновременно, для этого следует подставить соотвтетствующие имена вместо "файл.c" и "файл.s".

## Содержимое репозитория:  
src - папка с исходным кодом  
protect.exe - сама программа  
test.c - простая программа для тестирования  
a.exe - результат компиляции test.c с использованием защиты  
compile.bat - скрипт, выполняющий компиляцию одного файла  
