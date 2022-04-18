#define DEBUGPORT	0x00100000

typedef __UINT32_TYPE__ uint32_t;

void putc_debug(char ch)
{
	*((volatile uint32_t*)DEBUGPORT) = ch;
}

int main() {

	char string[12]="Hello World\0";
			
	for (int i=0;i<sizeof(string);i++) {
		putc_debug(string[i]);
	}
}