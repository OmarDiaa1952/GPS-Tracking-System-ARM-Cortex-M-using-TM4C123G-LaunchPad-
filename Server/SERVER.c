#include "UART.h"
#include "SERVER.h"
#include "SYSTICK.h"
#include <math.h>

typedef char * string;
void command(char code, char * data){
			char msgCode[3];
			msgCode[0] = '@';
			msgCode[1] = code;
			msgCode[2] = ',';

			UART0_WriteString(msgCode);
			UART0_WriteString(data);
			UART0_WriteString("\n");
}
string toString(double num,int decimalLength)
{
	int i;	
	int j;
	int r;
	int n;
	int m;
	double num_fractions;
	double num_int;
	int count;
	char str[6];
	int arr[10];
	//before decimals
	//int i;
	i = 0;
	if(num < 1.0){return "0.00";}
	num_fractions = num -(int) (num);
	num_int = (int) num;
	while ((9 - (int)(num_int))  <= 0)
	{
		if (i == 0)
		{
			num_int = num_int / pow(10, i);
			arr[i] = (int) (num_int) % 10;
		}
		else
		{
			num_int = num_int / pow(10, 1);
			arr[i] = (int) (num_int) % 10;
		}
		i++;
	} // 2 then 3
	//index of dot
	count = i;
	//int j;
	for (j = 1; j < 5; j++)
	{
		num_fractions = num_fractions * pow(10, 1);
		arr[i] = (int) (num_fractions) % 10; //4 decimals after point
		i++;
	}
	//now arr have 2>>3 then decimal part after count
	//string str;
	//int r;
	//int n;
	n = 0; // index of str
	for (r = (count - 1); r >= 0; r--) // decimal part
	{
		str[n] = (char)arr[r] + 48; //copy
		n++;
	}
	if(n == 0){
		return "0.00";
	}
	str[n] = '.';
	n++;
	//int m;
	for (m = count; m < (count + decimalLength); m++) // decimal part
	{
		if(arr[m] >= 0 && arr[m] <= 9){
			str[n] = (char)arr[m] + 48; //copy
			n++;
		}
	}
	//if(str[0])
	str[n] = '\0';
	return str;
}
