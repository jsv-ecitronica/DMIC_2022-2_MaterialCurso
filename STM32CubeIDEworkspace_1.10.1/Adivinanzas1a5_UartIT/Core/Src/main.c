/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  * Juego de las adivinanzas
  * Javier Soto - 2022/11/24
  *
  * Ejemplo de como usar la interrupción usando UART, asi como de lectura del
  * SysTick timer para generación de número base para el juego de las adivinanzas.
  * Desarrollado para una tarjeta NUCLE0-F103RB.
  *
  * Caracteristicas adicionales
  * - El sistema genera eco local de los caracteres recibidos
  * - Es necesario que el usuario deshabilite la opcion de eco local en su terminal
  * - Asegurese que su terminal use 'Control-H' (0x08) como 'Backspace'
  * - Se puede usar el Backspace para borrar datos ingresados.
  * - El nombre se tomara automaticamente cuando el buffer de nombre este lleno
  * - El numero tiene un limite maximo de caracteres, no se recibiran mas hasta que se ingrese Enter
  * - Use la tecla 'Esc' para reiniciar la aplicacion
  *
  * Referencias adicionales:
  * - https://www.asciiart.eu/
  * - https://codegolf.stackexchange.com/questions/16587/print-a-smiley-face
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PrintFunctions.h"
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief buffer name
 */
#define BUFFER_NAME_SIZE 20
#define BUFFER_NUMBER_SIZE 10

char bufRx[1];                          //Buffer para recepcion de caracteres por serial
char bufferName[BUFFER_NAME_SIZE];      //Buffer para el nombre del participante
char bufferNumber[BUFFER_NUMBER_SIZE];  //Buffer para el numero
uint8_t indexBuf;                       //indice para llenado de buffers

uint8_t bufferFullFlag;     //Bandera que indica cuando un buffer esta lleno (e.g. se recibio el enter)
uint8_t flagEsc;            //Bandera que indica cuando se recibio la tecla 'Esc'

/**
 * Estados para maquina de estados del MAIN
 */
typedef enum EStatesMain {
    MAIN_INIT_AND_REQUEST_NAME,
    MAIN_WAIT_NAME,
    MAIN_REQUEST_NUMBER,
    MAIN_WAIT_NUMBER,
    MAIN_EXECUTE_RIDDLE
} EStatesMain;
/**
 * Estados para maquina de estados de la interrupción
 */
typedef enum EStatesIRQ {
    IRQ_IDLE,
    IRQ_RX_NAME,
    IRQ_RX_NUMBER
} EStatesIRQ;
enum EStatesIRQ statesIrq = IRQ_IDLE;   //Maquina de estados de la interrupción

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  //HAL_StatusTypeDef UART_Start_Receive_IT(UART_HandleTypeDef *huart2, uint8_t *pData, uint16_t Size);

  HAL_UART_Receive_IT (&huart2, (uint8_t*)bufRx, sizeof(bufRx)); //Habilita IRQ por recepción de la UART

  enum EStatesMain statesMain;      //Maquina de estados del MAIN

  flagEsc = 1; //Emula un reset (ESC) como condicion inicial (inicializacion)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      char buf[100];
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (flagEsc) { //Reiniciar
        memset(bufferName, 0x0, sizeof(bufferName));
        memset(bufferNumber, 0x0, sizeof(bufferNumber));
        flagEsc = 0;
        statesMain = MAIN_INIT_AND_REQUEST_NAME;
        statesIrq = IRQ_IDLE;
        bufferFullFlag = 0;
        indexBuf = 0;

    } else switch (statesMain){

        case MAIN_INIT_AND_REQUEST_NAME:
            printLine("\x1b[2J"); //Clear the screen
            printLine("****************************************************************");
            printLine("* Universidad Escuela Colombiana de Ingenieria Julio Garavito  *");
            printLine("* DMIC - Javier Soto - 2022                                    *");
            printLine("* Juego 'JUGUEMOS A LAS ADIVINANZAS' - UART con Interrupciones *");
            printLine("* Desarrollado para una tarjeta NUCLE0-F103RB                  *");
            printLine("****************************************************************");
            printLine("");
            printLine("- El sistema genera eco local de los caracteres recibidos");
            printLine("- Deshabilite la opcion de eco local en su terminal");
            printLine("- Asegurese que su terminal use 'Control-H' (0x08) como 'Backspace'");
            printLine("- Utilice el Backspace para borrar datos ingresados.");
            printLine("- El nombre se tomara automaticamente cuando el buffer este lleno");
            printLine("- El numero tiene un limite maximo de caracteres");
            printLine("- Use la tecla 'Esc' para reiniciar la aplicacion");
            printLine("");
            printLine("Ingrese su nombre: ");
            statesIrq = IRQ_RX_NAME;
            statesMain = MAIN_WAIT_NAME;
            break;

        case MAIN_WAIT_NAME:
            if (bufferFullFlag) {
                statesMain = MAIN_REQUEST_NUMBER;
                bufferFullFlag = 0;
            }
            break;

        case MAIN_REQUEST_NUMBER:
            memset(bufferNumber, 0x0, sizeof(bufferNumber));
            indexBuf = 0;
            //
            printLine("");
            printLine("***************************************************************");
            snprintf(buf, sizeof(buf), "* %s, Juguemos a las ADIVINANZAS !!", bufferName);
            printLine(buf);
            printLine("***************************************************************");
            printLine("");
            printLine("Ingrese un numero entre 1 y 5, presione enter: ");
            //
            statesIrq = IRQ_RX_NUMBER;
            statesMain = MAIN_WAIT_NUMBER;
            break;

        case MAIN_WAIT_NUMBER:
            if (bufferFullFlag) {
                statesMain = MAIN_EXECUTE_RIDDLE;
                statesIrq = IRQ_IDLE;
                bufferFullFlag = 0;
            }
            break;

        case MAIN_EXECUTE_RIDDLE:
        {
            int numUsuario = atoi((char*)bufferNumber);
            //
            if (numUsuario < 1 || numUsuario > 5) {
                ImprimirError();
                HAL_GPIO_WritePin(LedGanador_GPIO_Port, LedGanador_Pin, GPIO_PIN_RESET); //Poner a 0 Led Ganador
            } else {
                //uint32_t numSistema = (g_lfsrValue % 5) + 1;
                uint32_t ticks = HAL_GetTick();
                uint32_t numSistema = (ticks % 5) + 1;

                printLine("");
                printTextValue("Numero de ticks: ", ticks);
                printTextValue("Numero de usuario: ", numUsuario);
                printTextValue("Numero de sistema: ", numSistema);
                printLine("");
                //
                if (numUsuario == numSistema) {
                    ImprimirMensajeGanador(bufferName);
                    //int sm = 1;
                    //printf("%c ",sm);
                    HAL_GPIO_WritePin(LedGanador_GPIO_Port, LedGanador_Pin, GPIO_PIN_SET); //Poner a 1 Led Ganador
                } else {
                    ImprimirMensajeNoGanador();
                    HAL_GPIO_WritePin(LedGanador_GPIO_Port, LedGanador_Pin, GPIO_PIN_RESET); //Poner a 0 Led Ganador
                }
            }
            statesMain = MAIN_REQUEST_NUMBER;
            break;
        }
    } //switch
    HAL_Delay(10); //Evalua maquina de estados cada 10 m
  }// while

  /* USER CODE END 3 */
} // main

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LedGanador_GPIO_Port, LedGanador_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LedGanador_Pin */
  GPIO_InitStruct.Pin = LedGanador_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LedGanador_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
/**
 *
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    if (huart->Instance == USART2) {
        //
        uint8_t echoFlag = 0;
        uint8_t backspaceFlag = 0;
        //
        if (bufRx[0] == 0x1B) {
            flagEsc = 1;
        } else switch (statesIrq) {

        case IRQ_IDLE:
            //No hace nada, descarta caracter recibido
            break;

            case IRQ_RX_NAME:   //recibiendo nombre
                //Almacenar caracter en buffer
                if (bufRx[0] == 0xA || bufRx[0] == 0xD) { //Enter
                    bufferFullFlag = 1;
                    echoFlag = 1;
                } else if (bufRx[0] == 0x8) { //backspace
                    if (indexBuf > 0) {
                        bufferName[--indexBuf] = '\0';
                        backspaceFlag = 1;
                    }
                } else if (indexBuf < BUFFER_NAME_SIZE - 1) {
                    bufferName[indexBuf++] = bufRx[0];
                    echoFlag = 1;
                }
                //Ejecutar accion si buffer esta lleno
                if (indexBuf >= BUFFER_NAME_SIZE - 1) {
                    bufferFullFlag = 1;
                    printLine("Buffer is full");
                }
                break;


            case IRQ_RX_NUMBER: //recibiendo numero
                //Almacenar caracter en buffer
                if (bufRx[0] == 0xA || bufRx[0] == 0xD) { //Enter
                    bufferFullFlag = 1;
                    echoFlag = 1;
                } else if (bufRx[0] == 0x8) { //backspace
                    if (indexBuf > 0) {
                        bufferNumber[--indexBuf] = '\0';
                        backspaceFlag = 1;
                    }
                }else if (indexBuf < BUFFER_NUMBER_SIZE - 1) {
                    bufferNumber[indexBuf++] = bufRx[0];
                    echoFlag = 1;
                }
                //Borrar caracter si buffer esta lleno
                if (indexBuf >= BUFFER_NAME_SIZE - 1) {
                    bufferNumber[--indexBuf] = '\0';
                    backspaceFlag = 1;
                }
                break;
        }
        if (backspaceFlag) {    //Borrar a caracter recibido
            HAL_UART_Transmit(&huart2, (uint8_t*)"\b \b", 3, 0xFFFF);
        }
        if (echoFlag) {         //Eco a caracter recibido
            HAL_UART_Transmit(&huart2, (uint8_t*)bufRx, sizeof(bufRx), 0xFFFF);
        }
        //Habilitar interrupcion por recepcion
        HAL_UART_Receive_IT(&huart2, (uint8_t*)bufRx, sizeof(bufRx));
    }
}


//void HAL_UARTEx_RxEventCallback()

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
