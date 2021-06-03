#include "tm4c123gh6pm.h"


#define LCD_RS (*((volatile unsigned long *)0x40004200))    //PA.7 for register select pin
#define LCD_EN  (*((volatile unsigned long *)0x40004100))   //PA.6 for enable pin
#define LCD_RW  (*((volatile unsigned long *)0x40004080))   //PA.5 for rw pin
#define clear_display     0x01
#define returnHome        0x02
#define moveCursorRight   0x06
#define moveCursorLeft    0x08
#define Shift_Cursor_Left 0x10
#define shiftDisplayRight 0x1C
#define shiftDisplayLeft  0x18
#define cursorBlink       0x0F
#define cursorOff         0x0C
#define cursorOn          0x0E
#define Function_set_4bit 0x28
#define Function_set_8bit 0x38
#define Entry_mode        0x06
#define Function_8_bit    0x32
#define Set5x7FontSize    0x20
#define FirstRow          0x80
#define SecondRow         0xC0 
void LCD_INIT(void);
void LCD_CMD(unsigned long cmd);
void LCD_WRITE (char data);
void Delay(int frac);
void hundredMicroSecounds(void);
void LCD_String(char *str);
