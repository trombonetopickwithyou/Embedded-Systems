/*
 * main.c
 */
int main(void) {
	int x = 5;

volatile char *ptr;
char data;

ptr = (volatile char *) 0x90080000;

for(;;)
{
	if(Glo.flag == 1)
		data = *ptr;
}


	return x;
}
