#include "TM4C123GH6PM.h"



//*****************************************************************************
//
// UART0
//
//*****************************************************************************
void UART0_Init(unsigned long baudRate){
	unsigned long uartIBRD, uartFBRD;
	double CLDIV = (16000000.0 / (16.0 * baudRate));
	
	uartIBRD = (int)(CLDIV);
	uartFBRD = (int)((CLDIV - uartIBRD) * 64 + 0.5);
	
	SET(SYSCTL_RCGCUART_R,0);
	SET(SYSCTL_RCGCGPIO_R,0);
	while((SYSCTL_PRGPIO_R & 0x01) == 0){}
	CLEAR(UART0_CTL_R,0);
	UART0_IBRD_R = uartIBRD;
	UART0_FBRD_R = uartFBRD;
	UART0_LCRH_R |= 0x70;
	UART0_CTL_R |= 0x301;
	GPIO_PORTA_DEN_R |= 0x03;
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_AMSEL_R &= ~0x03;
	GPIO_PORTA_PCTL_R |= 0x00000011;
	
}

void UART0_WriteChar(unsigned char ch){						//Write Char
	while((UART0_FR_R&TXFULL) != 0){}
	UART0_DR_R = ch;
}

unsigned char UART0_ReadChar(void){								//Read Char
	while((UART0_FR_R&RXEMPTY) != 0){}
	return UART0_DR_R&0xFF;
}

void UART0_WriteString(char *str){								//Write String
  while(*str){
    UART0_WriteChar(*str);
    str++;
  }
}

void UART0_ReadString(char *str, char stopCh){		//Read String
	char c = UART0_ReadChar();
	while(c && c != stopCh){
		*str = c;
		str++;
		c = UART0_ReadChar();
	}
	*str = 0x00;
}

//*****************************************************************************
//
// UART1
//
//*****************************************************************************

void UART1_Init(unsigned long baudRate){
	unsigned long uartIBRD, uartFBRD;
	double CLDIV = (16000000.0 / (16.0 * baudRate));
	
	uartIBRD = (int)(CLDIV);
	uartFBRD = (int)((CLDIV - uartIBRD) * 64 + 0.5);
	
	SET(SYSCTL_RCGCUART_R,1);
	SET(SYSCTL_RCGCGPIO_R,1);
	while((SYSCTL_PRGPIO_R & 0x02) == 0){}
	CLEAR(UART1_CTL_R,0);
	UART1_IBRD_R = uartIBRD;
	UART1_FBRD_R = uartFBRD;
	UART1_LCRH_R |= 0x70;
	UART1_CTL_R |= 0x301;
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_AMSEL_R &= ~0x03;
	GPIO_PORTB_PCTL_R |= 0x00000011;
	
}
void UART1_WriteChar(unsigned char ch){						//Write Char
	while((UART1_FR_R&TXFULL) != 0){}
	UART1_DR_R = ch;
}

unsigned char UART1_ReadChar(void){								//Read Char
	while((UART1_FR_R&RXEMPTY) != 0){}
	return UART1_DR_R&0xFF;
}

void UART1_WriteString(char *str){								//Write String
  while(*str){
    UART1_WriteChar(*str);
    str++;
  }
}

void UART1_ReadString(char *str, char stopCh){		//Read String
	char c = UART1_ReadChar();
	while(c && c != stopCh){
		*str = c;
		str++;
		c = UART1_ReadChar();
	}
	*str = 0x00;
}

