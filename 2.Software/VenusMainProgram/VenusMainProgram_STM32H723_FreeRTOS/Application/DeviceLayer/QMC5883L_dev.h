/*
***********************************************************************************************
*                                       QMC5883L����������
*
*                                      ʵ��QMC5883L�ײ�����
*
* Filename : QMC5883L.h
* Version : V1.00
* Programmer(s) : FengDuo
*
***********************************************************************************************
*/
 
//�ļ�����
#ifndef _QMC5883L_H_
#define _QMC5883L_H_
 
 
#ifdef   QMC5883L_MOUDLE
#define  QMC5883L_EXT
#else
#define  QMC5883L_EXT  extern
#endif
/*
***********************************************************************************************
*                                         INCLUDE FILES
***********************************************************************************************
*/
//......


//�����


//typedef unsigned          char uint8_t;
//typedef unsigned short     int uint16_t;
//typedef unsigned           int uint32_t;
//typedef unsigned       __INT64 uint64_t;

typedef 					int		CPU_INT16S;
typedef unsigned 	      	int		CPU_INT16U;

typedef long 	      		int		CPU_INT32S;
typedef 					float	CPU_FP32;
typedef	unsigned 			char	CPU_BOOLEAN;
typedef	unsigned 			char	CPU_INT08U;

#define TRUE  1
#define FALSE 0

/*
***********************************************************************************************
*                                            DEFINES
***********************************************************************************************
*/
/* ������ַ���豸�ӵ�ַ+��дѡ�� */
#define	QMC5883L_ADDR_WRITE                     0x1A
#define	QMC5883L_ADDR_READ	                0x1B
 
/* �豸�Ĵ�����ַ */
#define	QMC5883L_ADDR_XOUTL                     0x00
#define	QMC5883L_ADDR_XOUTH                     0x01
#define	QMC5883L_ADDR_YOUTHL                    0x02
#define	QMC5883L_ADDR_YOUTH                     0x03
#define	QMC5883L_ADDR_ZOUTL                     0x04
#define QMC5883L_ADDR_ZOUTH                     0x05
 
#define	QMC5883L_ADDR_STATUS                    0x06
#define	QMC5883L_ADDR_TEMPL                     0x07
#define	QMC5883L_ADDR_TEMPH                     0x08
#define	QMC5883L_ADDR_CFGA                      0x09
#define	QMC5883L_ADDR_CFGB                      0x0A
#define QMC5883L_ADDR_PERIORC                   0x0B
#define QMC5883L_ADDR_CHIPID                    0x0D
 
#define	QMC5883L_ADDR_CFGC                      0x20
#define	QMC5883L_ADDR_CFGD                      0x21
 
/* �豸�Ĵ�������ֵ */
#define QMC5883L_CFGA_OSR_512                   (0 << 7) | (0 << 6)
#define QMC5883L_CFGA_OSR_256                   (0 << 7) | (1 << 6)
#define QMC5883L_CFGA_OSR_128                   (1 << 7) | (0 << 6)
#define QMC5883L_CFGA_OSR_64                    (1 << 7) | (1 << 6)
#define QMC5883L_CFGA_RNG_2G                    (0 << 5) | (0 << 4)
#define QMC5883L_CFGA_RNG_8G                    (0 << 5) | (1 << 4)
#define QMC5883L_CFGA_ODR_10HZ                  (0 << 3) | (0 << 2)
#define QMC5883L_CFGA_ODR_50HZ                  (0 << 3) | (1 << 2)
#define QMC5883L_CFGA_ODR_100HZ                 (1 << 3) | (0 << 2)
#define QMC5883L_CFGA_ODR_200HZ                 (1 << 3) | (1 << 2)
#define QMC5883L_CFGA_MODE_STANDBY              (0 << 1) | (0 << 0)
#define QMC5883L_CFGA_MODE_CONTINUE             (0 << 1) | (1 << 0)
 
#define QMC5883L_CFGB_SOFT_RST                  (1 << 7)
#define QMC5883L_CFGB_ROL_PNT                   (1 << 6)
#define QMC5883L_CFGB_INT_ENB                   (1 << 0)
 
#define QMC5883L_CHIPID_VALUE                   0xFF                            /* ������ʶ,0XFF */
 
#define QMC5883L_CFGA_VALUE_STANDBY             ( QMC5883L_CFGA_OSR_512        \
                                                | QMC5883L_CFGA_RNG_8G         \
                                                | QMC5883L_CFGA_ODR_200HZ      \
                                                | QMC5883L_CFGA_MODE_STANDBY )  /* OSR = 512;RNG = 8G;ODR=200Hz;MODE:����ģʽ */
 
#define QMC5883L_CFGA_VALUE_CONTINUE            ( QMC5883L_CFGA_OSR_512        \
                                                | QMC5883L_CFGA_RNG_8G         \
                                                | QMC5883L_CFGA_ODR_200HZ      \
                                                | QMC5883L_CFGA_MODE_CONTINUE ) /* OSR = 512;RNG = 8G;ODR=200Hz;MODE:����ģʽ */
 
#define QMC5883L_CFGB_VALUE_REBOOT              ( QMC5883L_CFGB_SOFT_RST )      /* QMC5883L������� */
 
#define QMC5883L_CFGC_VALUE                     0x40
#define QMC5883L_CFGD_VALUE                     0x01
#define QMC5883L_PERIORC_VALUE                  0x01
 
#define QMC5883L_XYZBUF_LEN                     0x06
#define QMC5883L_TEMBUF_LEN                     0x02
 
#define QMC5883L_SENSITIVITY_2G                 ( 12 )
#define QMC5883L_SENSITIVITY_8G                 ( 3 )
 
#define QMC5883L_CUM_REBOOT_MAXCNT              ( 100 )                         /* �������ۼ����������� */
#define QMC5883L_CON_REBOOT_MAXCNT              QMC5883L_CUM_REBOOT_MAXCNT      /* �������������������� */
 
/* QMC5883L ������� */ 
#define QMC5883L_POWER_ON()                     BSP_SNR_PWR_H()
#define QMC5883L_POWER_OFF()                    BSP_SNR_PWR_L()
 
/* QMC5883L �ܽ� */ 
#define QMC5883L_PIN_SCL                        BSP_IIC1_PIN_SCL
#define QMC5883L_PIN_SDA                        BSP_IIC1_PIN_SDA
#define QMC5883L_PORT_SDA_SCL                   BSP_IIC1_PORT_SDA_SCL
 
#define QMC5883L_READ_TEMP                                                      /* ʹ��QMC58883L�ڲ��¶� */
 
typedef struct 
{
    CPU_INT16S  X_Data;                                                         /* ������X������ */
    CPU_INT16S  Y_Data;                                                         /* ������Y������ */
    CPU_INT16S  Z_Data;                                                         /* ������Z������ */
    CPU_INT16S  T_Data;                                                         /* �������ڲ��¶� */
    CPU_INT16U  MagDensity;                                                     /* �������ܴ���ǿ�� */
    CPU_FP32    XY_Angle;                                                       /* ������X��Y��н� */
    CPU_FP32    XZ_Angle;                                                       /* ������X��Z��н� */
    CPU_FP32    YZ_Angle;                                                       /* ������Y��Z��н� */
 
    CPU_INT16S  X_Offset;                                                       /* ������X��OFFSET */
    CPU_INT16S  Y_Offset;                                                       /* ������Y��OFFSET */
    CPU_INT16S  Z_Offset;                                                       /* ������Z��OFFSET */
 
    CPU_BOOLEAN HardFault;                                                      /* ������Ӳ���� */
    CPU_BOOLEAN DetectInvalid;                                                  /* ���������ʧЧ */
}QMC5883LSAMPLE_DEF;
 
typedef struct 
{
    CPU_INT16S X_Offset;                                                        /* ������X��OFFSET */
    CPU_INT16S Y_Offset;                                                        /* ������Y��OFFSET */
    CPU_INT16S Z_Offset;                                                        /* ������Z��OFFSET */
}QMC5883L_Save_DEF;
 
typedef enum{
    QMC5883L_DATA_SUCCESS,                                                      /* ��ȡ���ݳɹ� */
    QMC5883L_DATA_ERROR_ADDR,                                                   /* ������ͨ�ŵ�ַ���� */
    QMC5883L_DATA_ERROR_TIMEOUT,                                                /* ��������ȡ��ʱ */
    QMC5883L_DATA_ERROR_VALUE,                                                  /* ��������ֵ���� */
}QMC5883L_RESULT_SAMPLE;
/*
***********************************************************************************************
*                                            EXTERNS
***********************************************************************************************
*/
QMC5883L_EXT QMC5883L_Save_DEF QMC5883LSave;
/*
***********************************************************************************************
*                                  GLOBAL FUNCTION PROTOTYPES
***********************************************************************************************
*/
QMC5883L_EXT void QMC5883L_Soft_Reset(void);
QMC5883L_EXT void QMC5883L_Hard_Reset(void);
QMC5883L_EXT CPU_BOOLEAN QMC5883L_Init(void);
QMC5883L_EXT CPU_BOOLEAN QMC5883L_HardState(void);
QMC5883L_EXT CPU_BOOLEAN QMC5883L_DetectState(void);
QMC5883L_EXT CPU_BOOLEAN QMC5883L_Adjust_Magnetic(void);
QMC5883L_EXT QMC5883L_RESULT_SAMPLE QMC5883L_Read_MagDensity(void);
#endif