#ifndef __I2C_EEPROM_H__
#define	__I2C_EEPROM_H__


#include "stm32l4xx_hal.h"
#include <inttypes.h>

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define I2C_OWN_ADDRESS                            0x0A

#define I2C_WR	        0x52		/* д����bit */
#define I2C_RD	        0X53		/* ������bit */

#define I2C_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define I2C_GPIO_PORT                       GPIOC   
#define I2C_SCL_PIN                         GPIO_PIN_10
#define I2C_SDA_PIN                         GPIO_PIN_12

#define I2C_SCL_HIGH()                      HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SCL_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define I2C_SCL_LOW()                       HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SCL_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define I2C_SDA_HIGH()                      HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SDA_PIN,GPIO_PIN_SET)    // ����ߵ�ƽ
#define I2C_SDA_LOW()                       HAL_GPIO_WritePin(I2C_GPIO_PORT,I2C_SDA_PIN,GPIO_PIN_RESET)  // ����͵�ƽ
#define I2C_SDA_READ()                      HAL_GPIO_ReadPin(I2C_GPIO_PORT,I2C_SDA_PIN)



void    I2C_Start(void);
void    I2C_Stop(void);
void    I2C_SendByte(uint8_t _ucByte);
uint8_t I2C_ReadByte(uint8_t ack);
uint8_t I2C_WaitAck(void);
void    I2C_Ack(void);
void    I2C_NAck(void);
uint8_t I2C_CheckDevice(uint8_t _Address);
void I2C_InitGPIO(void);


uint8_t VL6180X_WriteByte(uint16_t reg,uint8_t data);
uint8_t VL6180X_ReadByte(uint16_t reg);
uint8_t VL6180X_Read_HalfWold(uint16_t reg);


#endif /* __I2C_EEPROM_H__ */

