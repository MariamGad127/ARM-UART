
/************LIBRARIES************/
#include "../include/BIT_MATH.h"
#include "../include/STD_Types.h"

/************MCAL************/
#include "../include/MRCC_Interface.h"
#include "../include/MGPIO_Interface.h"
#include "../include/MNVIC_Interface.h"
#include "../include/MEXTI_Interface.h"
#include "../include/MSTK_Interface.h"
#include "../include/MUSART_Interface.h"


void handler_USART(void);
u8 Global_u8RX ;
u8 led_r_value = 0;
u8 led_g_value = 0;
u8 led_b_value = 0;

void main(void) {

	/****************************************************************/
	/*             		  1- CLOCK GATING							*/
	/****************************************************************/
	/*Initialize RCC module*/
	MRCC_void_Init();
	/*Enable clock to GPIOA*/
	MRCC_void_EnablePeripheral(RCC_AHB1,RCC_AHB1_GPIOA);
	/*Enable clock to USART1*/
	MRCC_void_EnablePeripheral(RCC_APB2,RCC_APB2_USART1);


	/****************************************************************/
	/*             		  2- PINS SETUP 							*/
	/****************************************************************/
	/*Configure Pins 9 & 10 as alternative function -> USART1*/
	MGPIO_void_SetPinMode(GPIO_PORTA,GPIO_PIN9,GPIO_ALTERNATE);
	MGPIO_void_SetPinMode(GPIO_PORTA,GPIO_PIN10,GPIO_ALTERNATE);
	/*Setup the pins of the alternative functions*/
	MGPIO_void_SetAltFunction(GPIO_PORTA,GPIO_PIN9,0b0111);
	MGPIO_void_SetAltFunction(GPIO_PORTA,GPIO_PIN10,0b0111);

	/*Configure pin0 as output*/
	MGPIO_void_SetPinMode(GPIO_PORTA,GPIO_PIN0,GPIO_OUTPUT);
	/*Setup pin0 with the suitable setting*/
	MGPIO_void_SetPinOutputMode(GPIO_PORTA,GPIO_PIN0,GPIO_PUSH_PULL,GPIO_MEDIUM_SPEED);

	/*Configure pin1 as output*/
	MGPIO_void_SetPinMode(GPIO_PORTA,GPIO_PIN1,GPIO_OUTPUT);
	/*Setup pin0 with the suitable setting*/
	MGPIO_void_SetPinOutputMode(GPIO_PORTA,GPIO_PIN1,GPIO_PUSH_PULL,GPIO_MEDIUM_SPEED);

	/*Configure pin2 as output*/
	MGPIO_void_SetPinMode(GPIO_PORTA,GPIO_PIN2,GPIO_OUTPUT);
	/*Setup pin0 with the suitable setting*/
	MGPIO_void_SetPinOutputMode(GPIO_PORTA,GPIO_PIN2,GPIO_PUSH_PULL,GPIO_MEDIUM_SPEED);
	/****************************************************************/
	/*         	   3- ENABLE USART INTERRUPT						*/
	/****************************************************************/
	/*Enable Peripheral number of USART1*/
	MNVIC_void_EnablePeripheralInterrupt(37);
	/*callBack function when the interrupt fires goes to*/
	MUSART_void_SetCallback(handler_USART);

	/****************************************************************/
	/*             	 4- USART INITIALIZATION						*/
	/****************************************************************/
	MUSART_void_Init();
	MUSART_void_EnableUSART(MUSART_USART1);


	/****************************************************************/
	/*               5- DATA TRANSMISSION   						*/
	/****************************************************************/
	MUSART_void_Transmit(MUSART_USART1, (u8*)"Press 'r' or 'g' or 'b'\n", 23);

	while(1);


}


void handler_USART(void)
{
	MUSART_void_Receive(MUSART_USART1 ,&Global_u8RX);
	if(Global_u8RX == 'r')
	{

		if(led_r_value == 0)
		{
			MUSART_void_Transmit(MUSART_USART1,(u8*)"red_on",7);
			led_r_value = led_r_value ^ 1;
			MGPIO_void_SetPinValue(GPIO_PORTA,GPIO_PIN0,led_r_value);
		}
		else{
			MUSART_void_Transmit(MUSART_USART1,(u8*)"red_off",8);
			led_r_value = led_r_value ^ 1;
			MGPIO_void_SetPinValue(GPIO_PORTA,GPIO_PIN0,led_r_value);
		}
	}
	else if(Global_u8RX == 'g')
		{

			if(led_g_value == 0)
			{
				MUSART_void_Transmit(MUSART_USART1,(u8*)"green_on",9);
				led_g_value = led_g_value ^ 1;
				MGPIO_void_SetPinValue(GPIO_PORTA,GPIO_PIN1,led_g_value);
			}
			else{
				MUSART_void_Transmit(MUSART_USART1,(u8*)"green_off",10);
				led_g_value = led_g_value ^ 1;
				MGPIO_void_SetPinValue(GPIO_PORTA,GPIO_PIN1,led_g_value);
			}
		}
	else if(Global_u8RX == 'b')
		{

			if(led_b_value == 0)
			{
				MUSART_void_Transmit(MUSART_USART1,(u8*)"blue_on",8);
				led_b_value = led_b_value ^ 1;
				MGPIO_void_SetPinValue(GPIO_PORTA,GPIO_PIN2,led_b_value);
			}
			else{
				MUSART_void_Transmit(MUSART_USART1,(u8*)"blue_off",9);
				led_b_value = led_b_value ^ 1;
				MGPIO_void_SetPinValue(GPIO_PORTA,GPIO_PIN2,led_b_value);
			}
		}
	else if (Global_u8RX == 'o')
	    {
	        MUSART_void_Transmit(MUSART_USART1, (u8*)"all_on", 7);

	        led_r_value = 1;
	        led_g_value = 1;
	        led_b_value = 1;

	        MGPIO_void_SetPinValue(GPIO_PORTA, GPIO_PIN0, led_r_value);
	        MGPIO_void_SetPinValue(GPIO_PORTA, GPIO_PIN1, led_g_value);
	        MGPIO_void_SetPinValue(GPIO_PORTA, GPIO_PIN2, led_b_value);
	    }
	else
	    {
			MUSART_void_Transmit(MUSART_USART1, (u8*)"all_off", 8);

	        led_r_value = 0;
	        led_g_value = 0;
	        led_b_value = 0;

	        MGPIO_void_SetPinValue(GPIO_PORTA, GPIO_PIN0, led_r_value);
	        MGPIO_void_SetPinValue(GPIO_PORTA, GPIO_PIN1, led_g_value);
	        MGPIO_void_SetPinValue(GPIO_PORTA, GPIO_PIN2, led_b_value);
	    }

}
