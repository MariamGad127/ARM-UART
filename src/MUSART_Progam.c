/*
 * MUSART_Program.c
 *
 *  Created on: Mar 3, 2024
 *      Author: M
 */

/************LIBRARIES************/
#include "../include/STD_Types.h"
#include "../include/BIT_MATH.h"

/************MCAL************/
#include "../include/MUSART_Interface.h"
#include "../include/MUSART_Private.h"
#include "../include/MUSART_Config.h"

static void(*MUSART_Callback)(void) = NULL;

void MUSART_void_Init(void) {
	/********************USART1********************/
	// Set oversampling mode
#if MUSART1_OVERSAMPLING_MODE == MUSART_OVER_ON
	USART1->CR1.OVER8 = MUSART_OVER_ON;
#else
	USART1->CR1.OVER8 = MUSART_OVER_OFF;
#endif

	// Set data size
#if MUSART1_DATA_SIZE == MUSART_8_BITS
	USART1->CR1.M = MUSART_8_BITS;
#else
	USART1->CR1.M = MUSART_9_BITS;
#endif

	// Set wake mode
#if MUSART1_WAKE_MODE == MUSART_ADDRESS_MARK
	USART1->CR1.WAKE = MUSART_ADDRESS_MARK;
#else
	USART1->CR1.WAKE = MUSART_IDLE_LINE;
#endif

	// Set parity configurations
#if MUSART1_PARITY_CONTROL == MUSART_PARITY_ENABLED
	USART1->CR1.PCE = MUSART_PARITY_ENABLED;
#else
	USART1->CR1.PCE = MUSART_PARITY_DISABLED;
#endif

	// Set parity selection
#if MUSART1_PARITY_SELECTION == MUSART_PARITY_ODD
	USART1->CR1.PS = MUSART_PARITY_ODD;
#else
	USART1->CR1.PS = MUSART_PARITY_EVEN;
#endif

	// Set baud rate
	u8 localu8_Fraction = 0;
	USART1->BRR = ((u16)(USART_DIV_VALUE(USART1, MUSART1_BAUD_RATE)) << MUSART_MANTISSA_B0);
	localu8_Fraction = ((u8)(USART_DIV_VALUE(USART1, MUSART1_BAUD_RATE) - (u16)(USART_DIV_VALUE(USART1, MUSART1_BAUD_RATE) * 16))) & 0x0F;
	USART1->BRR |= localu8_Fraction << MUSART_FRACTION_B0;

	// Set transmitter mode
#if MUSART1_TRANSMITTER_MODE == MUSART_TRANMITTER_ON
	USART1->CR1.TE = MUSART_TRANMITTER_ON;
#else
	USART1->CR1.TE = MUSART_TRANMITTER_OFF;
#endif
	// Set receiver mode
#if MUSART1_RECEIVER_MODE == MUSART_RECEIVER_ON
	USART1->CR1.RE = MUSART_RECEIVER_ON;
#else
	USART1->CR1.RE = MUSART_RECEIVER_OFF;
#endif



	/*Set Interrupts States*/
#if MUSART1_PARITY_ERROR_INTERRUPT_STATE == MUSART_INTERRUPT_ENABLED
	USART1->CR1.PEIE = 1;
#else
	USART1->CR1.PEIE = 0;
#endif
#if MUSART1_TRANSMIT_REGISTER_EMPTY_INTERRPUT_STATE == MUSART_INTERRUPT_ENABLED
	USART1->CR1.TXEIE = 1;
#else
	USART1->CR1.TXEIE = 0;
#endif
#if MUSART1_TRANSMITTION_COMPLETE_INTERRPUT_STATE == MUSART_INTERRUPT_ENABLED
	USART1->CR1.TCIE = 1;
#else
	USART1->CR1.TCIE = 0;
#endif
#if MUSART1_RECEIVER_REGISTER_NOT_EMPTY_INTERRPUT_STATE == MUSART_INTERRUPT_ENABLED
	USART1->CR1.RXNEIE = 1;
#else
	USART1->CR1.RXNEIE = 0;
#endif
#if MUSART1_IDLE_LINE_INTERRUPT_STATE == MUSART_INTERRUPT_ENABLED
	USART1->CR1.IDLEIE = 1;
#else
	USART1->CR1.IDLEIE = 0;
#endif
	/*Set Send Break Mode*/
#if MUSART1_SEND_BREAK_MODE == MUSART_SEND_BREAK_ENABLED
	USART1->CR1.SBK = 1;
#else
	USART1->CR1.SBK = 0;
#endif
	/********************USART2********************/




	/********************USART6********************/
}

void MUSART_void_Transmit(u8 copyu8_USART_Num, u8 *ptr_Data, u8 copyu8_Length) {
	u8 localu8_Iterator = 0;
	switch(copyu8_USART_Num) {
	case MUSART_USART1:
		for(localu8_Iterator = 0; localu8_Iterator < copyu8_Length; localu8_Iterator++) {
			USART1->DR = ptr_Data[localu8_Iterator];
			while(GET_BIT(USART1->SR, SR_TXE) == USART_NOTSENT);
		}
		break;
	case MUSART_USART2:
		for(localu8_Iterator = 0; localu8_Iterator < copyu8_Length; localu8_Iterator++) {
			USART2->DR = ptr_Data[localu8_Iterator];
			while(GET_BIT(USART2->SR, SR_TXE) == USART_NOTSENT);
		}
		break;
	case MUSART_USART6:
		for(localu8_Iterator = 0; localu8_Iterator < copyu8_Length; localu8_Iterator++) {
			USART6->DR = ptr_Data[localu8_Iterator];
			while(GET_BIT(USART6->SR, SR_TXE) == USART_NOTSENT);
		}
		break;

	default: break;
	}
}

void MUSART_void_Receive(u8 copyu8_USART_Num, u8 *ptr_Return) {
	switch(copyu8_USART_Num) {
	case MUSART_USART1:
		while(GET_BIT(USART1->SR , SR_RXNE) == USART_NOT_RECEIVED);
		*ptr_Return = USART1->DR;
		break;
	case MUSART_USART2:
		while(GET_BIT(USART2->SR , SR_RXNE) == USART_NOT_RECEIVED);
		*ptr_Return = USART2->DR;
		break;
	case MUSART_USART6:
		while(GET_BIT(USART6->SR , SR_RXNE) == USART_NOT_RECEIVED);
		*ptr_Return = USART6->DR;
		break;
	default: break;
	}
}

void MUSART_void_EnableUSART(u8 copyu8_USART_Num) {
	switch(copyu8_USART_Num) {
	case MUSART_USART1:
		USART1->CR1.UE = USART_ON;
		break;

	case MUSART_USART2:
		USART2->CR1.UE = USART_ON;
		break;

	case MUSART_USART6:
		USART6->CR1.UE = USART_ON;
		break;
	default: break;
	}
}

void MUSART_void_DisableUSART(u8 copyu8_USART_Num) {
	switch(copyu8_USART_Num) {
	case MUSART_USART1:
		USART1->CR1.UE = USART_OFF;
		break;

	case MUSART_USART2:
		USART2->CR1.UE = USART_OFF;
		break;

	case MUSART_USART6:
		USART6->CR1.UE = USART_OFF;
		break;
	default: break;
	}
}

void MUSART_void_SetCallback(void(*ptrToFunc)(void)) {
	MUSART_Callback = ptrToFunc;
}

void USART1_IRQHandler(void) {
	MUSART_Callback();
}

