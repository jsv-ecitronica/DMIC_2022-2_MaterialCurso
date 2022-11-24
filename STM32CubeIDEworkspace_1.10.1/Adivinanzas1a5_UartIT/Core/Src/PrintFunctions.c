/*
 * PrintFunctions.c
 *
 *  Created on: Nov 23, 2022
 *      Author: javie
 */

#include "PrintFunctions.h"
#define BUFFER_SIZE 100

char uartBuf[BUFFER_SIZE];
int uartBufLenght;

extern UART_HandleTypeDef huart2;

void print(char* text) {
    //
    uartBufLenght = snprintf(uartBuf, sizeof(uartBuf), "%s", text);
    HAL_UART_Transmit(&huart2, (uint8_t*)uartBuf, uartBufLenght, 100);
}

void printLine(char* text) {
    //
    uartBufLenght = snprintf(uartBuf, sizeof(uartBuf), "\n\r%s", text);
    HAL_UART_Transmit(&huart2, (uint8_t*)uartBuf, uartBufLenght, 100);
}

void printTextValue(char* text, uint32_t value) {
    //
    uartBufLenght = snprintf(uartBuf, sizeof(uartBuf), "\n\r%s: %u", text, (unsigned int)value);
    HAL_UART_Transmit(&huart2, (uint8_t*)uartBuf, uartBufLenght, 100);
}

void printValue(uint32_t value) {
    //
    uartBufLenght = snprintf(uartBuf, sizeof(uartBuf), " %u", (unsigned int)value);
    HAL_UART_Transmit(&huart2, (uint8_t*)uartBuf, uartBufLenght, 100);
}

void ImprimirMensajeGanador(char* nombre) {

    char buf[100];

    printLine("");
    printLine("                          oooo$$$$$$$$$$$$oooo");
    printLine("                      oo$$$$$$$$$$$$$$$$$$$$$$$$o");
    printLine("                   oo$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o         o$   $$ o$");
    printLine("   o $ oo        o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$o       $$ $$ $$o$");
    printLine("oo $ $ ~$      o$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$o       $$$o$$o$");
    printLine("~$$$$$$o$     o$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$o    $$$$$$$$");
    printLine("  $$$$$$$    $$$$$$$$$$$      $$$$$$$$$$$      $$$$$$$$$$$$$$$$$$$$$$$");
    printLine("  $$$$$$$$$$$$$$$$$$$$$$$    $$$$$$$$$$$$$    $$$$$$$$$$$$$$  ~~~$$$");
    printLine("   ~$$$~~~$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     ~$$$");
    printLine("    $$$   o$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     ~$$$o");
    printLine("   o$$~   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$       $$$o");
    printLine("   $$$    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" "$$$$$$ooooo$$$$o");
    printLine("  o$$$oooo$$$$$  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   o$$$$$$$$$$$$$$$$$");
    printLine("  $$$$$$$$~$$$$   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     $$$$~~~~~~~~");
    printLine(" ~~~~       $$$$    ~$$$$$$$$$$$$$$$$$$$$$$$$$$$$~      o$$$");
    printLine("            ~$$$o     ~~~$$$$$$$$$$$$$$$$$$~$$~         $$$");
    printLine("              $$$o          ~$$~~$$$$$$~~~~           o$$$");
    printLine("               $$$$o                                o$$$~");
    printLine("                ~$$$$o      o$$$$$$o~$$$$o        o$$$$");
    printLine("                  ~$$$$$oo     ~~$$$$o$$$$$o   o$$$$~~");
    printLine("                     ~~$$$$$oooo  ~$$$o$$$$$$$$$~~~");
    printLine("                        ~~$$$$$$$oo $$$$$$$$$$");
    printLine("                                ~~~~$$$$$$$$$$$");
    printLine("                                    $$$$$$$$$$$$");
    printLine("                                     $$$$$$$$$$~");
    printLine("                                      ~$$$~~  ");
    printLine("");
    snprintf(buf, sizeof(buf), "               ¡¡¡ Felicidades %s, adivinaste el numero !!!", nombre);
    printLine(buf);
}

/**
 *
 */
void ImprimirMensajeNoGanador(void){
    printLine("");
    printLine("       ###########");
    printLine("      ##         ##");
    printLine("      #  ~~   ~~  #");
    printLine("      # (*)   (*) #");
    printLine("      (     ^  '  )");
    printLine("       |         |");
    printLine("       |  <===>  |");
    printLine("        \\       /");
    printLine("       /  -----  \\");
    printLine("    ---    \\X/    ---");
    printLine("   /       |x|       \\");
    printLine("  |        |x|        |");
    printLine("");
    printLine("  Lo siento, esta vez no hubo suerte !!!");
}

void ImprimirError (void) {
    printLine("");
    printLine("  _____    _____    _____    _____    _____ ");
    printLine(" |  ___|  |  _  \\  |  _  \\  /     \\  |  _  \\");
    printLine(" |  ___|  |     /  |     /  |  |  |  |     /");
    printLine(" |_____|  |__|__\\  |__|__\\  \\_____/  |__|__\\");
    printLine("");
    printLine("Dato ingresado invalido !!!");
}
