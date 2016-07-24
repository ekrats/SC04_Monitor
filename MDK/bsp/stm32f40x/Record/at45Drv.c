/*****************************************************************************
*
* Copyright (c) 2007,����ͨҵ�Ƽ����޹�˾ .All rights reserved.
*
* -----------------------------------------------------------------------------
*
* �ļ�ժҪ:���Ѽ��װ�õ�flash ��IC �������ļ���ATMEL��˾��
* AT45DXX�ȴ�����FLASH /IC�� �Ķ�д����
*
* -----------------------------------------------------------------------------
*
* �ļ�����:	At45drv.c
* ���뻷��:	ads1.2
* ���°汾:  v1.0
* Ӳ��ƽ̨:  ���Ѽ��V1.0 LPC213X
*****************************************************************************/

/***�޸ļ��汾��¼*****************************************************************
* �޸�ʱ��:  2007/7/28
* �޸���Ա: �Ƴ���
* �����汾:	V1.0
* ��������: ���α�д
*
**********************************************************************************/
/***�޸ļ��汾��¼*****************************************************************
* �޸�ʱ��:  
* �޸���Ա:
* �����汾:
* ��������:
*
**********************************************************************************/
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//
//                       �� AT45DBxxxx   оƬ˵�� ��
//
//AT45xxx Series 
//Devices Description 
//AT45DB011D    1M bit , 2.7-Volt Minimum Serial-Interface Flash with One 264-Byte SRAM Buffer 
//AT45DB021D    2M bit , 2.7-Volt Minimum Serial-Interface Flash with One 264-Byte SRAM Buffer 
//AT45DB041D    4M bit , 2.5-Volt or 2.7-Volt DataFlash 
//AT45DB081D    8M bit , 2.5 or 2.7-Volt Only Serial-Interface Flash 
//AT45DB161D    16M bit, 2.7-Volt Only Serial-Interface Flash with two SRAM Data Buffers 
//AT45DB321D    32M bit, 2.7-Volt Only Serial Interface Flash with two SRAM Data Buffers
//AT45DB642D    64M bit, 2.7-Volt Dual-Interface Flash with two 1056-Byte SRAM 

//��AT45DB011D��1Mbit����AT45DB021D��2Mbit����AT45DB041D��4Mbit����AT45DB081D��8Mbit����
//AT45DB161D��16Mbit����AT45DB321D��32Mbit����AT45DB642D��64Mbit��ȫϵ�ж��У�����
//011D~041D����264�ֽ�Page�ģ�161D��321D��528�ֽ�Page��642D��1056�ֽ�Page�ġ�
//��ϵͳ�������������������ֱ�����ΪBUF1��BUF2����������Flash�к��ټ��ġ����Ҹ�ϵ��
//�������װ����ͨ�õ�


//AT45DB161D�Ǵ��нӿڵ�����оƬ���ɹ�����2.5V~2.7V���ɹ㷺Ӧ��������������ͼ�񡢳���
//�������ݴ洢�С�AT45DB161D֧��RapidS���нӿڣ������ڸ��ٳ��ϡ�RapidS���нӿ�����SPI
//����ݵģ��ٶȿɴﵽ66MHz����������17,301,504��λ������֯Ϊ4096��ҳ��ÿ��ҳ512��528
//���ֽڡ��������洢����AT45DB161D����������SRAM���ݻ�������ÿ��������512/528���ֽڡ�
//�����洢�����ڱ��ʱ���������������������ݵģ�����֧��������ʽд�롣�벢��FLASH����
//����ͬ��������RapidS���нӿڣ��Ӷ��������˿�������������ͬʱҲ�����ϵͳ�ɿ��ԣ���
//���˿�����������С�˷�װ���������Ӧ������ҵ����ҵ����Ҫ���ܶȡ������������͵�ѹ���
//���ĵ�Ӧ�ó��ϡ�
//
//AT45DB161D�����򵥵���ϵͳ���±�̣�����������߱�̵�ѹ��оƬ���Բ���2.5V~3.6V��
//2.7V~3.6V����Դ���磬���б�����ȡ������������ͨ��#CS������ʹ�ܣ���ͨ�����߽ӿ�
//��SI��SO��SCK����������ͨ�š�
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//�����������Щ����������������������������������������Щ��������������Щ�������������
//��  ����  ��           �����빦��                   ��   ��Ч��ƽ   ��    ����    ��
//�����������੤���������������������������������������੤�������������੤������������
//��  CS    ��#CS�����ϵ��½��ؽ�������һ������������ ��   LOW        ��   Input    ��
//��        ������������һ����������һ���ڲ�������о��              ��            ��
//��        ��Ƭ�ڲ��ı�̻���������ڣ�оƬ��������Щ�              ��            ��
//��        ����״̬��ֱ���������.                   ��              ��            ��
//�����������੤���������������������������������������੤�������������੤������������
//��  SCK   ������ʱ�ӣ�������������оƬ�ṩʱ���źţ���              ��   Input    ��
//��        �����������������ĳ��롣SI�����ϵ�����ة�              ��            ��
//��        ��ַ������������ʱ��SCK�������ر�д�룬�� ��              ��            ��
//��        ��SO�����ϵ������������ʱ�ӵ��½��ر仯. ��              ��            ��
//�����������੤���������������������������������������੤�������������੤������������
//��  SI    ���������룺SI����������оƬ����λ��ʽд�멦              ��   Input    ��
//��        �����ݡ�SI�����ϵ����������������������ة�              ��            ��
//��        ��ַ��SI�ϵ�������ʱ�ӵ�������д��оƬ��  ��              ��            ��
//�����������੤���������������������������������������੤�������������੤������������
//��  SO    �����������SO����������оƬ����λ��ʽ�����              ��   Output   ��
//��        �����ݡ�SI�ϵ�������ʱ�ӵ��½��ر仯��    ��              ��            ��
//�����������੤���������������������������������������੤�������������੤������������
//��  WP    ��д��������#WP������Ϊ��ЧʱоƬ�������� ��   LOW        ��   Input    ��
//��        ���������������Է�ֹ�������������ݵ��ƻ���              ��            ��
//��        ����������������ʹ������������������Ȼ���ԩ�              ��            ��
//��        ����оƬʶ��#WP�������ڲ������ߣ������� ��              ��            ��
//��        ���ա�������Ȼ�������ⲿ�ӵ�VCC��         ��              ��            ��
//�����������੤���������������������������������������੤�������������੤������������
//��  Reset ����λ��#RESET�����ϵĵ͵�ƽ����ֹ���ڴ�����   LOW        ��   Input    ��
//��        ���Ĳ�������λ�ڲ�״̬��������״̬��#RESET��              ��            ��
//��        �������ϵĵ͵�ƽ��ʹоƬһֱ���ڸ�λ״̬����              ��            ��
//��        ����#RESET��תΪ�ߵ�ƽ�󣬲��ܽ��������Ĳ٩�              ��            ��
//��        ������оƬ�ڲ������ϵ縴λ��·���������Ų���              ��            ��
//��        ����ʱ���ⲿ�ӵ��ߵ�ƽ.                   ��              ��            ��
//�����������ة����������������������������������������ة��������������ة�������������
// 
// AT45DB161D�Ĵ��������б���Ϊ3����������ȣ��ֱ�Ϊ����������ҳ������ġ��洢���ṹͼ��
// �Ը�����������˷�������ϸ˵����ÿ����������ҳ�������еı�̲������������ҳ�ġ�
// ������������������оƬ�����������ҳ��
//
//��������������������������������������������
//��               �����ṹ                 ��
//��������������������������������������������
//�������� 0A����8ҳ   4096  /4224   �ֽ�   ��
//��������������������������������������������
//�������� 0B����248ҳ 126,976/130,944�ֽ�  ��
//��������������������������������������������
//�������� 1 ����256ҳ 131,072/135,168�ֽ�  ��
//��������������������������������������������
//�������� 2 ����256ҳ 131,072/135,168�ֽ�  ��
//��������������������������������������������
//��               ......                   ��
//��������������������������������������������
//�������� 14����256ҳ 131,072/135,168�ֽ�  ��
//��������������������������������������������
//�������� 15����256ҳ 131,072/135,168�ֽ�  ��
//��������������������������������������������
// 
//
//
//
//
//
//


#define _AT45DRV_
#include <rtthread.h>
#include <stm32f4xx.h>
//#include "ds.h"	 
#include "at45Drv.h"
#include "spiDrv.h"

#define SetFCS (FLASH_CS_HIGH())
#define ClrFCS (FLASH_CS_LOW())

//��ʱ�ж��ö�ʱ������
// #define TIME_TICKS_VAL    (OSTick)
// extern vu32 OSTick; //�ñ���Ҫ��Timer�жϽ����ۼ�,�ۼ�Ƶ��1KHz         

//-----------------------------------------------------------------------------------
// SPI PORT
//-----------------------------------------------------------------------------------
#define SPI_DEVICE_PORT             (SPI2)



/*************************************�����ڲ�������FLASH�ĵײ��������*************************/



/****************************************************************************
** �������ƣ�static u8 SPI_FLASH_SendByte(u8 byte) 
** ��    �ܣ�ͨ��SPI����һ���ֽ�����
** ˵    ���� 
** ��ڲ�������
** ���ڲ�������ȡ������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
/*******************************************************************************
* Function Name  : SPI_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
// rt_uint8_t SPI_SendByte(SPI_TypeDef* SPIx, rt_uint8_t byte)
// {
// 	rt_uint32_t i = 0 ;
// 	rt_uint32_t j = 0 ;
// 	/* Loop while DR register in not emplty */
// 	while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)&&(++i<1000000));
// 	/* Send byte through the SPI1 peripheral */
// 	SPI_I2S_SendData(SPIx, byte);
// 	/* Wait to receive a byte */
// 	while ((SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)&&(++j<1000000));
// 	/* Return the byte read from the SPI bus */
// 	return SPI_I2S_ReceiveData(SPIx) ;
// }
/****************************************************************************
** �������ƣ�static u8 ICReadByte(void) 
** ��    �ܣ���һ���ֽ�����
** ˵    ���� 
** ��ڲ�������
** ���ڲ�������ȡ������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
static u8 ICReadByte(void)
{
	return(SPI_SendByte(SPI_DEVICE_PORT,0x0));
}

/****************************************************************************
** �������ƣ�static void ICWriteByte(u8 data) 
** ��    �ܣ�дһ���ֽ�����
** ˵    ���� 
** ��ڲ�����data ��д������
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
static void ICWriteByte(u8 data)
{
	SPI_SendByte(SPI_DEVICE_PORT,data);
}

/****************************************************************************
** �������ƣ�static void ICWaitReady(void) 
** ��    �ܣ�����״̬��BUSYλ����������ʱ�ȴ�
** ˵    ��������״̬�ֵĵ�8λ�����λΪ0��ȴ�
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��7��30��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICWaitReady(void)                 
{
	u8 c;
	while (1)
	{
		c = ICStatus();
		if (c&0x80) break;
	}
}


AT45FlashStatus ICWaitReady_timeout(u16 timeout)                 
{
	u8 c;
    //u32 t;

    //t = 0;//
	while (1)
	{
		c = ICStatus();
		if(c & 0x80)
        {
            return AT45_FLASH_COMPLETE;
        }

//         if((TIME_TICKS_VAL - t) > timeout)
//         {
//             return AT45_FLASH_OP_TIMEOUT;
//         }
	}
}

/****************************************************************************
** �������ƣ�static void ICCommand(OpCode cmd, u16 Page, u16 ByteAddr)
** ��    �ܣ�ִ��IC���Ĳ���ָ��
** ˵    ��������OPCODE�������IC�����в�����
**           OPCODE�ľ��庬��μ�ͷ�ļ�
** ��ڲ�����cmd������; Pageҳ����; ByteAddr�ֽڵ�ַ
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��7��30��
**---------------------------------------------------------------------------
** �޸��ˣ�������
** �ա��ڣ�2010-05-19
** �ڡ��ݣ�if�ж���������������(cmd != DeepPowerDown)&&(cmd != ResumeDeepPowerDown))����֧��ʡ��ģʽ���˳�ʡ��ģʽ����
****************************************************************************/
/*IC������ѡ��ִ��*/
static void ICCommand(OpCode cmd, u16 Page, u16 ByteAddr)
{
	u8  temp     = 0 ;
	u32 cmd_temp = 0;
	u8  data;

	/*����������*/
	ICWriteByte((u8)cmd);		

	if (   (cmd != StatusRead) 
		&& (cmd != IDRead)
		&& (cmd != DeepPowerDown)
		&& (cmd != ResumeDeepPowerDown)
		)//����3���ֽڵ�ַ
	{
		cmd_temp =  (Page&0xfff) << 10;  //12λҳ��ַPA0-PA11
		cmd_temp += (ByteAddr & 0x3FF);  //528ģʽ�¶�Ӧҳ�µ��ֽڵ�ַPB0-PB9
		//�����Ǹ�λ��ǰ��ʽ:PB0-PB9,PA0-PA11,XX
		data = (u8)(cmd_temp >> 16);
		ICWriteByte(data);
		data = (u8)(cmd_temp >> 8);
		ICWriteByte(data);
		data = (u8)cmd_temp ;
		ICWriteByte(data);

	}

	switch (cmd)
	{
	case PageRead:              //ֱ�Ӷ�ҳ�����治��
		{
			temp = ICReadByte();	//����32λ�޹�λ
			temp = ICReadByte();
			temp = ICReadByte();
			temp = ICReadByte();
			temp = temp ;
			//			ucResult = ICReadNextByte();
			break;
		}

	case Buf1Read:              //����1��
	case Buf2Read:             //����2��
		/*		{
		temp = ICReadByte();
		//			ucResult = ICReadNextByte();
		break;
		}
		*/
	case PageToBuf1:            //ҳ��������1
	case PageToBuf2:            //ҳ��������2
		{
			temp = ICReadByte();	//����8λ�޹�λ
			break;
		}
		/*		case PageCmpBuf1:           //ҳ�뻺��1�Ƚϣ��ȽϽ����״̬�Ĵ�����6λ
		case PageCmpBuf2:                   //ҳ�뻺��2�Ƚϣ��ȽϽ����״̬�Ĵ�����6λ
		case Buf1Write:                     //����1д
		case Buf2Write:                     //����2д
		case Buf1ToPageWithErase:             //ҳ�����󣬽�����1д��ҳ
		case Buf2ToPageWithErase:             //ҳ�����󣬽�����2д��ҳ
		case Buf1ToPageWithoutErase:          //ֱ�ӽ�����1д��ҳ
		case Buf2ToPageWithoutErase:          //ֱ�ӽ�����2д��ҳ
		case PageWriteThroughBuf1:            //������д�뻺��1����д��ҳ
		case PageWriteThroughBuf2:            //������д�뻺��2����д��ҳ
		case PageRewriteThroughBuf1:          //ҳ���뻺��1��Ȼ�󻺴�1д��ҳ
		case PageRewriteThroughBuf2:          //ҳ���뻺��2��Ȼ�󻺴�2д��ҳ
		break;

		case StatusRead:                      //��״̬�Ĵ���
		//		ucResult = ICReadNextByte();
		break;
		*/
	default : break;
	}

}

/****************************************************************************
** �������ƣ�static u8 ICReadBuf1Byte(u16 ByteAddr)
** ��    �ܣ��ӻ�����1ָ����ַ��һ���ֽ�����
** ˵    ���� 
** ��ڲ�����ByteAddr��ʼ��ַ
** ���ڲ�������������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
static u8 ICReadBuf1Byte(u16 ByteAddr)
{
	u8 Rt_Temp;

	ClrFCS;
	ICCommand(Buf1Read, 0, ByteAddr);
	Rt_Temp = ICReadByte();
	SetFCS;

	return (Rt_Temp);
}

/****************************************************************************
** �������ƣ�static u8 ICReadBuf2Byte(u16 ByteAddr)
** ��    �ܣ��ӻ�����2ָ����ַ��һ���ֽ�����
** ˵    ���� 
** ��ڲ�����ByteAddr��ʼ��ַ
** ���ڲ�������������
** ȫ�ֱ�����
** �����ߣ� �Ƴ���
** �ա��ڣ� 2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
static u8 ICReadBuf2Byte(u16 ByteAddr)
{

	u8 Rt_Temp;

	ClrFCS;
	ICCommand(Buf2Read, 0, ByteAddr);
	Rt_Temp = ICReadByte();
	SetFCS;

	return (Rt_Temp);
}
/***************************************���¶���ӿں���*************************************************************/

/****************************************************************************
** �������ƣ�void  ICInit(void) 
** ��    �ܣ���ʼ��AT45DB161
** ˵    ������
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��7��30��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
u8 AT45ICInit(void)                  
{
	//SPI_FLASH_Init();

	ICIDR();  //��IC����ID
	ICID.sr = ICStatus(); 

	//�ж�FLASH�������Ƿ�ATMEL��˾��AT45DB161
	if(    ((ICID.sr & 0x3c) == 0x2c) 
		&& (ICID.mid == 0x1f) 
		&& (ICID.did1==0x26)
	  )
	{
		return (TRUE);
	}
	else
	{
		return(FALSE);
	}

}

/****************************************************************************
** �������ƣ�u8 ICStatus(void)
** ��    �ܣ���FLASH/IC��״̬
** ˵    ����AT45DB161״̬��Ϊ 
**           PAGE SIZE(BIT0):       1->512byte 0->528byte
**           PROTECT  (BIT1)��      1->����  0->δ���� 
**			 оƬ������(BIT2-BIT5)��AT45DB161�̶�Ϊ BIT2-BIT5->1101
**			 COMP(BIT6):            1->���Ƚ�ҳ������һ�������뻺�������� 0->���Ƚ�ҳ�뻺�����������
**           RDY/-BUSY(BIT7):       1->���� 0->æ
**
**           Bit 7    Bit 6    Bit 5    Bit 4    Bit 3    Bit 2    Bit 1      Bit 0
**           RDY/BUSY COMP       1        0        1        1     PROTECT   PAGE SIZE
** ��ڲ�������
** ���ڲ�����IC��״̬��
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��7��30��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
u8 ICStatus(void)                     
{
	u8 ucResult;

	ClrFCS;
	ICCommand(StatusRead, 0, 0);
	ucResult = ICReadByte();
	SetFCS;

	return (ucResult);
}




/****************************************************************************
** �������ƣ�void  ICIDR(void) 
** ��    �ܣ���ȡIC���洢���ĳ���ID
** ˵    ��������ID��1�ֽ�1F��ʾATMEL��˾2�ֽ�26��ʾ16M DATAFLASH
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��7��30��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void  ICIDR(void)                  
{
	ClrFCS;
	ICCommand(IDRead, 0, 0);
	ICID.mid  = ICReadByte();
	ICID.did1 = ICReadByte();
	ICID.did2 = ICReadByte();
	ICID.einf = ICReadByte();
	SetFCS;
}
/****************************************************************************
** �������ƣ�u8 ICWriteToPage(u8 *pData,u16 Page, u16 ByteAddr, u16 Size, u8 Operation)
** ��    �ܣ�ҳд��������,������������
** ˵    ���� 
** ��ڲ�����p���� count���� page д��ҳ��byteaddr д���ֽڵ�ַ
operation ������ʽ����ADD��NEW��׷�����ݻ���ȫ������
** ���ڲ�����д�ɹ����
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
u8 ICWriteToPage(u8 *pData,u16 Page, u16 ByteAddr, u16 Size, u8 Operation)
{

	/*��ȡҳ����������2*/
	if((Operation == OpCmdADD)||(Operation == OpCmdADDCLR))
	{
		ICPageToBuf2(Page);    
		ICWaitReady();
	}
	/*д���ַ���������2*/
	ICWriteBuf2(ByteAddr, pData,Size);
	if((Operation == OpCmdNEWCLR)||(Operation == OpCmdADDCLR))
	{
		ICWriteBuf2((ByteAddr+Size), NULL, (528 - Size - ByteAddr));
	}
	/*����2д��ҳ*/
	ICBuf2ToPage(Page);
	ICWaitReady();

	/*�Ƚ�д���Ƿ�ɹ�*/
	return (ICBuf2CmpPage(Page));
}
/****************************************************************************
** �������ƣ�void ICWriteFullPage(u8 *pData, u16 page, u8 bufferIndex)
** ��    �ܣ�FLASH��ҳд,����ҪԤ����.
** ˵    ����  
** ��ڲ�����*pData       ��д�������,  
             page         ҳ��
             bufferIndex  ʹ�õ�Ram������
** ���ڲ�������
** ȫ�ֱ�����
** ����  �ߣ� 
** �ա�  �ڣ� 
**---------------------------------------------------------------------------
** �� �� �ˣ�
** �ա�  �ڣ�
** �ڡ�  �ݣ�
****************************************************************************/
void ICWriteFullPage(u8 *pData, u16 page, u8 bufferIndex)
{
	switch(bufferIndex)
	{
	case 0:
		ICWriteBuf1(0, pData, 528);

		ClrFCS;
		ICCommand(Buf1ToPageWithoutErase, page, 0);
		SetFCS;

		break;
	case 1:
		ICWriteBuf2(0, pData, 528);

		ClrFCS;
		ICCommand(Buf2ToPageWithoutErase, page, 0);
		SetFCS;
		break;

	}

}
/****************************************************************************
** �������ƣ�void ICReadPage(u8 *pData, u16 Page, u16 ByteAddr, u16 size)
** ��    �ܣ�FLASHֱ��ҳ��ȡ
** ˵    ��������ҳ��͵�ַ����ȡһ�����������ݷ��� 
** ��ڲ�����*pData��ȡ���ص�����,  page ����ҳ��byteaddr �����ֽ���ʼ��ַ
size��ȡ���ֽ�����
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICReadPage(u8 *pData, u16 Page, u16 ByteAddr, u16 Size)
{
	u16 i;

	//	Size = Size&0x1ff;

	ClrFCS;
	ICCommand(PageRead, Page, ByteAddr);

	for(i=0;i<Size;i++)
	{
		*pData++ = ICReadByte();
	}

	SetFCS;
}

/****************************************************************************
** �������ƣ�void ICWriteBuf1(u16 ByteAddr,u8 *pData,u16 Size)
** ��    �ܣ�����1д��������
** ˵    ���� 
** ��ڲ�����ByteAddr��ʼ��ַpdataд������� size���ݳ���
** ���ڲ�����д�ɹ����
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICWriteBuf1(u16 ByteAddr,u8 *pData,u16 Size)
{
	u16 i;

	//	Size = Size&0x1ff;

	ClrFCS;
	ICCommand(Buf1Write, 0, ByteAddr);

	for(i = 0; i < Size; i++)
	{
		ICWriteByte(*pData++);
	}

	SetFCS;
}

/****************************************************************************
** �������ƣ�void ICWriteBuf2(u16 ByteAddr,u8 *pData,u16 Size)
** ��    �ܣ�����2д��������
** ˵    ���� 
** ��ڲ�����ByteAddr��ʼ��ַpdataд������� size���ݳ���
** ���ڲ�����д�ɹ����
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICWriteBuf2(u16 ByteAddr,u8 *pData,u16 Size)
{
	u16 i;

	//	Size &= 0x3ff;

	ClrFCS;
	ICCommand(Buf2Write, 0, ByteAddr);

	if(pData == (u8 *)NULL)
	{
		for(i = 0; i < Size; i++)
		{
			ICWriteByte(0);
		}
	}
	else
	{
		for(i = 0; i < Size; i++)
		{
			ICWriteByte(*pData++);
		}
	}

	SetFCS;
}



/****************************************************************************
** �������ƣ�u8  ICBuf1CmpPage(u16 Page)
** ��    �ܣ��Ƚϻ���1��flashҳ������
** ˵    ���� 
** ��ڲ�����page �Ƚ�ҳ��
** ���ڲ������Ƚϻ�����1���ҳ�����Ƿ���� TRUE:��ȣ�FALSE������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
u8  ICBuf1CmpPage(u16 Page)
{

	ClrFCS;
	ICCommand(PageCmpBuf1, Page, 0);
	SetFCS;

	ICWaitReady();

	//�ж�д���������SR�Ĵ���6λΪ1��д���д���
	if(ICStatus() & 0x40) return (FALSE);
	else return (TRUE);
}

/****************************************************************************
** �������ƣ�u8   ICBuf2CmpPage(u16 Page)
** ��    �ܣ��Ƚϻ���2��flashҳ������
** ˵    ���� 
** ��ڲ�����page �Ƚ�ҳ��
** ���ڲ������Ƚϻ�����2���ҳ�����Ƿ���� TRUE:��ȣ�FALSE������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
u8  ICBuf2CmpPage(u16 Page)
{

	ClrFCS;
	ICCommand(PageCmpBuf2, Page, 0);
	SetFCS;

	ICWaitReady();

	//�ж�д���������SR�Ĵ���6λΪ1��д���д���
	if(ICStatus() & 0x40) return (FALSE);
	else return (TRUE);
}

/****************************************************************************
** �������ƣ�void  ICBuf1ToPage(u16 Page)
** ��    �ܣ�����1д�뵽flashҳд��������
** ˵    ���� 
** ��ڲ�����page д��ҳ��
** ���ڲ�����
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void  ICBuf1ToPage(u16 Page)
{
	ClrFCS;
	ICCommand(Buf1ToPageWithErase, Page, 0);
	SetFCS;
}

void  ICBuf1ToPageN(u16 Page)
{
	ClrFCS;
	ICCommand(Buf1ToPageWithoutErase, Page, 0);
	SetFCS;
}

/****************************************************************************
** �������ƣ�void  ICBuf2ToPage(u16 Page)
** ��    �ܣ�����2д�뵽flashҳд��������
** ˵    ���� 
** ��ڲ�����page д��ҳ��
** ���ڲ�����
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void  ICBuf2ToPage(u16 Page)
{
	ClrFCS;
	ICCommand(Buf2ToPageWithErase, Page, 0);
	SetFCS;
}

void  ICBuf2ToPageN(u16 Page)
{
	ClrFCS;
	ICCommand(Buf2ToPageWithoutErase, Page, 0);
	SetFCS;
}

/****************************************************************************
** �������ƣ�void  ICPageToBuf1(u16 Page) 
** ��    �ܣ���FLASHҳ���ݶ��뵽����1��
** ˵    ���� 
** ��ڲ�����page ����ҳ��
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void  ICPageToBuf1(u16 Page)       
{
	ClrFCS;
	ICCommand(PageToBuf1, Page, 0);
	SetFCS;
}

/****************************************************************************
** �������ƣ�void  ICPageToBuf2(u16 Page) 
** ��    �ܣ���FLASHҳ���ݶ��뵽����2��
** ˵    ���� 
** ��ڲ�����page ����ҳ��
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void  ICPageToBuf2(u16 Page)       
{
	ClrFCS;
	ICCommand(PageToBuf2, Page, 0);
	SetFCS;
}

/****************************************************************************
** �������ƣ�void  ICPageErase(u16 Page) 
** ��    �ܣ�ҳ����
** ˵    ���� 
** ��ڲ�����Page ��������ҳ(0-4095)
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void  ICPageErase(u16 Page)       
{
	ClrFCS;
	ICCommand(PageErase, Page, 0);
	SetFCS;
}


/****************************************************************************
** �������ƣ�u8 ICSectorErase(u8 Sector,EraseSectorProperty EraseSector_Property) 
** ��    �ܣ���������
** ˵    ���� 
** ��ڲ�����Sector ����������;	EraseSector_Property:������������ Erase_0_Sector->��������0��Erase_other_Sector->��������1-15
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�������
** �ա��ڣ�2010-05-19
** �ڡ��ݣ�ԭ��ֻ֧��1-15�����������޸ĺ�֧��������������
****************************************************************************/
u8  ICSectorErase(u8 Sector,EraseSectorProperty EraseSector_Property)      
{
	u8 data;

	if(Erase_0_Sector == EraseSector_Property) //����0����
	{
		if((SECTOR_0A == Sector)||(SECTOR_0B == Sector))
		{
			ClrFCS;
			ICCommand(SectorErase, (u16)Sector, 0);
			SetFCS;

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(Erase_other_Sector == EraseSector_Property)	//����1-15����
	{
		if((Sector == 0) || (Sector > 15))
		{
			return FALSE;
		}

		ClrFCS;
		/*����������*/
		ICWriteByte((u8)SectorErase);
		data = Sector << 2;
		ICWriteByte(data);
		ICWriteByte(0);
		ICWriteByte(0);
		SetFCS;
		return TRUE;
	}

	return FALSE;
}

/****************************************************************************
** �������ƣ�void ICReadBuf1(u16 ByteAddr,u8 *pData,u16 Size)
** ��    �ܣ���������1����
** ˵    ���� 
** ��ڲ�����ByteAddr ������1��ַ;	*pData:��ȡ���ݻ������׵�ַ�� Size����ȡ���ݳ���
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ�������
** �ա��ڣ�2010��5��19��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICReadBuf1(u16 ByteAddr,u8 *pData,u16 Size)
{
	u16 read_loop;
	for(read_loop = 0; read_loop < Size; read_loop++)
	{
		pData[read_loop] = ICReadBuf1Byte(ByteAddr);
		ByteAddr++;
	}
}

/****************************************************************************
** �������ƣ�void ICReadBuf2(u16 ByteAddr,u8 *pData,u16 Size)
** ��    �ܣ���������2����
** ˵    ���� 
** ��ڲ�����ByteAddr ������2��ַ;	*pData:��ȡ���ݻ������׵�ַ�� Size����ȡ���ݳ���
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ�������
** �ա��ڣ�2010��5��19��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICReadBuf2(u16 ByteAddr,u8 *pData,u16 Size)
{
	u16 read_loop;
	for(read_loop = 0; read_loop < Size; read_loop++)
	{
		pData[read_loop] = ICReadBuf2Byte(ByteAddr);
		ByteAddr++;
	}
}
/****************************************************************************
** �������ƣ�void ICDeepPowerDown(void)
** ��    �ܣ�ʹAT45DB161����ʡ��ģʽ
** ˵    ����ʡ��ģʽ�ڼ䣬AT45DB161ֻ����ʶ���˳�ʡ��ģʽ��������������
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ�������
** �ա��ڣ�2010��5��19��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICDeepPowerDown(void)                     
{
	ClrFCS;
	ICCommand(DeepPowerDown, 0, 0);
	SetFCS;
}

/****************************************************************************
** �������ƣ�void ICResumeDeepPowerDown(void)
** ��    �ܣ�ʹAT45DB161�˳�ʡ��ģʽ
** ˵    ����
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ�������
** �ա��ڣ�2010��5��19��
**---------------------------------------------------------------------------
** �޸��ˣ�
** �ա��ڣ�
** �ڡ��ݣ�
****************************************************************************/
void ICResumeDeepPowerDown(void)                  
{
	ClrFCS;
	ICCommand(ResumeDeepPowerDown, 0, 0);
	SetFCS;
}

/****************************************************************************
** �������ƣ�u8 ICSectorErase(u8 Sector,EraseSectorProperty EraseSector_Property) 
** ��    �ܣ���������
** ˵    ���� 
** ��ڲ�����Sector ����������;	EraseSector_Property:������������ Erase_0_Sector->��������0��Erase_other_Sector->��������1-15
** ���ڲ�������
** ȫ�ֱ�����
** �����ߣ��Ƴ���
** �ա��ڣ�2007��8��16��
**---------------------------------------------------------------------------
** �޸��ˣ�������
** �ա��ڣ�2010-05-19
** �ڡ��ݣ�ԭ��ֻ֧��1-15�����������޸ĺ�֧��������������
****************************************************************************/
u8 ICBlockErase(u16 Block)
{   
	u16 buf_block = 0;


	if(512 < buf_block)
	{
		return FALSE;
	}

	buf_block =(Block & 0x1FF) << 3; 
	ClrFCS;
	ICCommand(BlockErase, buf_block, 0);
	SetFCS;

	return TRUE;

} 





/*********************** End Of File ****************************************************/









