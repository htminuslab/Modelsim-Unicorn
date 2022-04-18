OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)

ENTRY(reset_vec)

MEMORY
{
    MEM (rwx) : ORIGIN = 0x00000000, LENGTH = 0x10000
    IO (rw!x) : ORIGIN = 0x80000000, LENGTH = 0x1000
}
SECTIONS
{
    . = 0;

    .text :
    {
        *(.text)
    } > MEM

    .data :
    {
        *(.rodata*)
        *(.data)
        _global = . + 0x800;
        *(.sbss)
        *(.bss)
 _heap = .;
    } > MEM

    PROVIDE ( _stack = ORIGIN(MEM) + LENGTH(MEM) ); 
}

