#include "TM4C123.h"                    // Device header
#include "bsp.h"
#include <stdint.h>

int main() {
	
	setupPeripherals();
	
	printBTString("Smart House Console\n\r");
	printBTString("Type \"r\" or \"g\" to turn on the LEDs\n\r");
	
	while(1);
	
}

