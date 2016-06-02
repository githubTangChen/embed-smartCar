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
#include "common.h"
#include "MKL_FTFA.h"


/********************************************************************************************************
** ��̬����:These variables are used by the run_ccob_cmd() function to support
********************************************************************************************************/
static INT8U ftfa_enable_interrupts_flag = FALSE;                           /* �ж�ʹ�ܱ�־            */
static INT8U ftfa_interrupt_mask = 0;                                  
/********************************************************************************************************
** �ص�������A callback function which get's called if a CCOB command is taking a long time to run
** and interrupts are disabled.
********************************************************************************************************/
//FTFA_CALLBACK_TYPE ftfa_callback_function = NULL;

/*********************************************************************************************************
** Function name:           ftfa_enable_interrupts
** Descriptions:            Setup static global variables so that the run_ccob_command function will 
**                          enable interrupts after it starts a command running.
** input parameters:        set_ccie��set_rdcollie
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void ftfa_enable_interrupts( INT8U set_ccie, INT8U set_rdcollie)
{
    ftfa_enable_interrupts_flag = TRUE;    
    ftfa_interrupt_mask = (set_ccie << FTFA_FCNFG_CCIE_SHIFT ) | (set_rdcollie << FTFA_FCNFG_RDCOLLIE_SHIFT );    
}
/*********************************************************************************************************
** Function name:           ftfa_enable_interrupts
** Descriptions:            Setup static global variables to stop the run_ccob_command function from 
**                          enabling interrupts after it starts a command running.
** input parameters:        set_ccie��set_rdcollie
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void ftfa_disable_interrupts( void )
{
    /*
     *  Make sure the interrupt enable bits are cleared to 0.
     */
    FTFA_FCNFG = FTFA_FCNFG & ~(FTFA_FCNFG_CCIE_MASK | FTFA_FCNFG_RDCOLLIE_MASK);
    ftfa_enable_interrupts_flag = FALSE;
    ftfa_interrupt_mask = 0;
}

#ifdef KEIL
#pragma arm section code = "RAM_CODE" 
#endif
/*********************************************************************************************************
** Function name:           ftfa_run_ccob_cmd
** Descriptions:            All ccob commands are executed by calling this function.
** input parameters:        command��count
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
RUN_IN_RAM INT8U ftfa_run_ccob_cmd(INT8U command[], INT16U count)
{
    INT8U ret_val = 0;
    INT8U idx;
    INT16U cmd[12];

    /*
     *  Clear any previous errors.   Some errors prevent a new command from running.
     */
    FTFA_FSTAT = FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_ACCERR_MASK | FTFA_FSTAT_RDCOLERR_MASK;

    /*
     *  To make the memory map in the debugger cleaner, set all values to 0.  This is optional.
     */
    for(idx = 0; idx < 12; idx++) 
    {
    cmd[idx] = 0;
    } 

    /*
     *  Copy the command into the FTFA module's CCOB registers.
     */
    for(idx = 0; idx < count; idx++) 
    {
    cmd[idx] = command[idx];
    }  
    FTFA_FCCOB0 = cmd[0];  
    FTFA_FCCOB1 = cmd[1];
    FTFA_FCCOB2 = cmd[2];
    FTFA_FCCOB3 = cmd[3];
    FTFA_FCCOB4 = cmd[4];
    FTFA_FCCOB5 = cmd[5];
    FTFA_FCCOB6 = cmd[6];
    FTFA_FCCOB7 = cmd[7];
    FTFA_FCCOB8 = cmd[8];
    FTFA_FCCOB9 = cmd[9];
    FTFA_FCCOBA = cmd[10];  
    FTFA_FCCOBB = cmd[11];
  
    /*
     *  Run command
     */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    if (ftfa_enable_interrupts_flag)
    {
        FTFA_FCNFG = FTFA_FCNFG | ftfa_interrupt_mask;
    }

    /*
     *  If the command complete interrupt is not enabled,
     */
    //if (!(ftfa_interrupt_mask & FTFA_FCNFG_CCIE_MASK))
    {
        /*
         *  wait for command to complete
         */
        while(( FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK) == 0) 
        {
            /*
             *  While waiting, should a callback function be called?
             */
            /*if (ftfa_callback_function != NULL)
            {
                    ftfa_callback_function();
            }*/
        } 
        ret_val = ftfa_check_for_fstat_errors();
    }
  
    /*
     *  Return FSTAT when complete or 0 if interrupts are enabled
     */
    return(ret_val);
}
/*********************************************************************************************************
** Function name:           ftfa_check_for_fstat_errors
** Descriptions:            Return fstat with all bits but the error bits masked off.
**                          Errors bits the calling routine should look out for are:
** input parameters:        command��count
** output parameters:       none
** FTFA_FSTAT_MGSTAT0_MASK  - a lot of commands use this to indicate pass/fail status.
** FTFA_FSTAT_ACCERR_MASK   - usually indicates an invalid command, address or parameter was used.
** FTFA_FSTAT_RDCOLERR_MASK - Accessing the FLEXNVM while erasing the FLEXNVM is one example of this.
** FTFA_FSTAT_FPVIOL_MASK   - protection violation.
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
RUN_IN_RAM INT8U ftfa_check_for_fstat_errors(void)
{
    INT8U ret_val;
      
    ret_val = FTFA_FSTAT;
    ret_val &= (FTFA_FSTAT_FPVIOL_MASK | FTFA_FSTAT_ACCERR_MASK | 
                          FTFA_FSTAT_MGSTAT0_MASK | FTFA_FSTAT_RDCOLERR_MASK);
         
    return ret_val;
}

#ifdef KEIL
#pragma arm section 
#endif
/*
 *  All CCOB address fields are only 23 bits long.   Only the low order address
 *  bits are used to identify the data.   Bit 23 selects the pflash (0) or the
 *  FLEXNVM(1).
 */
INT32U ftfa_convert_addr_to_ccob_addr(INT32U addr)
{
//If address is in the data flash block(s).
//  if (addr >= FTFA_FLEXNVM_START_ADDR) 
//  {
//      addr = (addr ^ FTFA_FLEXNVM_START_ADDR) | FTFA_FLEXNVM_CCOB_START_ADDR;
//  }
    return addr;
}

/*
 *  Suspend erase sector.   TBD -- Not clear whether ERSBLK can be suspended from the spec...
 * Spec doesn't refer to it but seems like it should be...
 */
INT8U suspend_erase(INT8U *suspended)
{
    INT8U ret_val;
    *suspended = FALSE;
  
    /*
     *  Request to suspend.  If the erase completes
     *  before it is suspended, this bit is cleared.
     */
    FTFA_FCNFG |= FTFA_FCNFG_ERSSUSP_MASK;
    
    /*
     *  Waiting on suspend.
     */
    while( (FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK) == 0)
    {
        /*
         *  While waiting, should a callback function be called?
         */
        /*if (ftfa_callback_function != NULL)
        {
            ftfa_callback_function();
        }*/
    }
    
    if (FTFA_FCNFG & FTFA_FCNFG_ERSSUSP_MASK)
    {
        *suspended = TRUE;
    }
    else
    {
        /*
         *  erase completed before being suspended
         */
    }
    
   /*
    *  returns 0 for pass or fstat error bits for fail.
    */
   ret_val = ftfa_check_for_fstat_errors();

   return ret_val;
}

/*
 *  Resume erasing.
 */
INT8U resume_erase(void)
{
    INT8U ret_val;

    if (FTFA_FCNFG & FTFA_FCNFG_ERSSUSP_MASK)
    {
        /*
         *  Resume erase command
         */
        FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;
    }
     
    /*
     *  wait for command to complete
     */
    while(( FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK) == 0) 
    {
        /*
         *  While waiting, should a callback function be called?
         */
        /*if (ftfa_callback_function != NULL)
        {
         ftfa_callback_function();
        }*/
    } 
     
    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    ret_val = ftfa_check_for_fstat_errors();

    return ret_val;
}


/*
 *  abort erasing.
 */
INT8U abort_erase(void)
{
    INT8U ret_val;

    if (FTFA_FCNFG & FTFA_FCNFG_ERSSUSP_MASK)
    {
        /*
         *  The next time a command is executed, the erase
         *  operation that is currently suspended will be 
         *  aborted and the next command will be executed.
         */
        FTFA_FCNFG = FTFA_FCNFG & ~FTFA_FCNFG_ERSSUSP_MASK;
    }

    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    ret_val = ftfa_check_for_fstat_errors();

    return ret_val;
}

/*********************************************************************************************************
** Function name:           FTFA_RD1SEC
** Descriptions:            Read 1s Section.   checks if a section of program flash or data flash memory
**                          is erased to the specified read margin level. The Read 1s Section command 
**                          defines the starting address and the number of 128 bits to be verified.
** input parameters:        add:Flash adr must be 128 bit aligned
**                          num_of_128_bits_to_be_verified:
**                          read_1_margin_choice:
**                                      - FTFA_NORMAL_READ_LEVEL    
**	                                    - FTFA_USER_MARGIN_LEVEL    
**	                                    - FTFA_FACTORY_MARGIN_LEVEL 
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_RD1SEC(INT32U addr, INT16U num_of_128_bits_to_be_verified, INT8U read_1_margin_choice) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;
     
    /*
     *  Don't forget to do this -> 
     */
    addr = ftfa_convert_addr_to_ccob_addr(addr);
    /*
     *  if you are using a pflash/FLEXNVM part.
     */
     
    cmd_ary[0] = FTFA_RD1SEC_CMD;
    cmd_ary[1] = bits_23_16(addr);
    cmd_ary[2] = bits_15_8(addr);
    cmd_ary[3] = bits_7_0(addr);
     
    cmd_ary[4] = bits_15_8(num_of_128_bits_to_be_verified);
    cmd_ary[5] = bits_7_0(num_of_128_bits_to_be_verified);
     
    cmd_ary[6] = read_1_margin_choice;

    ret_val = ftfa_run_ccob_cmd(cmd_ary, 7);        

    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/*********************************************************************************************************
** Function name:           FTFA_PGMCHK
** Descriptions:            Tests a previously programmed program flash or data flash longword to see 
**                          if it reads correctly at the specified margin level. 
**                          defines the starting address and the number of 128 bits to be verified.
** input parameters:        add:Flash adr must be 128 bit aligned
**                          expected_data:
**                          read_1_margin_choice:
**                                      - FTFA_NORMAL_READ_LEVEL    
**	                                    - FTFA_USER_MARGIN_LEVEL    
**	                                    - FTFA_FACTORY_MARGIN_LEVEL 
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_PGMCHK(INT32U addr, INT32U expected_data, INT8U read_1_margin_choice) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;

    /*
     *  on't forget to do this -> 
     */
    addr = ftfa_convert_addr_to_ccob_addr(addr);
    /*
     *  if you are using a pflash/FLEXNVM part.     
     */

    cmd_ary[0] = FTFA_PGMCHK_CMD;
    cmd_ary[1] = bits_23_16(addr);
    cmd_ary[2] = bits_15_8(addr);
    cmd_ary[3] = bits_7_0(addr);

    cmd_ary[4] = read_1_margin_choice;

    cmd_ary[8]   = bits_31_24(expected_data);
    cmd_ary[9]   = bits_23_16(expected_data);
    cmd_ary[0xa] = bits_15_8(expected_data);
    cmd_ary[0xb] = bits_7_0(expected_data);
     
    ret_val = ftfa_run_ccob_cmd(cmd_ary, 12);        

    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/*********************************************************************************************************
** Function name:           FTFA_RDRSRC
** Descriptions:            Provided for the user to read data from special-purpose
**                          memory resources located within the Flash module.
**                          defines the starting address and the number of 128 bits to be verified.
** input parameters:        add:Flash adr must be 64 bit aligned
**                          resource_select_code:
**                                      - FTFA_IFR_RSRC    
**                                      - FTFA_VERSION_ID_RSRC  
**                          data:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_RDRSRC(INT32U addr, INT8U resource_select_code, INT32U *data) 
{
    INT8U cmd_ary[12];
    INT32U ret_val;
    INT32U data0;
     
    /*
     *  initialize return variables.
     */
    *data = 0;
          
    /*
     *  Don't forget to do this -> 
     */
    addr = ftfa_convert_addr_to_ccob_addr(addr); 
    /*
     *  if youare using a pflash/FLEXNVM part.
     */
     
    cmd_ary[0] = FTFA_RDRSRC_CMD;
    cmd_ary[1] = bits_23_16(addr);
    cmd_ary[2] = bits_15_8(addr);
    cmd_ary[3] = bits_7_0(addr);

    cmd_ary[8] = resource_select_code;
     
    ret_val = ftfa_run_ccob_cmd(cmd_ary, 9);        

    if (ret_val == 0)
    {
        /*
         *  Read the data out of the CCOB's
         */
        data0 = (FTFA_FCCOB4 << 24) + (FTFA_FCCOB5 << 16) + (FTFA_FCCOB6 << 8) + FTFA_FCCOB7;

        *data = data0;
    }
        
    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/*********************************************************************************************************
** Function name:           FTFA_PGM4
** Descriptions:            Programs eight previously-erased bytes in the program
**                          flash memory or in the flexNVM flash memory using an embedded algorithm.
** input parameters:        add:Flash adr must be 128 bit aligned
**                          data:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_PGM4(INT32U addr, INT32U data) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;

    /*
     *  Don't forget to do this -> 
     */
    addr = ftfa_convert_addr_to_ccob_addr(addr);
    /*
     *  if you are using a pflash/FLEXNVM part.     
     */
    cmd_ary[0] = FTFA_PGM4_CMD;
    cmd_ary[1] = bits_23_16(addr);
    cmd_ary[2] = bits_15_8(addr);
    cmd_ary[3] = bits_7_0(addr);
                                      
    cmd_ary[4] = bits_31_24(data);     
    cmd_ary[5] = bits_23_16(data);     
    cmd_ary[6] = bits_15_8(data);      
    cmd_ary[7] = bits_7_0(data);       

    cmd_ary[8]   = 0;     
    cmd_ary[9]   = 0;     
    cmd_ary[0xa] = 0;      
    cmd_ary[0xb] = 0;       

    ret_val = ftfa_run_ccob_cmd(cmd_ary, 12);

    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/*********************************************************************************************************
** Function name:           FTFA_ERSSCR
** Descriptions:            Erase Flash Sector.  Erases all 4K byte addresses in a flash sector.
** input parameters:        add:Flash adr must be 128 bit aligned
**                          data:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_ERSSCR( INT32U addr) 
{
   INT8U cmd_ary[12];
   INT8U ret_val;
     
   /*
    *  Don't forget to do this -> 
    */
    addr = ftfa_convert_addr_to_ccob_addr(addr);
   /*
    *  if youare using a pflash/FLEXNVM part.
    */
   cmd_ary[0] = FTFA_ERSSCR_CMD;
   cmd_ary[1] = bits_23_16(addr);
   cmd_ary[2] = bits_15_8(addr);
   cmd_ary[3] = bits_7_0(addr);

   ret_val = ftfa_run_ccob_cmd(cmd_ary, 4);

   /*
    *  returns 0 for pass or fstat error bits for fail.
    */
   return ret_val;     
}
/*********************************************************************************************************
** Function name:           FTFA_RD1ALL
** Descriptions:            Checks if the program flash blocks, data flash blocks,
**                          EEPROM backup records, and data flash IFR have been erased to the specified read
**                          margin level, if applicable, and releases security if the readout passes, 
**                          i.e. all data reads as
** input parameters:        margin_level
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_RD1ALL( INT8U margin_level) 
{
   INT8U cmd_ary[12];
   INT8U ret_val;
   cmd_ary[0] = FTFA_RD1ALL_CMD;
   cmd_ary[1] = margin_level;
   ret_val = ftfa_run_ccob_cmd(cmd_ary, 2);
   /*
    *  returns 0 for pass or fstat error bits for fail.
    */
   return ret_val;     
}
/*********************************************************************************************************
** Function name:           FTFA_RD1ALL
** Descriptions:            Read Once command.  Provides read access to a reserved 64-byte field located in the
**                          program flash 0 IFR (see Program flash 0 IFR map and Program Once field).
** input parameters:        record_index: 0 to 7.
**                          data:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_RDONCE(INT8U record_index, INT32U *data) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;
    INT32U data0;

    *data = 0;

    cmd_ary[0] = FTFA_RDONCE_CMD;
    cmd_ary[1] = record_index;
                                      
    ret_val = ftfa_run_ccob_cmd(cmd_ary, 12);
    
    if (ret_val == 0)
    {
    /*
     *  Read the data out of the CCOB's
     */
    data0 = (FTFA_FCCOB4 << 24) + (FTFA_FCCOB5 << 16) + (FTFA_FCCOB6 << 8) + FTFA_FCCOB7;

    *data = data0;
    }
       
    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/*********************************************************************************************************
** Function name:           FTFA_PGMONCE
** Descriptions:            Program Once command.  enables programming to a reserved 64-byte field in the
**                          program flash 0 IFR (see Program flash 0 IFR map and Program Once field).
** input parameters:        record_index: 0 to 15.
**                          data:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_PGMONCE(INT8U record_index, INT32U data) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;
    cmd_ary[0] = FTFA_PGMONCE_CMD;
    cmd_ary[1] = record_index;
    cmd_ary[4] = bits_31_24(data);     
    cmd_ary[5] = bits_23_16(data);     
    cmd_ary[6] = bits_15_8(data);      
    cmd_ary[7] = bits_7_0(data);            
    ret_val = ftfa_run_ccob_cmd(cmd_ary, 12);
    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/*********************************************************************************************************
** Function name:           FTFA_ERSALL
** Descriptions:            Erase all blocks command.  erases all flash memory, initializes the FlexRAM, 
**                          verifies all memory contents, and releases MCU security.
** input parameters:        record_index: 0 to 15.
**                          data:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_ERSALL(void) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;
    cmd_ary[0] = FTFA_ERSALL_CMD;
    ret_val = ftfa_run_ccob_cmd(cmd_ary, 1);
    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;     
}
/*********************************************************************************************************
** Function name:           FTFA_VFYKEY
** Descriptions:            Verify backdoor access key command.  only executes if the mode and security
**                          conditions are satisfied (see Flash commands by mode). Execution of the Verify
**                          Backdoor Access Key command is further qualified by the FSEC[KEYEN] bits. The
**                          Verify Backdoor Access Key command releases security if user-supplied keys in the
**                          FCCOB match those stored in the Backdoor Comparison Key bytes of the Flash
**                          Configuration Field.
** input parameters:        most_significant_word:
**                          least_significant_word:
** output parameters:       none
** Returned value:          none
** Created by:              Li Xiang
** Created Date:            2012-12-14
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT8U FTFA_VFYKEY(INT32U most_significant_word, INT32U least_significant_word) 
{
    INT8U cmd_ary[12];
    INT8U ret_val;
    cmd_ary[0] = FTFA_VFYKEY_CMD;
    cmd_ary[4] = bits_31_24(most_significant_word);     
    cmd_ary[5] = bits_23_16(most_significant_word);     
    cmd_ary[6] = bits_15_8(most_significant_word);      
    cmd_ary[7] = bits_7_0(most_significant_word);       
    cmd_ary[8]   = bits_31_24(least_significant_word);     
    cmd_ary[9]   = bits_23_16(least_significant_word);     
    cmd_ary[0xa] = bits_15_8(least_significant_word);      
    cmd_ary[0xb] = bits_7_0(least_significant_word);       
    ret_val = ftfa_run_ccob_cmd(cmd_ary, 12);
    /*
     *  returns 0 for pass or fstat error bits for fail.
     */
    return ret_val;
}
/************************/
//Flashд����
/***********************/
void FlashWrite()
{
    INT8U   ucTemp      = 0;
    
    INT32U  ulDataStore[10] ;
    ucTemp = FTFA_ERSSCR(0x10000);//Erease 0x10000
 
    ucTemp = FTFA_RD1SEC(0x10000, 0x100, FTFA_NORMAL_READ_LEVEL); 
    printf("Read 1S :%d\r\n",ucTemp);       
    ucTemp = FTFA_RD1SEC(0x10000, 0x100, FTFA_USER_MARGIN_LEVEL); 
    printf("Read 1S :%d\r\n",ucTemp);       
    ucTemp = FTFA_RD1SEC(0x10000, 0x100, FTFA_FACTORY_MARGIN_LEVEL);   
    printf("Read 1S :%d\r\n",ucTemp);   
    
    printf("Program 0x10000 with 0x11223344 \r\n");   
    ucTemp = FTFA_PGM4(0x10000, MotorParameter.MiddlePositionDuty); 
    printf("Check Programmed with 0x11223344: %d \r\n",ucTemp);      
    ucTemp = FTFA_PGMCHK(0x10000, MotorParameter.MiddlePositionDuty, FTFA_USER_MARGIN_LEVEL);   
    
    ucTemp = FTFA_RDRSRC(0x0,FTFA_VERSION_ID_RSRC,ulDataStore);
    ucTemp = FTFA_RDRSRC(0x4,FTFA_VERSION_ID_RSRC,&ulDataStore[1]);
    printf("Read Vesion ID : %#8x %#8x\r\n",ulDataStore[0],ulDataStore[1]);    
    
    ucTemp = FTFA_RDRSRC(0x0,FTFA_IFR_RSRC,&ulDataStore[2]);
    ucTemp = FTFA_RDRSRC(0x4,FTFA_IFR_RSRC,&ulDataStore[3]); 
    ucTemp = FTFA_RDRSRC(0x8,FTFA_IFR_RSRC,&ulDataStore[4]); 
    
    printf("Read Part IFR : %#8x %#8x %#8x\r\n",ulDataStore[2],ulDataStore[3],ulDataStore[4]);    
    while(ucTemp){
        printf("Some Part Error\r\n");    
    }
    printf("ALL Part OK\r\n");     
}
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/  
 
