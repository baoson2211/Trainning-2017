#include <stdio.h>

#include "stm8s.h"
#include "stm8s_it.h"
#include "stm8s_itc.h"
#include "stm8s_gpio.h"
#include "stm8s_clk.h"

#include "uart.h"
#include "tim.h"
#include "DS18x20.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO extern uint8_t UART_Recv;

/* Private function prototypes -----------------------------------------------*/
int CLK_config(void);
int ITC_Config(void);
// #ifdef __SDCC__
// void print_float(char s[], long temp);
// #endif

/* Private functions ---------------------------------------------------------*/
static void delay(uint32_t t)
{
    while(t--);
}

int CLK_config(void)
{
    /* De-initialized clock control */
    CLK_DeInit();
    /* Enable HSE */
    CLK_HSECmd(ENABLE);
    /* Wait until HSE has been ready */
    while(CLK_GetFlagStatus(CLK_FLAG_HSERDY) != SET);
    /* Switch clock source to HSE and disable HSI (Auto mode) */
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

    /* Wait until HSE is chosen as SYSCLK Source */
    while(CLK_GetSYSCLKSource() != CLK_SOURCE_HSE);
    /* Stop clock switch */
    CLK_ClockSwitchCmd(DISABLE);
    /* Prescale of SYSCLK Source as 1 (not divided) */
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);

    return 0;
}

int ITC_Config(void) {
    UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
    enableInterrupts();

    return 0;
}

// #ifdef __SDCC__
// void print_float(char s[], long temp) {
//     uint8_t a,b,c,d;
//
//     a =(temp%10000)/1000;
//     b =((temp%10000)%1000)/100;
//     c =(((temp%10000)%1000)%100)/10;
//     d =(((temp%10000)%1000)%100)%10;
//     sprintf(s, "Temp: %ld.%d%d%d%d Celsius\r\n",(temp/10000),a,b,c,d);
// }
// #endif

int main( void )
{
    char s[30];    // final string
    uint32_t clk;  // clock freq
    long temp;     // temperature

    /* config clock source as HSE = 8 MHz
     * defaut: if you dont call this func, MCU'll run with HSI/8
     * See more in Reference manual (en.CD00190271) page 84 figure 22
     */
    while(CLK_config());
    /* Led as pin 7 of GPIOD */
    GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_Init(ONEWIREBUS_PORT, ONEWIREBUS_PIN, GPIO_MODE_OUT_OD_HIZ_FAST);
    /* UART config */
    while(UART2_config());
    /* current CPU freq */
    clk = CLK_GetClockFreq();
    /* Set pin 7 of port D */

    // sendUartFn("Start\r\n");

    while(1) {
        /* write to string */
        temp = DS18x20_ReadTemp();
        sprintf(s, "*%08ld#", temp);
        sendUartFn(s);
        delay_ms(5000);
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    (void)file;
    (void)line;
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif
