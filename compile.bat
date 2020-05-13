gcc -S %1 -o input.s
protect.exe input.s
gcc output.s