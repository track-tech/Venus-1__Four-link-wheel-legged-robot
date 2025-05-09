/**
 * @file CAN_drv.c
 * @author __TRACKER (zhangbanzhuan@foxmail.com)
 * @brief 外设驱动
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "CAN_drv.h"
#include "fdcan.h"

/* USER CODE BEGIN 0 */

#include "MI_Motor_dev.h"
#include "DJI_Motor_dev.h"
#include "Bsp_usart_fputc.h"

FDCAN_RxHeaderTypeDef fdcan2_RxHeader;
FDCAN_TxHeaderTypeDef fdcan2_TxHeader;

FDCAN_TxHeaderTypeDef CAN_TxHeader_MI;
FDCAN_RxHeaderTypeDef CAN_RxHeader_MI;


void CAN_Init(void)
{
    
    HAL_FDCAN_Start(&hfdcan1);          //大坑，务必记得启动--fdcan1对接cybergear
    HAL_FDCAN_Start(&hfdcan2);          //大坑，务必记得启动--fdcan2对接C610

    FDCAN1_fliter_config();
    FDCAN2_fliter_config();

}


//can发送一组数据(固定格式:ID为0x200,标准帧,数据帧)	
//len:数据长度(最大为8),可设置为FDCAN_DLC_BYTES_2~FDCAN_DLC_BYTES_8				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
uint8_t FDCAN2_Send_Msg(uint8_t* msg,uint32_t len)
{	
    fdcan2_TxHeader.Identifier=CAN_2006Moto_ALL_ID;            //32位ID
    fdcan2_TxHeader.IdType=FDCAN_STANDARD_ID;                  //标准ID
    fdcan2_TxHeader.TxFrameType=FDCAN_DATA_FRAME;              //数据帧
    fdcan2_TxHeader.DataLength=len;                            //数据长度
    fdcan2_TxHeader.ErrorStateIndicator=FDCAN_ESI_ACTIVE;            
    fdcan2_TxHeader.BitRateSwitch=FDCAN_BRS_OFF;               //关闭速率切换
    fdcan2_TxHeader.FDFormat=FDCAN_CLASSIC_CAN;                //传统的CAN模式
    fdcan2_TxHeader.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     //无发送事件
    fdcan2_TxHeader.MessageMarker=0;                           
    
    if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2,&fdcan2_TxHeader,msg)!=HAL_OK) return 1;//发送
    return 0;	
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//其他,接收的数据长度;
uint8_t FDCAN2_Receive_Msg(uint8_t *buf, uint16_t *Identifier)
{	
    if(HAL_FDCAN_GetRxMessage(&hfdcan2,FDCAN_RX_FIFO0,&fdcan2_RxHeader,buf)!=HAL_OK)return 0;//接收数据
	*Identifier = fdcan2_RxHeader.Identifier;
	return fdcan2_RxHeader.DataLength >> 16;	
}



void FDCAN2_fliter_config(){
    
    FDCAN_FilterTypeDef FDCAN2_RXFilter;

      //配置RX滤波器   
    FDCAN2_RXFilter.IdType=FDCAN_STANDARD_ID;                       //标准ID
    FDCAN2_RXFilter.FilterIndex=0;                                  //滤波器索引                   
    FDCAN2_RXFilter.FilterType=FDCAN_FILTER_MASK;                   //滤波器类型
    FDCAN2_RXFilter.FilterConfig=FDCAN_FILTER_TO_RXFIFO0;           //过滤器0关联到FIFO0  
    FDCAN2_RXFilter.FilterID1=0x0000;                               //32位ID
    FDCAN2_RXFilter.FilterID2=0x0000;                               //如果FDCAN配置为传统模式的话，这里是32位掩码
    if(HAL_FDCAN_ConfigFilter(&hfdcan2,&FDCAN2_RXFilter)!=HAL_OK) //滤波器初始化
		{
			Error_Handler();
		}
    HAL_FDCAN_Start(&hfdcan2);                               //开启FDCAN
    HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0);



}

void FDCAN1_fliter_config(){
    
    FDCAN_FilterTypeDef FDCAN2_RXFilter;

    //配置RX滤波器   
	FDCAN2_RXFilter.IdType=FDCAN_EXTENDED_ID;               //扩展ID
	FDCAN2_RXFilter.FilterIndex=0;                          //滤波器索引   
	FDCAN2_RXFilter.FilterType=FDCAN_FILTER_MASK;           //滤波器类型
	FDCAN2_RXFilter.FilterConfig=FDCAN_FILTER_TO_RXFIFO0;   //过滤器0关联到FIFO0  
	FDCAN2_RXFilter.FilterID1 =0x0000; 
	FDCAN2_RXFilter.FilterID2 =0x0000;
//	FDCAN2_RXFilter.FilterID1 =0x00000000; 
//	FDCAN2_RXFilter.FilterID2 =0x1FFFFFFF;	

	
	if(HAL_FDCAN_ConfigFilter(&hfdcan1,&FDCAN2_RXFilter)!=HAL_OK)                           //滤波器初始化
	HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_REJECT, FDCAN_REJECT, DISABLE, DISABLE);    //FDCAN_ACCEPT_IN_RX_FIFO0
	HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0);
	HAL_FDCAN_Start(&hfdcan1);                                                              //开启FDCAN

}




