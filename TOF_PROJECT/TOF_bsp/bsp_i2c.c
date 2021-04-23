
/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp_i2c.h"
#include "VL6180.h"

/**
  * 函数功能: I2C总线位延迟，最快400KHz
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void I2C_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
        
    IAR工程编译效率高，不能设置为7
	*/
	for (i = 0; i < 29; i++);
}

/**
  * 函数功能: CPU发起I2C总线启动信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_HIGH();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_LOW();
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
}

/**
  * 函数功能: CPU发起I2C总线停止信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_LOW();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_HIGH();
}

/**
  * 函数功能: CPU向I2C总线设备发送8bit数据
  * 输入参数: Byte ： 等待发送的字节
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_SendByte(uint8_t Byte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (Byte & 0x80)
		{
			I2C_SDA_HIGH();
		}
		else
		{
			I2C_SDA_LOW();
		}
		I2C_Delay();
		I2C_SCL_HIGH();
		I2C_Delay();	
		I2C_SCL_LOW();
		if (i == 7)
		{
			I2C_SDA_HIGH(); // 释放总线
		}
		Byte <<= 1;	/* 左移一个bit */
		I2C_Delay();
	}
}


/**
  * 函数功能: CPU从I2C总线设备读取8bit数据
  * 输入参数: 无
  * 返 回 值: 读到的数据
  * 说    明：无
  */
uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_HIGH();
		I2C_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_LOW();
		I2C_Delay();
	}
  if(ack==0)
		I2C_NAck();
	else
		I2C_Ack();
	return value;
}

/**
  * 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 输入参数: 无
  * 返 回 值: 返回0表示正确应答，1表示无器件响应
  * 说    明：无
  */
uint8_t I2C_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_HIGH();	/* CPU释放SDA总线 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	I2C_Delay();
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_LOW();
	I2C_Delay();
	return re;
}

/**
  * 函数功能: CPU产生一个ACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_Ack(void)
{
	I2C_SDA_LOW();	/* CPU驱动SDA = 0 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU产生1个时钟 */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
	I2C_SDA_HIGH();	/* CPU释放SDA总线 */
}

/**
  * 函数功能: CPU产生1个NACK信号
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_NAck(void)
{
	I2C_SDA_HIGH();	/* CPU驱动SDA = 1 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU产生1个时钟 */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();	
}

/**
  * 函数功能: 配置I2C总线的GPIO，采用模拟IO的方式实现
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void I2C_InitGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* 打开GPIO时钟 */
	//I2C_GPIO_CLK_ENABLE();

  //GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  //HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);

  /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
  I2C_Stop();
}

/**
  * 函数功能: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
  * 输入参数: _Address：设备的I2C总线地址
  * 返 回 值: 返回值 0 表示正确， 返回1表示未探测到
  * 说    明：在访问I2C设备前，请先调用 I2C_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
  */
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	//I2C_InitGPIO();		/* 配置GPIO */	
	I2C_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	I2C_SendByte(_Address | I2C_WR);
	ucAck = I2C_WaitAck();	/* 检测设备的ACK应答 */
	I2C_Stop();			/* 发送停止信号 */
	return ucAck;
}



//写	reg寄存器 data数据
uint8_t VL6180X_WriteByte(uint16_t reg,uint8_t data)
{
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xFF);
	
	I2C_Start();
	I2C_SendByte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);
	if(I2C_WaitAck())	//等待应答
	{
		I2C_Stop();	
		return 1;		
	}
	I2C_SendByte(Index_H);
	I2C_WaitAck();	//等待ACK
	I2C_SendByte(Index_L);
	I2C_WaitAck();	//等待ACK
	I2C_SendByte(data);
	if(I2C_WaitAck())	//等待ACK
	{
		I2C_Stop();	 
		return 1;		 
	}
	I2C_Stop();
	return 0;	
}

//VL6180X读取8位数据
uint8_t VL6180X_ReadByte(uint16_t reg)
{
	uint8_t res;
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xff);
    I2C_Start(); 
	I2C_SendByte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);//发送器件地址+写命令	
	I2C_WaitAck();		//等待应答 
    I2C_SendByte(Index_H);	//写寄存器地址
    I2C_WaitAck();		//等待应答
	I2C_SendByte(Index_L);	//写寄存器地址
	I2C_WaitAck();	
	
    I2C_Start();
	I2C_SendByte((VL6180X_DEFAULT_I2C_ADDR<<1)|1);//发送器件地址+读命令	
    I2C_WaitAck();		//等待应答 
	res=I2C_ReadByte(0);//读取数据,发送nACK 
    I2C_Stop();			//产生一个停止条件 
	return res;
}




//读取16位数据
//VL6180X读取16位数据
uint8_t VL6180X_Read_HalfWold(uint16_t reg)
{
	uint16_t res;
	uint8_t Index_H = (uint8_t)(reg >> 8);
	uint8_t Index_L = (uint8_t)(reg & 0xff);
    I2C_Start(); 
	I2C_SendByte((VL6180X_DEFAULT_I2C_ADDR<<1)|0);//发送器件地址+写命令	
	I2C_WaitAck();		//等待应答 
    I2C_SendByte(Index_H);	//写寄存器地址
    I2C_WaitAck();		//等待应答
	I2C_SendByte(Index_L);	//写寄存器地址
	I2C_WaitAck();	
	
    I2C_Start();
	I2C_SendByte((VL6180X_DEFAULT_I2C_ADDR<<1)|1);//发送器件地址+读命令	
    I2C_WaitAck();		//等待应答 
	res = I2C_ReadByte(1);//读取数据,发送ACK 
	res <<= 8;
	res |= I2C_ReadByte(0);//读取数据,发送nACK 
    I2C_Stop();			//产生一个停止条件 
	return res;
}

