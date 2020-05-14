#pragma once

// Код инициализации
const char* INIT_x64 = "\
\tpushq\t$0x%x\n\
%s\
\taddq\t$8, %%rsp\n\
\tmovq\t%%rax, %%r15\n\
\tmovq\t$0, (%%rax)\n\
";

// Код для замены инструкции call
const char* CALL_x64 = "\
\tmovq\t(%r15), %r14\n\
\tincq\t%r14\n\
\tmovq\t%r14, (%r15)\n\
\tpushq\t$0\n\
\tmovq\t%rip, (%r15, %r14, 8)\n\
\tjmp\t\t\
";

// Код для замены инструкции ret
const char* RET_x64 = "\
\tmovq\t(%r15), %r14\n\
\tdecq\t%r14\n\
\tmovq\t%r14, (%r15)\n\
\tincq\t%r14\n\
\taddq\t$0x8, %rsp\n\
\tjmp\t\t*(%r15,%r14,4)\n\
";

// Код вызова malloc() для UNIX x64
const char* UNIX_MALLOC_x64 = "\tcall\tmalloc@PLT\n";

// Код функции save_ret
const char* SAVE_RET_x64 = "\
\t.text\n\
save_ret:\n\
\tpushq\t%rax\n\
\tmovq\t0x8(%rsp), %rax\n\
\taddq\t$0x5, %rax\n\
\tmovq\t%rax, (%r15, %r14, 8)\n\
\tpopq\t%rax\n\
\tret\n\
";
