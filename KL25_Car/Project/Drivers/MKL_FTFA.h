/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                        http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MKL_FTFA.h
** Last modified date:      2012-12-14
** Last version:            V1.0
** Descriptions:            FTFA��������
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Li Xiang
** Created date:            2012-12-14
** Version:                 V0.9
** Descriptions:            The original version ��ʼ�汾
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**
*********************************************************************************************************/
#ifndef _MKL_FTFA_H_
#define _MKL_FTFA_H_

//
//-------- <<< Use Configuration Wizard in Context Menu >>> ----------------------------------------------
//
#ifdef __cplusplus
extern "C" {
#endif
/*********************************************************************************************************
** �궨��
*********************************************************************************************************/    
#ifdef  IAR
#define RUN_IN_RAM __ramfunc
#else
#define RUN_IN_RAM    
#endif 


typedef unsigned char           BOOLEAN;                                /* 2???��?��?                     */
typedef unsigned char           INT8U;                                  /* ?T��?o?8????D����?��?            */
typedef signed   char           INT8S;                                  /* ��D��?o?8????D����?��?            */
typedef unsigned short          INT16U;                                 /* ?T��?o?16????D����?��?           */
typedef signed   short          INT16S;                                 /* ��D��?o?16????D����?��?           */
typedef unsigned long           INT32U;                                 /* ?T��?o?32????D����?��?           */
typedef signed   long           INT32S;                                 /* ��D��?o?32????D����?��?           */
    
//typedef void (* FTFA_CALLBACK_TYPE)(void);    
/*
 * CCOB commands as defined in the KL20 reference manual.
 */
#define FTFA_RD1SEC_CMD			0x01
#define FTFA_PGMCHK_CMD			0x02
#define FTFA_RDRSRC_CMD			0x03
#define FTFA_PGM4_CMD			0x06
#define FTFA_ERSSCR_CMD			0x09
#define FTFA_RD1ALL_CMD			0x40
#define FTFA_RDONCE_CMD			0x41
#define FTFA_PGMONCE_CMD		0x43
#define FTFA_ERSALL_CMD			0x44
#define FTFA_VFYKEY_CMD			0x45

/*
 * Margin read levels
 */
#define FTFA_NORMAL_READ_LEVEL    0x00
#define FTFA_USER_MARGIN_LEVEL    0x01
#define FTFA_FACTORY_MARGIN_LEVEL 0x02    
 
/*
 *Resource select codes for RDRSRC command
 *CCOB address bit 23 is used to select between flexNVM IFR and pflash IFR
 */
#define FTFA_IFR_RSRC             0x00     
#define FTFA_VERSION_ID_RSRC      0x01     

//Helpful definitions to extract bits from 32 bit bytes.
//All CCOB commands are big endien while the P3 is little endien.
#define bits_31_24(data)   (INT8U)(((INT32U)data) >> 24)
#define bits_23_16(data)   (INT8U)(((INT32U)data) >> 16)
#define bits_15_8(data)    (INT8U)(((INT32U)data) >> 8)
#define bits_7_0(data)     (INT8U)((INT32U)data)

#define OPT_TARGET_P1 1

/*
 * Flash memory map.
 */
#if CPU_MKL25
  //pFLASH = program flash
  #define FTFA_PFLASH_START_ADDR          0x00000000
  #define FTFA_PFLASH_END_ADDR	          0x0001FFFF
  
//   #define FTFA_PFLASH_CCOB_START_ADDR     0x40020004

  //Flash Configuration Field
  #define FTFA_FLASH_CONFIG_FIELD_START_ADDR            0x00000400
  #define FTFA_FLASH_CONFIG_FIELD_END_ADDR	            0x0000040F

#endif



///extern void ftfa_set_callback_function( FTFA_CALLBACK_TYPE callback_func);

extern void ftfa_enable_interrupts( uint8_t set_ccie, uint8_t set_rdcollie);
extern void ftfa_disable_interrupts( void );

extern INT8U ftfa_check_for_fstat_errors(void);
extern INT32U ftfa_convert_addr_to_ccob_addr(INT32U addr);
extern INT8U suspend_erase(uint8_t *suspended);
extern INT8U resume_erase(void);


extern INT8U FTFA_RD1SEC(INT32U addr, INT16U num_of_128_bits_to_be_verified, INT8U read_1_margin_choice); 
extern INT8U FTFA_PGMCHK(INT32U addr, INT32U expected_data, INT8U read_1_margin_choice); 
extern INT8U FTFA_RDRSRC(INT32U addr, INT8U resource_select_code, INT32U *data); 
extern INT8U FTFA_PGM4(INT32U addr, INT32U data); 
extern INT8U FTFA_ERSSCR( INT32U addr); 
extern INT8U FTFA_RD1ALL( INT8U margin_level); 
extern INT8U FTFA_RDONCE(INT8U record_index, INT32U *data);
extern INT8U FTFA_PGMONCE(INT8U record_index, INT32U data); 
extern INT8U FTFA_ERSALL(void); 
extern INT8U FTFA_VFYKEY(INT32U most_significant_word, INT32U least_significant_word);
extern void FlashWrite(void);

#ifdef __cplusplus
}
#endif


#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/  

