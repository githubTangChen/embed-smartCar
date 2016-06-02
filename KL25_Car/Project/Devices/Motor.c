/*****************************************
文 件 名：Motor.c
描    述：马达驱动板控制底层驱动
          以及舵机控制底层驱动
修改日期：2016-3-5
*****************************************/
#include "common.h"

//马达驱动板PWM初始化函数，初始占空比为0
//初始频率为18000Hz
//使用的是TMP0的CH0   和 CH1通道
//对应引脚是   PTE22 和 PTE23
void MotorInit()
{
  uint32 clk_hz ;//时钟频率
  uint16 mod;
  uint8  ps;
  //uint16 cv;
  uint16 freq = 18000;//设置频率
  //uint16 duty = 38;//设置占空比
  /******************* 开启时钟 和 复用IO口*******************/
  SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;          //使能TPM2时钟
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
  //PTE22
  PORT_PCR_REG(PORTE_BASE_PTR,22) = 0x03 << PORT_PCR_MUX_SHIFT;
  //PTE23
  PORT_PCR_REG(PORTE_BASE_PTR,23) = 0x03 << PORT_PCR_MUX_SHIFT;
  /*       计算频率设置        */
  //  若 CPWMS = 1 ，即双边捕捉脉冲，则 PMW频率 =  pll频率/2 /2 /(2^预分频因子)/模数
  //  若 CPWMS = 0 ，即单边捕捉脉冲，则 PMW频率 =  pll频率/2    /(2^预分频因子)/模数
  //  EPWM的周期 ：MOD - CNTIN + 0x0001   (CNTIN 设为0)
  //  脉冲宽度：CnV - CNTIN
  //  模数 MOD < 0x10000
  //  预分频因子 PS  < 0x07
  //  预分频因子 PS 越小时，模数 mod 就越大，计数就越精准，PWM输出更为准确
  //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
  //  即 (((clk_hz/0x10000 )/ freq ) >> PS ) < 1
  // 以 CPWMS = 1 ，即双边捕捉脉冲为例
  clk_hz = (core_clk_mhz * 1000 * 1000) >> 1 ; // pll频率 / 2
  mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
  ps = 0;
  while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
  {
    ps++;
  }
  mod = (clk_hz >> ps) / freq;// 求 MOD 的值
  //cv = (duty * (mod - 0 + 1)) / PRECISON;
  /******************** 选择输出模式为 边沿对齐PWM *******************/
  //通道状态控制，根据模式来选择 边沿或电平
  //TPM2_CH0
  TPM_CnSC_REG(TPM2_BASE_PTR, 0) &= ~TPM_CnSC_ELSA_MASK;
  TPM_CnSC_REG(TPM2_BASE_PTR, 0)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
  //TPM2_CH1
  TPM_CnSC_REG(TPM2_BASE_PTR, 1) &= ~TPM_CnSC_ELSA_MASK;
  TPM_CnSC_REG(TPM2_BASE_PTR, 1)  =  TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
  // MSnB:MSnA = 1x       边沿对齐PWM
  // ELSnB:ELSnA = 10     先高后低
  // ELSnB:ELSnA = 11     先低后高
  /******************** 配置时钟和分频 ********************/
  //  SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
  //CH0
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock
  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
  TPM_SC_REG(TPM2_BASE_PTR)    = ( 0
                              //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                | TPM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                | TPM_SC_CMOD(1)
                              );
  TPM_MOD_REG(TPM2_BASE_PTR)   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
  TPM_CnV_REG(TPM2_BASE_PTR, 0) = 0;
  TPM_CNT_REG(TPM2_BASE_PTR)   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
   
  //CH1
  TPM_SC_REG(TPM2_BASE_PTR)    = ( 0
                              //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                | TPM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                | TPM_SC_CMOD(1)
                              );
  TPM_MOD_REG(TPM2_BASE_PTR)   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
  TPM_CnV_REG(TPM2_BASE_PTR, 1) = 0;
  TPM_CNT_REG(TPM2_BASE_PTR)   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}

//控制马达的正反转以及转速
//参数:Duty  占空比
//Direction 方向 
//取值  0  后退 ，1  前进，2 制动
void MotorRun(uint32 Duty,char Direction)
{
  if (Duty > MAX_MOTOR_PWM_VAULE) return;
  uint32 cv;
  uint32 mod;
  do
  {
    mod = TPM_MOD_REG(TPM2_BASE_PTR);        //读取 MOD 的值
  }
  while(mod == 0);      //读一次，可能是0 ，需要多读取几次。
  if (Direction == FORWARD) //前进
  {
    cv = (Duty * (mod - 0 + 1)) / PRECISON;
    // 配置FTM通道值
    TPM_CnV_REG(TPM2_BASE_PTR,0) = cv;
    TPM_CnV_REG(TPM2_BASE_PTR,1) = 0;
  }
  else if (Direction == BACKWARD) //后退
  {
    cv = (Duty * (mod - 0 + 1)) / PRECISON; //设置占空比
    // 配置FTM通道值
    TPM_CnV_REG(TPM2_BASE_PTR,0) = 0;
    TPM_CnV_REG(TPM2_BASE_PTR,1) = cv;
  }
  else if (Direction == BRAKING) //制动
  {
    // 配置FTM通道值
    TPM_CnV_REG(TPM2_BASE_PTR,0) = 0;
    TPM_CnV_REG(TPM2_BASE_PTR,1) = 0;
  }
}
//舵机PWM初始化函数，初始占空比为0
//初始频率为300Hz
//使用的是TMP0的CH0通道
//对应引脚是   PTE24
void ServoMotorInit()
{
  uint32 clk_hz ;//时钟频率
  uint16 mod;
  uint8  ps;
  uint16 freq = 300;//设置频率
  /******************* 开启时钟 和 复用IO口*******************/
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;       //使能TPM0时钟
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
  //PTE24
  PORT_PCR_REG(PORTE_BASE_PTR,24) = 0x03 << PORT_PCR_MUX_SHIFT;
  /*       计算频率设置        */
  //  若 CPWMS = 1 ，即双边捕捉脉冲，则 PMW频率 =  pll频率/2 /2 /(2^预分频因子)/模数
  //  若 CPWMS = 0 ，即单边捕捉脉冲，则 PMW频率 =  pll频率/2    /(2^预分频因子)/模数
  //  EPWM的周期 ：MOD - CNTIN + 0x0001   (CNTIN 设为0)
  //  脉冲宽度：CnV - CNTIN
  //  模数 MOD < 0x10000
  //  预分频因子 PS  < 0x07
  //  预分频因子 PS 越小时，模数 mod 就越大，计数就越精准，PWM输出更为准确
  //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
  //  即 (((clk_hz/0x10000 )/ freq ) >> PS ) < 1
  // 以 CPWMS = 1 ，即双边捕捉脉冲为例
  clk_hz = (core_clk_mhz * 1000 * 1000) >> 1 ; // pll频率 / 2
  mod = (clk_hz >> 16 ) / freq ;      // 临时用 mod 缓存一下
  ps = 0;
  while((mod >> ps) >= 1)             // 等 (mod >> ps) < 1 才退出 while 循环 ，即求 PS 的最小值
  {
    ps++;
  }
  mod = (clk_hz >> ps) / freq;// 求 MOD 的值
  //cv = (duty * (mod - 0 + 1)) / PRECISON;
  /******************** 选择输出模式为 边沿对齐PWM *******************/
  //通道状态控制，根据模式来选择 边沿或电平
  //TPM0_CH0
  //AB ：占空比 = 1-Duty
  //AA ：占空比 = Duty
  TPM_CnSC_REG(TPM0_BASE_PTR, 0) &= ~TPM_CnSC_ELSA_MASK;
  TPM_CnSC_REG(TPM0_BASE_PTR, 0)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
  // MSnB:MSnA = 1x       边沿对齐PWM
  // ELSnB:ELSnA = 10     先高后低
  // ELSnB:ELSnA = 11     先低后高
  /******************** 配置时钟和分频 ********************/
  //  SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
  //CH0
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock
  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
  TPM_SC_REG(TPM0_BASE_PTR)    = ( 0
                              //| TPM_SC_CPWMS_MASK         //0：上升沿计数模式 ，1： 跳变沿计数模式选择 （注释了表示 0）
                                | TPM_SC_PS(ps)             //分频因子，分频系数 = 2^PS
                                | TPM_SC_CMOD(1)
                              );
  TPM_MOD_REG(TPM0_BASE_PTR)   = mod;                        //模数, EPWM的周期为 ：MOD - CNTIN + 0x0001
  TPM_CnV_REG(TPM0_BASE_PTR, 0) = 0;
  TPM_CNT_REG(TPM0_BASE_PTR)   = 0;                          //计数器。只有低16位可用（写任何值到此寄存器，都会清0）
}
//舵机改变角度函数
//适用于左对齐方式的改变占空比
//duty：占空比
/*
*舵机最右偏时PWM的占空比为，取此时角度为0
*舵机偏角与Duty对应关系表
*角度：  
*Duty值：
*PWM的占空比 = Duty/1000
*/
void ServoMotorChangeDegree(uint32 Duty)
{
  if(Duty > MAX_SERVOMOTOR_PWM_VAULE || Duty < MIN_SERVOMOTOR_PWM_VAULE) return ;//限幅，防止舵机堵转
  uint32 mod;
  uint32 cv;
  do
  {
    mod = TPM_MOD_REG(TPM0_BASE_PTR);        //读取 MOD 的值
  }
  while(mod == 0);      //读一次，可能是0 ，需要多读取几次。
  cv = (Duty * (mod - 0 + 1)) / PRECISON;
  // 配置FTM通道值
  TPM_CnV_REG(TPM0_BASE_PTR,0) = cv;
}