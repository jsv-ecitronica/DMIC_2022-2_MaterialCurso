/*
 * PrintFunctions.h
 *
 *  Created on: Nov 23, 2022
 *      Author: javie
 */

#ifndef INC_PRINTFUNCTIONS_H_
#define INC_PRINTFUNCTIONS_H_

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>

void print(char* text);
void printLine(char* text);
void printTextValue(char* text, uint32_t value);
void printValue(uint32_t value);

void ImprimirMensajeGanador(char* nombre);
void ImprimirMensajeNoGanador(void);
void ImprimirError (void);

#endif /* INC_PRINTFUNCTIONS_H_ */
