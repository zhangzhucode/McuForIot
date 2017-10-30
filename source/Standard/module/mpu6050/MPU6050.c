/*****************************************************************************
**	文件名称： MPU6050.c
**	程序作者： zzhere2007
**	修改时间： 20130820
**	程序功能： MPU6050驱动
**	总共引脚：  SDA PE0 , SCL PE1 , vcc-3.3 ,GND
*******************************************************************************/ 
#include "MPU6050.h"

//#include "LED.h"

typedef struct int16_xyz{
				int16_t X;
				int16_t Y;
				int16_t Z;}S_INT16_XYZ;

uint8_t 		mpu6050_buffer[14];					
S_INT16_XYZ 	GYRO_OFFSET,ACC_OFFSET;			
uint8_t 		GYRO_OFFSET_OK = 1;
uint8_t 		ACC_OFFSET_OK = 1;
S_INT16_XYZ 	MPU6050_ACC_LAST,MPU6050_GYRO_LAST;	
int16_t 		MPU6050_tem;
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器
*******************************************************************************/
void MPU6050_Dataanl(void)	   //取值-基准
{
	//加速度
	MPU6050_ACC_LAST.X=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - ACC_OFFSET.X;
	MPU6050_ACC_LAST.Y=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - ACC_OFFSET.Y;
	MPU6050_ACC_LAST.Z=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) - ACC_OFFSET.Z;
	//温度ADC
	MPU6050_tem		  =(int16_t)((((int16_t)mpu6050_buffer[6]) << 8) | mpu6050_buffer[7])/340 +36;
   	//陀螺仪
	MPU6050_GYRO_LAST.X=((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]) - GYRO_OFFSET.X;
	MPU6050_GYRO_LAST.Y=((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) - GYRO_OFFSET.Y;
	MPU6050_GYRO_LAST.Z=((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) - GYRO_OFFSET.Z;
	//MPU6050_tem =(MPU6050_tem-32)/1.8;
	RS232_PutString("\r\n-----MPU6050--ACC---------------");
	RS232_PutString("\r\nMPU6050_ACC_LAST.X:");
	RS232_PutWord(MPU6050_ACC_LAST.X,16);
	RS232_PutString("\r\nMPU6050_ACC_LAST.Y:");
	RS232_PutWord(MPU6050_ACC_LAST.Y,16);
	RS232_PutString("\r\nMPU6050_ACC_LAST.Z:");
	RS232_PutWord(MPU6050_ACC_LAST.Z,16);
	RS232_PutString("\r\n-----MPU6050--tem---------------");
	RS232_PutString("\r\nMPU6050_tem:");
	RS232_PutWord(MPU6050_tem,16);
	RS232_PutString("\r\n-----MPU6050--GYRO---------------");
	RS232_PutString("\r\nMPU6050_GYRO_LAST.X:");
	RS232_PutWord(MPU6050_GYRO_LAST.X,16);
	RS232_PutString("\r\nMPU6050_GYRO_LAST.Y:");
	RS232_PutWord(MPU6050_GYRO_LAST.Y,16);
	RS232_PutString("\r\nMPU6050_GYRO_LAST.Z:");
	RS232_PutWord(MPU6050_GYRO_LAST.Z,16);

	/**************************以下是一个200次的滤波****************************************/
	if(!GYRO_OFFSET_OK)
	{
		static int32_t	tempgx=0,tempgy=0,tempgz=0;
		static uint8_t cnt_g=0;

		if(cnt_g==0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			cnt_g = 1;
			return;
		}
		tempgx+= MPU6050_GYRO_LAST.X;
		tempgy+= MPU6050_GYRO_LAST.Y;
		tempgz+= MPU6050_GYRO_LAST.Z;
		if(cnt_g==200)
		{
			GYRO_OFFSET.X=tempgx/cnt_g;
			GYRO_OFFSET.Y=tempgy/cnt_g;
			GYRO_OFFSET.Z=tempgz/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
			return;
		}
		cnt_g++;
	}
	if(!ACC_OFFSET_OK)
	{
		static int32_t	tempax=0,tempay=0,tempaz=0;
		static uint8_t cnt_a=0;

		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= MPU6050_ACC_LAST.X;
		tempay+= MPU6050_ACC_LAST.Y;
		tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			return;
		}
		cnt_a++;		
	}
}
/**************************读数据函数********************************************
//将iic读取到得数据分拆,放入相应寄存器,更新MPU6050_Last
*******************************************************************************/
void MPU6050_READ(void)
{
	i2cRead(devAddr,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050_buffer);
}
/******************************************************************************
*函数原型:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
 输入	dev     目标设备地址
        reg	    寄存器地址
        bitNum  要修改目标字节的bitNum位
        data    为0 时，目标位将被清0 否则将被置位
返回   成功 为1  失败为0
*******************************************************************************/ 
void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
	u8 b;
	i2cRead(dev, reg, 1, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	i2cWrite(dev, reg, b);
}
/******************************************************************************
*函数原型:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
输入	dev  目标设备地址
reg	   寄存器地址
bitStart  目标字节的起始位
length   位长度
data    存放改变目标字节位的值
返回   成功 为1 
失败为0
*******************************************************************************/ 
void IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	
	u8 b;
	u8 mask;

	i2cRead(dev, reg, 1, &b);
	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
	data <<= (8 - length);
	data >>= (7 - bitStart);
	b &= mask;
	b |= data;
	i2cWrite(dev, reg, b);
}
/**************************设置函数*************************************************************************************
*函数原型:		void MPU6050_setClockSource(uint8_t source)
*功　　能:	    设置  MPU6050 的时钟源
* CLK_SEL | Clock Source
* --------+--------------------------------------
* 0       | Internal oscillator
* 1       | PLL with X Gyro reference
* 2       | PLL with Y Gyro reference
* 3       | PLL with Z Gyro reference
* 4       | PLL with external 32.768kHz reference
* 5       | PLL with external 19.2MHz reference
* 6       | Reserved
* 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source){
	IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
	
}
/** Set full-scale gyroscope range.	  //陀螺仪最大量程
* @param range New full-scale gyroscope range value
* @see getFullScaleRange()
* @see MPU6050_GYRO_FS_250
* @see MPU6050_RA_GYRO_CONFIG
* @see MPU6050_GCONFIG_FS_SEL_BIT
* @see MPU6050_GCONFIG_FS_SEL_LENGTH
*/
void MPU6050_setFullScaleGyroRange(uint8_t range) {
	IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************设置函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
	IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}
/**************************设置函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
enabled =1   睡觉
enabled =0   工作
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************设置函数********************************************
*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************设置函数********************************************
*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

void MPU6050_setDLPF(uint8_t mode)
{
	IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}
/**************************初始化函数********************************************
*函数原型:		void MPU6050_initialize(void)
*功　　能:	    初始化 	MPU6050 以进入可用状态。
*******************************************************************************/
void MPU6050_INIT(void)
{
//		I2C2_Write_1Byte(0xd0,0x19,0x07);//1khz
//		I2C2_Write_1Byte(0xd0,0x1a,0x03);//44hz滤波
//		I2C2_Write_1Byte(0xd0,0x1b,0x00);//gyro 250du/s
//		I2C2_Write_1Byte(0xd0,0x1c,0x08);//acc +-4g
//		I2C2_Write_1Byte(0xd0,0x24,13);//400khz
//		I2C2_Write_1Byte(0xd0,0x6a,0x00);//mst_en=0
//		I2C2_Write_1Byte(0xd0,0x6b,0x00);//sleep=0
	
	MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //设置时钟  0x6b   0x01
	OSTimeDly(2);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_500);//陀螺仪最大量程 +-500度每秒
	OSTimeDly(2);
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_4);	//加速度度最大量程 +-4G
	OSTimeDly(2);
	MPU6050_setDLPF(MPU6050_DLPF_BW_42);
	OSTimeDly(2);
	MPU6050_setSleepEnabled(0); //进入工作状态
	OSTimeDly(2);
	MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
	OSTimeDly(2);
	MPU6050_setI2CBypassEnabled(1);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
	OSTimeDly(2);
	
	//	//配置MPU6050 的中断模式 和中断电平模式
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 0);
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);
	//	//开数据转换完成中断
	//    IICwriteBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1);
}
/***********************************************************************************************
**
**			   test程序：已通过
***********************************************************************************************/
void MPU6050_test(void)
{	
	i2cInit();
	MPU6050_INIT();
	while(1)
	{  
	   	MPU6050_READ();
	   	MPU6050_Dataanl();
	   	
	   	printf("\r\test——loop");
	   	OSTimeDly(OS_TICKS_PER_SEC);
	}	
}

