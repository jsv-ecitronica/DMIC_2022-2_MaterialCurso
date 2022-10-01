Instrucciones para Implementar el juego de as adivinanzas. Para este ejemplo se utilizo la tarjeta NucleoF103RB.

1- Cree un proyecto con la tarjeta correspondiente. Inicie el proyecto con la configuracion por defecto. 

2- Cambie la etiqueta del led (o uno de los leds, dependiendo de la tarjeta que use) a "LedGanador". El resto de periféricos usará la configuración por defecto, sin embargo asegurese de que el SysTick (tiempo base) y que la USART2 (115200-8N1) estén activas.   

3- El foro de Shawn Hymel presenta el código que se usará en este proyecto. Siga las instrucciones de dicho foro. A continuación se presentan las instrucciones de manera resumida:

>> Deshabilite el archivo "syscalls.c": Click derecho sobre el archivo -> Properties -> C/C++ Build -> Seleccione "Exclude Resources form build" -> Apply and Close.

>> Cree el archivo "retarget.h" en la carpeta "Inc" (Includes), copie el código correspondiente del foro o copielo/importelo de este proyecto.

>> Cree el archivo "retarget.c" en la carpeta "Src" (Soures), copie el código correspondiente del foro o copielo/importelo de este proyecto.

4- Cree o importe los archivos "funciones.c" y "funciones.h" en las carpetas correspondientes.

5- Copie y pegue las secciones de codigo correspondientes desde el archivo "main.c"
 
*******************************************************************************
Nota: dependiendeo del dispositivo (microcontrolador/tarjeta nucleo) que esté usando, es posible que deba realizar las siguientes modificaciones en el código:

>> Referenciar una uart diferente para la función "RetargetInit". Para este ejemplo se utilizó la uart2 (huart2).

>> Especificar la libreria HAL del dispositivo correspondiente en el archivo "retarget.h". Para este ejemplo se utilizó: #include "stm32f1xx_hal.h". La ruta de acceso para verificar el nombre de la librería en este caso es: "\Drivers\STM32F1xx_HAL_Driver\Inc\stm32f1xx_hal.h"

  