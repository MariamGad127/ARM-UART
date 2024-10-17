/*
 * MUSART_Interface.h
 *
 *  Created on: Mar 3, 2024
 *      Author: M
 */

#ifndef MUSART_INTERFACE_H_
#define MUSART_INTERFACE_H_


#define MUSART_USART1		1
#define MUSART_USART2		2
#define MUSART_USART6		6


void MUSART_void_Init(void);
void MUSART_void_Transmit(u8 copyu8_USART_Num, u8 *ptr_Data, u8 copyu8_Length);
void MUSART_void_Receive(u8 copyu8_USART_Num, u8 *ptr_Return);
void MUSART_void_EnableUSART(u8 copyu8_USART_Num);
void MUSART_void_DisableUSART(u8 copyu8_USART_Num);
void MUSART_void_SetCallback(void(*ptrToFunc)(void));



#endif /* MUSART_INTERFACE_H_ */
