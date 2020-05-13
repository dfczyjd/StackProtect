#pragma once

// Код инициализации
const char* INIT_x64 = "\
\tpushq\t$0x%x\n\
%s\
\taddq\t$4, %%rsp\n\
\tmovq\t%%rax, %%rdi\n\
\tmovq\t$0, (%%rax)\n\
";

// Код для замены инструкции call
const char* CALL_x64 = "\
\tmovq\t(%rdi), %rsi\n\
\tincq\t%rsi\n\
\tmovq\t%rsi, (%rdi)\n\
\tpushq\t$0\n\
\tcall\tsave_ret\n\
\tjmp\t\t\
";

// Код для замены инструкции ret
const char* RET_x64 = "\
\tmovq\t(%rdi), %rsi\n\
\tdecq\t%rsi\n\
\tmovq\t%rsi, (%rdi)\n\
\tincq\t%rsi\n\
\tsubq\t$0x4, %rsp\n\
\tjmp\t\t*(%rdi,%rsi,4)\n\
";

// Код вызова malloc() для UNIX x64
const char* UNIX_MALLOC_x64 = "\tcall\tmalloc@PLT\n";

// Код функции save_ret
const char* SAVE_RET_x64 = "\
save_ret:\n\
\tpushq\t%rax\n\
\tmovq\t0x4(%rsp), %rax\n\
\taddq\t$0x5, %rax\n\
\tmovq\t%rax, (%rdi, %rsi, 4)\n\
\tpopq\t%rax\n\
\tret\n\
";
