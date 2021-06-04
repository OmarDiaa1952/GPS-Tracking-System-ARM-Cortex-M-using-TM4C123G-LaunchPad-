//UART0
void UART0_Init(unsigned long baudRate);

void UART0_WriteChar(unsigned char ch);

unsigned char UART0_ReadChar(void);

void UART0_WriteString(char *str);

void UART0_ReadString(char *str, char stopCh);

//UART1
void UART1_Init(unsigned long baudRate);

void UART1_WriteChar(unsigned char ch);

unsigned char UART1_ReadChar(void);

void UART1_WriteString(char *str);

void UART1_ReadString(char *str, char stopCh);
