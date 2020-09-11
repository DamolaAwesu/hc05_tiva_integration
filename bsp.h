#ifndef __BSP_H__
#define __BSP_H__

/* Board Support Package for the EK-TM4C123GXL board */

/* system values */
#define SYS_CLOCK_HZ 	16000000U
#define SYSTICK_LOAD	SYS_CLOCK_HZ/2U - 1U
#define SYSTICK_EN		(1U << 2) | (1U << 1) | 1U
#define GPIO_CFG			(1 << 0)|(1 << 5)|(1 << 4)|(1 << 3)
#define TIMER_CFG			(1 << 0)
#define ADC_CFG				(1 << 0)
#define UART_CFG			(1 << 0)|(1 << 2)
#define LOCK_KEY			0x4C4F434B
#define CR_UNLOCK			0xFF

#define TIMER_LOAD		SYS_CLOCK_HZ*25 - 1
#define TIMER_EN			(1U << 5) | (1U << 0)

#define BAUDINT_9600	104
#define BAUDFR_9600		11
#define UART0_TX_RX		0x03
#define UART0_PTCL		0x11
#define UART2_TX_RX 	0xC0
#define UART2_PTCL		0x11000000
#define UART_EN				(1<<9)|(1<<8)|(1<<0)

#define ADC_IN				(1U << 3)
#define SS3						(1U << 3)

/* on-board LEDs */
#define LED_RED   		(1U << 1)
#define LED_BLUE  		(1U << 2)
#define LED_GREEN 		(1U << 3)


/*Function Declarations*/
void setupPeripherals(void);
char readChar(void);
void printChar(char c);
void printString( char *string);
char readBTChar(void);
void printBTChar(char c);
void printBTInt(uint8_t c);
void printBTString(char *string);

#endif // __BSP_H__
