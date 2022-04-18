# Executed under Mingw64
# 
OBJS = start.o main.o
CFLAGS = -mabi=ilp32 -march=rv32im -IC:/utils/RISC-V_toolchain/riscv-none-embed/include -ffreestanding -nostdlib
TOOLCHAIN_PREFIX = /opt/riscv-toolchain/bin/riscv32-unknown-elf-

main.bin: main.elf
	$(TOOLCHAIN_PREFIX)objcopy -O binary  $< $@
	$(TOOLCHAIN_PREFIX)size --format=berkeley main.elf

main.elf: $(OBJS) rv32.lds
	$(TOOLCHAIN_PREFIX)gcc $(CFLAGS) -Wl,-T,rv32.lds,-Map,main.map,--strip-debug -o $@ $(OBJS) 
	$(TOOLCHAIN_PREFIX)objdump -drwx -S main.elf > main.asm

%.o: %.c
	$(TOOLCHAIN_PREFIX)gcc -c $(CFLAGS) $<

%.o: %.S
	$(TOOLCHAIN_PREFIX)gcc -c $(CFLAGS) $<

main.o: CFLAGS += -Wno-implicit-int -Wno-implicit-function-declaration

clean:
	rm -Rf *.o *.d main.elf main.map main.bin main.asm

.PHONY: test clean

-include *.d

