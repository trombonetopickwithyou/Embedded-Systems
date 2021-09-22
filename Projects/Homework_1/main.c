/*
ECE 5380-001
HW1
Authors: 	Connor Collins
			Joshua Williams

Description: This code interfaces with the
Spectrum Digital TMS320C6713 DSK Board, and lights up LEDs D7-D10
correponding to switches SW1.
*/

int main(void)
{
	volatile char* ptr;
	ptr=(volatile char*) 0x90080000;

	//ssss xxxx -> 0000 ssss
	for(;;)
		*ptr = *ptr >> 4;

	return 0;
}
