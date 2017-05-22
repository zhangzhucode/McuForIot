
#include "MPU6050.h"

#define HMC5883L_TWI_ADDR 0x3C //(0x1e<<1)
#define ID_A_DEFAULT            (0x48)  /* normal value of ID register A */
#define ID_B_DEFAULT            (0x34)  /* normal value of ID register B */
#define ID_C_DEFAULT            (0x33)  /* normal value of ID register C */
#define HMC5883L_CONFIG_REG_A   (0x00)  /* configuration register A */
#define HMC5883L_CONFIG_REG_B   (0x01)  /* configuration register B */
#define HMC5883L_MODE_REG       (0x02)  /* mode register */
#define HMC5883L_MAG_X_HI       (0x03)  /* X mag reading - MSB */
#define HMC5883L_MAG_X_LO       (0x04)  /* X mag reading - LSB */
#define HMC5883L_MAG_Z_HI       (0x05)  /* Z mag reading - MSB */
#define HMC5883L_MAG_Z_LO       (0x06)  /* Z mag reading - LSB */
#define HMC5883L_MAG_Y_HI       (0x07)  /* Y mag reading - MSB */
#define HMC5883L_MAG_Y_LO       (0x08)  /* Y mag reading - LSB */
#define HMC5883L_STATUS_REG     (0x09)  /* device status */
#define HMC5883L_ID_REG_A       (0x0a)  /* ID register A */
#define HMC5883L_ID_REG_B       (0x0b)  /* ID register B */
#define HMC5883L_ID_REG_C       (0x0c)  /* ID register C */
///*************************HMC5883L程序部分******************/
///*起始信号*/
//void IIC_Start(void)
//{
//	SDA_out;
//	SDA_set;
//	SCL_set;
//	Delay(35);
//	SDA_clr;
//	Delay(35);
//	SCL_clr;
//}
///*停止信号*/?
//void IIC_Stop(void)
//{
//	SDA_out;
//	SDA_clr;
//	SCL_set;
//	Delay(35);
//	SDA_set;
//	Delay(35);
//}
///*发送应答信号*/
//void IIC_SendAck(uchar Ack)//Ack(0:Ack 1:NoAck)
//{
//	SDA_out;
//	if(Ack)
//		SDA_set;
//	else
//		SDA_clr;
//		SCL_set;
//		Delay(35);
//		SCL_clr;
//		Delay(35);
//}
///*接收应答信号*/ 
//uchar IIC_RecAck(void)
//{
//	uchar	A;
//	SDA_in;
//	SCL_set;
//	Delay(35);
//
//	if(PINC&(1<<4))
//		A=1;
//	else
//		A=0;
//	SCL_clr;
//	Delay(35);
//	return A 1:0; } 
///*向IIC总线发送一个字节数据*/ 
//void HMC5883_Send_Byte(uchar?Dat)
//{
//	uchar i; 
//	SDA_out;
//	for(i=0;i<8;i++)
//	{
//		if(Dat & 0x80)
//			SDA_set;
//		else
//			SDA_clr;
//			SCL_set;
//		Delay(35);
//		SCL_clr;
//		Delay(35);
//		Dat<<=1;
//	}
//		IIC_RecAck();
//}
///*从IIC总线接收一个字节数据*/
//uchar HMC5883_Rec_Byte(void)
//{
//	uchar i,Dat=0;
//	SDA_in;
//	SDA_set;
//	for(i=0;i<8;i++)
//	{
//	Dat<<=1;
//	SCL_set;
//	Delay(35);
//	if(PINC & (1<<4))
//	Dat |=0x01;
//	SCL_clr;
//	Delay(35);
//	}
//	
//	return Dat;
//}
//
///*单字节写HMC5833*/?
//void Single_Write_HMC5883(uchar Address,uchar Dat)
//{
//	IIC_Start();
//	HMC5883_Send_Byte(SlaveAddress);
//	HMC5883_Send_Byte(Address);
//	HMC5883_Send_Byte(Dat);
//	IIC_Stop();
//}
//
///*单字节读HMC5833*/
///*uchar Single_Read_HMC5883(ucharvAddr)
//{
//	uchar Value;
//	IIC_Start();
//	HMC5883_Send_Byte(SlaveAddress);
//	HMC5883_Send_Byte(Addr);
//	IIC_Start();
//	HMC5883_Send_Byte(SlaveAddress+1);
//	Value=HMC5883_Rec_Byte();
//	IIC_SendAck(1);
//	IIC_Stop();
//	return Value;
//	}*/
//
///*多字节读HMC5833*/
//
//void Multiple_Read_HMC5883(void)
//{
//	uchar i;  //连续读出HMC5883内部角度数据，地址范围0x3~0x5?????IIC_Start();?
//	I2C_SendByte(0x3c);
//	I2C_SendByte(0x03);//发送存储单元地址，从0x03开始??????IIC_Start();
//	I2C_SendByte(0x3c+1);
//	for(i=0;i<6;i++)?//连续读取6个地址数据，存储在Rec_Data?????
//	{
//  		Rec_Data[i]=HMC5883_Rec_Byte();
//		if(i==5)
//		IIC_SendAck(1); //最后一个数据需要回NOACK?????????
//		else
//		IIC_SendAck(0); //回应ACK?????
//		}?
//      IIC_Stop();
//	  Delay(500);
//}
////初始化HMC5883，根据需要请参考pdf进行修改
//void HMC5883_Init(void) 
//{ 
//	Single_Write_HMC5883(0x02,0x00);  
//} 
/*************************************************
**主函数 
*************************************************/
void hmc5883l(void) 
{  

    int X,Y,Z; 
	//double Angle; 	
	u8 Rec_Data[20]={0};
	i2cWrite(30, HMC5883L_MODE_REG,0);
	i2cWrite(30,HMC5883L_CONFIG_REG_A,0x70);//CRA
    i2cWrite(30,HMC5883L_CONFIG_REG_B,0x40);//CRB
    i2cWrite(30,HMC5883L_MODE_REG,0x00);//连续模式
		
    do	
	{
		//i2cWrite(30,HMC5883L_MODE_REG,0x01);//MR  Single-Measurement Mode
		i2cRead(30,0x03,6,Rec_Data);
		X=Rec_Data[0]<<8 | Rec_Data[1];
        Z=Rec_Data[2]<<8 | Rec_Data[3];
		Y=Rec_Data[4]<<8 | Rec_Data[5];
		RS232_PutString("\r\nhmc5883L—X:");
		RS232_PutWord(X,16);
		RS232_PutString("\r\nhmc5883L—Y:");
		RS232_PutWord(Y,16);
		RS232_PutString("\r\nhmc5883L—Z:");
		RS232_PutWord(Z,16);
		/*X=Rec_Data[0]<<8 | Rec_Data[1];//Combine?MSB?and?LSB?of?X?Data?output?register?????????
        Z=Rec_Data[2]<<8 | Rec_Data[3];//Combine?MSB?and?LSB?of?Z?Data?output?register?????????
		Y=Rec_Data[4]<<8 | Rec_Data[5];//Combine?MSB?and?LSB?of?Y?Data?output?register?????????
		Angle= atan2((double)Y,(double)X)*(180/3.14159265)+180;//单位：角度?(0~360)?????????
		Angle*=10; 
		Acr=(uint)Angle;	
		Send_DATA(0x82,0);	
        Send_DATA(0x20,1);	
		Send_DATA(Acr%10000/1000+0x30,1);	
        Send_DATA(Acr%1000/100+0x30,1);	
		Send_DATA(Acr%100/10+0x30,1);	
		//Send_DATA('.',1);?
		//Send_DATA(Acr%10+0x30,1);?????????
        Send_DATA(0xa1,1);	
		Send_DATA(0xe3,1);	
		if((Acr>=0 && Acr<=300)	|| (Acr>=3300))

		Dis_str(0x92,"【东】");	
         if(Acr>=600?&&?Acr<=1200)	
             Dis_str(0x92,"【南】");	
		 if(Acr>=1500?&&?Acr<=2100)	
		     Dis_str(0x92,"【西】");	
		 if(Acr>=2400?&&?Acr<=3000)	
		    Dis_str(0x92,"【北】");	
		for(N=12;N>0;N--)Delay(50000);	*/
		OSTimeDly(OS_TICKS_PER_SEC);
     }	
	while(1);	
}


