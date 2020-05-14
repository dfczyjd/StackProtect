#pragma once

// Код инициализации
const char* INIT_x86 = "\
\tpushl\t$0x%x\n\
%s\
\taddl\t$4, %%esp\n\
\tmovl\t%%eax, %%edi\n\
\tmovl\t$0, (%%eax)\n\
";

// Код для замены инструкции call
const char* CALL_x86 = "\
\tmovl\t(%edi), %esi\n\
\tincl\t%esi\n\
\tmovl\t%esi, (%edi)\n\
\tpushl\t$0\n\
\tcall\tsave_ret\n\
\tjmp\t\t\
";

// Код для замены инструкции ret
const char* RET_x86 = "\
\tmovl\t(%edi), %esi\n\
\tdecl\t%esi\n\
\tmovl\t%esi, (%edi)\n\
\tincl\t%esi\n\
\taddl\t$0x4, %esp\n\
\tjmp\t\t*(%edi,%esi,4)\n\
";

// Код вызова malloc() для UNIX x86
const char* UNIX_MALLOC_x86 = "\
\tcall\t__x86.get_pc_thunk.bx\n\
\taddl\t$_GLOBAL_OFFSET_TABLE_, %ebx\n\
\tcall\tmalloc@PLT\n";

// Код вызова malloc() для Windows
const char* WIN_MALLOC = "\tcall\t_malloc\n";

// Код функции save_ret
const char* SAVE_RET_x86 = "\
\t.text\n\
save_ret:\n\
\tpushl\t%eax\n\
\tmovl\t0x4(%esp), %eax\n\
\taddl\t$0x5, %eax\n\
\tmovl\t%eax, (%edi, %esi, 4)\n\
\tpopl\t%eax\n\
\tret\n\
";
