
gcc -g -Wall -c -m64 -Idisasm ../disasm/riscv-disas.c
gcc -g -Wall -c -m64 -I%MTI_HOME%/include -I../src -I../disasm -I../include ../src/riscv_emu.c
gcc -g -Wall -c -m64 -I%MTI_HOME%/include -I../src -I../disasm -I../include ../src/fli_riscv.c
gcc -g -shared fli_riscv.o riscv_emu.o riscv-disas.o  -L%MTI_HOME%/win64pe -lmtipli -L./ -lunicorn -lm -m64 -lpthread -o fli_riscv.dll
