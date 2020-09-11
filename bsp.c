/* Board Support Package */
#include "TM4C123.h"                    // Device header
#include "TM4C123GH6PM.h"
#include "bsp.h"

uint8_t value;
char x;
//__attribute__((naked)) void assert_failed (char const *file, int line) {
    /* TBD: damage control */
//    NVIC_SystemReset(); /* reset the system */
//}

void setupPeripherals(void) {
	/*Set up all necessary peripherals: UARTs, Timers, ADC and GPIOs*/
	
	/*Enable clock to GPIOA, GPIOF ports and UART0 module*/
	SYSCTL->RCGCUART |= UART_CFG;
	SYSCTL->RCGCTIMER |= TIMER_CFG;
	SYSCTL->RCGCADC |= ADC_CFG;
	SYSCTL->RCGCGPIO |= GPIO_CFG;
	
	/*Configure pins PA0 and PA1 for alternate digital functions (UART RX and TX pins)*/
	GPIOA->AFSEL |= UART0_TX_RX;
	GPIOA->AMSEL &= ~UART0_TX_RX;
	GPIOA->PCTL = UART0_PTCL;
	GPIOA->DEN |= UART0_TX_RX;
	
	/*Configure pins for UART2 RX and TX (Bluetooth connection)*/
	GPIOD->LOCK = LOCK_KEY;
	GPIOD->CR = CR_UNLOCK;
	GPIOD->AFSEL |= UART2_TX_RX;
	GPIOD->AMSEL &= ~UART2_TX_RX;
	GPIOD->PCTL = UART2_PTCL;
	GPIOD->DEN |= UART2_TX_RX;
	
	/*Set up PE3 as analog input */
	GPIOE->AFSEL |= ADC_IN;   //enable alternate function for PE3
	GPIOE->DEN &= ~ADC_IN;
	GPIOE->AMSEL |= ADC_IN;
	
	/*Set up the RGB LEDs*/
	GPIOF->DIR |= LED_RED|LED_BLUE|LED_GREEN;
	GPIOF->DEN |= LED_RED|LED_BLUE|LED_GREEN;
	GPIOF->DATA &= ~LED_RED|LED_BLUE|LED_GREEN;
	
	/*Set up SysTick Timer*/
	SysTick->LOAD = SYSTICK_LOAD;
	SysTick->VAL  = 0U;
	SysTick->CTRL = SYSTICK_EN;
	
	/*Timer0A and Timer0B Setup */
	TIMER0->CTL &= ~(1U << 0);
	TIMER0->CFG = (0x0 << 0);
	TIMER0->TAMR |= 0x02;
	TIMER0->TAILR = TIMER_LOAD;
	TIMER0->IMR = 0x0;
	TIMER0->CTL |= TIMER_EN;
	
	/* ADC0 Config */
	ADC0->ACTSS &= ~SS3;  //disable Sample Sequencer 3 for configuration
	ADC0->EMUX &= ~(0xF << 12);
	ADC0->EMUX |= (0x5 << 12);   //set SS3 to sample on timer interrupt event
	ADC0->SSMUX3 = 0;
	ADC0->SSCTL3 |= ((1U << 1) | (1U << 2));
	ADC0->IM |= SS3;
	ADC0->ACTSS |= SS3;
	
	/*Configure UART0 with following properties: Baud rate of 9600; 8-bit data; No parity, 1 stop bit; system clock as source*/
	UART0->CTL &= ~(1 << 0);
	UART0->IBRD = BAUDINT_9600;
	UART0->FBRD = BAUDFR_9600;
	UART0->LCRH = (0x3 << 5);
	UART0->CC = 0x05;
	UART0->CTL = UART_EN;
	
	/*Configure UART2 with properties: Baud Rate: 9600; Data: 8-bit; No Parity, 1 stop bit; System clock as source*/
	UART2->CTL &= ~(1 << 0);
	UART2->IBRD = BAUDINT_9600;
	UART2->FBRD = BAUDFR_9600;
	UART2->LCRH = (0x3 << 5);
	UART2->IM |= (1 << 4);
	UART2->CC = 0x05;
	UART2->CTL = UART_EN;
	
	/*Set up ADC0 Seq3 Interrupts in the NVIC*/
	NVIC->IP[17] |= 0x6000;
	NVIC->ISER[0] |= (1 << 17);
	
	/*Set up UART2 Interrupts in the NVIC*/
	NVIC->IP[33] |= 0x4000;
	NVIC->ISER[1] |= (1 << 1);
	
	/*Enable Global Interrupts*/
	__enable_irq();
	
}

void SysTick_Handler(void) {
	//disable interrupts to avoid race conditions and re-enable after instructions have been carried out
		__disable_irq();
    GPIOF->DATA ^= LED_BLUE;
		__enable_irq();
}

void ADC0SS3_Handler(void) {
	printString("Resistance Value: ");
	printBTString("Resistance Value: ");
	value = (ADC0->SSFIFO3)*100/4096;
	printChar(value);
	printString("\n\r");
	printBTInt(value);
	printBTString("\n\r");
	ADC0->ISC |= (1 << 3);
}

void UART2_Handler(void) {
	if((UART2->RIS & (1 << 4)) || (UART2->RIS & (1 << 6))) {
		x = UART2->DR;
		switch(x) {
			case 'r':
				GPIOF->DATA = LED_RED;
				break;
			case 'g':
				GPIOF->DATA = LED_GREEN;
				break;
			default:
				GPIOF->DATA &= ~(LED_RED|LED_GREEN);
				break;
		}
		UART2->ICR |= (1 << 4);
	}
}

char readChar(void) {
	//read 8-bit data from the UART data register
	char dat;
	while((UART0->FR & (1 << 4)) != 0);
	dat = UART0->DR;
	return dat;
}

void printChar(char c) {
	//print the read data on the terminal
	while((UART0->FR & (1 << 5)) != 0);
	UART0->DR = c;
}

void printString(char *string) {
	//print string of 8-bit data on terminal
	while(*string) {
		printChar(*(string++));
	}
}

char readBTChar(void) {
	char dat;
	while((UART2->FR & (1 << 4)) != 0);
	dat = UART2->DR;
	return dat;
}

void printBTChar(char c) {
	//print the read data on the Bluetooth terminal
	while((UART2->FR & (1 << 5)) != 0);
	UART2->DR = c;
}

void printBTInt(uint8_t c) {
	char temp[3];
	int i = 2;
	while(i >= 0) {
		if((c%10)!= 0){
			temp[i] = ((c % 10)+'0');
		}
		else {
			temp[i] = '0';
		}
		c = c/10;
		i--;
	}
	printBTString(temp);
}

void printBTString(char *string) {
	//print string of 8-bit data on Bluetooth terminal
	while(*string) {
		printBTChar(*(string++));
	}
}

