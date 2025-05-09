/**
 * @file TIM_drv.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
 
 
#include "stm32h7xx_hal.h"
#include "tim.h"
#include "TIM_drv.h"


void TIM_Init(void)
{

    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);    //Buzzer
    
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);	//LED_R	
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);    //LED_G
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);    //LED_B

	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);    //LED_B

	
    __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 2000);
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 2000);
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 2000);    
    
}


 
void BuzzerWarning(void)
{

    static uint32_t currentMillis, previousMillisA0, previousMillisA1;
    static uint16_t OnceFlag = 0, LEDFlag = 0;
    uint16_t freq = 1000;     

    currentMillis = HAL_GetTick(); //��ȡ��ǰϵͳʱ��    

    
//        if (currentMillis - previousMillisA0 >= 1000) //��ǰʱ��̼�ȥǰ��ִ�е�ʱ���
//        {
//            previousMillisA0 = currentMillis; //����ִ��ʱ���
//            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
//        }
//        if (currentMillis - previousMillisA1 >= 500) //��ǰʱ��̼�ȥǰ��ִ�е�ʱ���
//        {
//            previousMillisA1 = currentMillis; //����ִ��ʱ���
//            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
//        }

    
//������CH1 ���CH2 �ϵ羯������(��)��֮��ÿ������һ��(��)
        if(OnceFlag == 0){
          for(int i=1;i<4; i++){
            __HAL_TIM_SET_AUTORELOAD(&htim8, freq);

            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 50);
            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 0);
            HAL_Delay(100);
              
            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0);
            __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_2, 2000);
            HAL_Delay(100); 
          }
          OnceFlag = 1;
        }
        else if( (OnceFlag == 1)&(currentMillis - previousMillisA0 >= 500) ){
            previousMillisA0 = currentMillis; //����ִ��ʱ���
            if(LEDFlag == 0){
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
            LEDFlag = 1;
            }
            else{
            __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 2000);
            LEDFlag = 0;
            }
        }

}

