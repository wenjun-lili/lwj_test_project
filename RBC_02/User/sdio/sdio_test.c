/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SDIO sd�����������������ļ�ϵͳ��
  ******************************************************************************
  * @attention
  *
 
  ******************************************************************************
  */
#include "sdio/sdio_test.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./usart/bsp_usart.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;

extern SD_CardInfo SDCardInfo;

/* Private function prototypes -----------------------------------------------*/
static void SD_EraseTest(void);
static void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/



void SD_Test(void)
{
  /*------------------------------ SD Init ---------------------------------- */
	/* SD��ʹ��SDIO�жϼ�DMA�жϽ������ݣ��жϷ������λ��bsp_sdio_sd.c�ļ�β*/
   Status=SD_Init();
	
	 printf("\r\n ����һ��MicroSD��ʵ�飨û�����ļ�ϵͳ��*****\r\n");
	
	if(Status != SD_OK)
  {    
	  
    printf("\r\nSD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
		printf("\r\n���ص�Status��ֵΪ��%d\r\n",Status);
  }
  else
  {
    printf("SD����ʼ���ɹ���\n");		 
  }
   
  
	printf( " \r\n CardType is :%d ", SDCardInfo.CardType );
	printf( " \r\n CardCapacity is :%d ", SDCardInfo.CardCapacity );
	printf( " \r\n CardBlockSize is :%d ", SDCardInfo.CardBlockSize );
	printf( " \r\n RCA is :%d ", SDCardInfo.RCA);
	printf( " \r\n ManufacturerID is :%d \r\n", SDCardInfo.SD_cid.ManufacturerID );
	
	
	
  if(Status == SD_OK) 
  {
 
    /*��������*/
    SD_EraseTest();
    
 
    /*single block ��д����*/
    SD_SingleBlockTest();
    
		//�ݲ�֧��ֱ�Ӷ���д������д���ö�������д���̴���
 
    /*muti block ��д����*/
    SD_MultiBlockTest();
  }
 
}


/**
  * @brief  Tests the SD card erase operation.
  * @param  None
  * @retval None
  */
void SD_EraseTest(void)
{  
  /*------------------- Block Erase ------------------------------------------*/
  if (Status == SD_OK)
  {
    /* Erase NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes) */
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));// ��һ������Ϊ������ʼ��ַ���ڶ�������Ϊ����������ַ
  }

  if (Status == SD_OK)
  {
		// ��ȡ�ող���������
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);

    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation(); // ѭ����ѯDMA�����Ƿ����

    /* Wait until end of DMA transfer */
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of erased blocks */
  if (Status == SD_OK)
  {
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(EraseStatus == PASSED)
  {    
 
    printf("\r\n SD���������Գɹ���\n");
  }
  else
  {
 
    printf("\r\nSD����������ʧ�ܣ�\n");
    printf("\r\n��ܰ��ʾ������SD����֧�ֲ������ԣ���SD����ͨ�������single��д���ԣ�����ʾSD���ܹ�����ʹ�á�\n");
  }
}

/**
  * @brief  Tests the SD card Single Blocks operations.
  * @param  None
  * @retval None
  */
void SD_SingleBlockTest(void)
{  
  /*------------------- Block Read/Write --------------------------*/
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();// �ȴ�DMA�������
    while(SD_GetStatus() != SD_TRANSFER_OK);//�ȴ�SDIO��SD���������
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);//��ȡ����
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);// �Ƚ�
  }
  
  if(TransferStatus1 == PASSED)
  {
 
    printf("\r\n Single block ���Գɹ���\n");

  }
  else
  {
 
    printf("\r\n Single block ����ʧ�ܣ���ȷ��SD����ȷ���뿪���壬��һ��SD�����ԣ�\n");
    
  }
}

/**
  * @brief  Tests the SD card Multiple Blocks operations.
  * @param  None
  * @retval None
  */
void SD_MultiBlockTest(void)
{  
  /*--------------- Multiple Block Read/Write ---------------------*/
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == PASSED)
  {
 
    printf("Multi block ���Գɹ���");

  }
  else
  {
	 
    printf("Multi block ����ʧ�ܣ���ȷ��SD����ȷ���뿪���壬��һ��SD�����ԣ�");
  }
}

/**
  *��������Buffercmp 
  * @brief  Compares two buffers. �Ƚ������������������Ƿ����
  * @param  pBuffer1, pBuffer2: buffers to be compared. Ҫ�ȽϵĻ�������ָ��
  * @param  BufferLength: buffer's length  ����������
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
// Offset:���ڻ������ĵ�һ��ֵ
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++)
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  * @brief  Checks if a buffer has all its values are equal to zero.
  * @param  pBuffer: buffer to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer values are zero
  *         FAILED: At least one value from pBuffer buffer is different from zero.
  */
/*
  ��������eBuffercmp
  ��������黺�����������Ƿ�Ϊ0
  pBuffer��Ҫ�ȽϵĻ����� bufferLength������������
  �����
  PASSED������������ȫ��Ϊ0
  FAILED������������������һ����Ϊ0
*/
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

/*********************************************END OF FILE**********************/
