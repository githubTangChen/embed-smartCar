/*****************************************
文 件 名：PIT.c
描    述：定时器驱动
作    者：landzo 蓝电子
*****************************************/
#include "common.h"
#include "PIT.h"


extern uint8_t TIMEflag_5ms   ;
extern uint8_t TIMEflag_10ms  ;
extern uint8_t TIMEflag_20ms  ;
extern uint8_t TIMEflag_30ms  ;
extern uint8_t TIMEflag_500ms ;
extern uint8_t TIMEflag_300ms ;

uint16_t  TimeCount = 0 ;

//PIT0定时器初始化
//每cnt/1000 ms触发一次中断
void PIT_Init(uint32_t cnt)
{
  cnt = cnt * bus_clk_khz/1000;
  SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;          //使能PIT时钟

  PIT_MCR  = (0
              //| PIT_MCR_MDIS_MASK       //禁用PIT定时器时钟选择（0表示使能PIT，1表示禁止PIT）
              //| PIT_MCR_FRZ_MASK        //调试模式下停止运行（0表示继续运行，1表示停止运行）
               );
  PIT_LDVAL(0)  = cnt - 1 ;                    //设置溢出中断时间

  PIT_TFLG(0) |= PIT_TFLG_TIF_MASK;        //清中断标志（写1 清空标志位）                          //清中断标志位

  PIT_TCTRL(0) &= ~ PIT_TCTRL_TEN_MASK;        //禁止PITn定时器（用于清空计数值）
  PIT_TCTRL(0)  = ( 0
                    | PIT_TCTRL_TEN_MASK        //使能 PITn定时器
                    | PIT_TCTRL_TIE_MASK        //开PITn中断
                  );
  enable_irq(PIT_irq_no);            //开中断
  set_irq_priority(PIT_irq_no,3) ;
}

/*************************************************************************
*                             蓝宙电子工作室
*
*  函数名称：PIT_IRQHandler
*  功能说明：PIT0 定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：
*************************************************************************/

void PIT_IRQHandler(void)
{
  //清中断标志（写1 清空标志位）
  PIT_TFLG(0) |= PIT_TFLG_TIF_MASK;
  TimeCount ++ ;
  if(TimeCount%5 == 0 )
  {
    TIMEflag_5ms = 1;
  }
  if(TimeCount%10 == 0 )
  {
    TIMEflag_10ms = 1;    
  }
  if(TimeCount%20 == 0 )
  {
    TIMEflag_20ms = 1;
  }
  if(TimeCount%30 == 0 )
  {
    TIMEflag_30ms = 1;
  }
  if(TimeCount%300 == 0 )
  {
    TIMEflag_300ms = 1;
  }
  if(TimeCount%500 == 0 )
  {
    TIMEflag_500ms = 1;
    TimeCount = 0 ;
    
  }
}
