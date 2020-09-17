/*------------------------------------TM4C123 Cortex-M4F Project----------------------------------------
Version: 1.1
Author(s): Damola Awesu; Vinay Garade
File Description: Contains the main() function for the project

------------------------------------------------------------------------------------------------------*/

#include "TM4C123.h"                    // Device header
#include "bsp.h"
#include <stdint.h>

int main() {
	
	setupPeripherals();
	
	printf("Test Setup");
	
	while(1);
	
}
