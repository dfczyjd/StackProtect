#pragma once

// Код инициализации
const char* INIT_x86 = "\
\tmovl\t$0x%x, %%edx\n\
%s\
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
\tsubl\t$0x4, %esp\n\
\tjmp\t\t*(%edi,%esi,4)\n\
";

// Код функции save_ret
const char* SAVE_RET_x86 = "\
save_ret:\n\
\tpushl\t%eax\n\
\tmovl\t0x4(%esp), %eax\n\
\taddl\t$0x5, %eax\n\
\tmovl\t%eax, (%edi, %esi, 4)\n\
\tpopl\t%eax\n\
\tret\n\
";
