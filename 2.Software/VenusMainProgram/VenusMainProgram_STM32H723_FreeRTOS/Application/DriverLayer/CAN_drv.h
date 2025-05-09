/**
 * @file CAN_drv.h
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
 
 #include "stm32h7xx_hal.h"

void CAN_Init(void);

uint8_t FDCAN2_Send_Msg(uint8_t* msg,uint32_t len);

void FDCAN1_fliter_config(void);
void FDCAN2_fliter_config(void);
 
extern FDCAN_RxHeaderTypeDef fdcan2_RxHeader;
extern FDCAN_TxHeaderTypeDef fdcan2_TxHeader;

extern FDCAN_TxHeaderTypeDef CAN_TxHeader_MI;
extern FDCAN_RxHeaderTypeDef CAN_RxHeader_MI;