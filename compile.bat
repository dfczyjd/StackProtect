gcc -S -fno-stack-protector %1 -o input.s
protect.exe -x86 input.s
gcc input.s