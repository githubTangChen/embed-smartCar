/*****************************************
�� �� ����Motor.c
��    ���������������Ƶײ�����
          �Լ�������Ƶײ�����
�޸����ڣ�2016-3-5
*****************************************/
#include "common.h"

//���������PWM��ʼ����������ʼռ�ձ�Ϊ0
//��ʼƵ��Ϊ18000Hz
//ʹ�õ���TMP0��CH0   �� CH1ͨ��
//��Ӧ������   PTE22 �� PTE23
void MotorInit()
{
  uint32 clk_hz ;//ʱ��Ƶ��
  uint16 mod;
  uint8  ps;
  //uint16 cv;
  uint16 freq = 18000;//����Ƶ��
  //uint16 duty = 38;//����ռ�ձ�
  /******************* ����ʱ�� �� ����IO��*******************/
  SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;          //ʹ��TPM2ʱ��
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
  //PTE22
  PORT_PCR_REG(PORTE_BASE_PTR,22) = 0x03 << PORT_PCR_MUX_SHIFT;
  //PTE23
  PORT_PCR_REG(PORTE_BASE_PTR,23) = 0x03 << PORT_PCR_MUX_SHIFT;
  /*       ����Ƶ������        */
  //  �� CPWMS = 1 ����˫�߲�׽���壬�� PMWƵ�� =  pllƵ��/2 /2 /(2^Ԥ��Ƶ����)/ģ��
  //  �� CPWMS = 0 �������߲�׽���壬�� PMWƵ�� =  pllƵ��/2    /(2^Ԥ��Ƶ����)/ģ��
  //  EPWM������ ��MOD - CNTIN + 0x0001   (CNTIN ��Ϊ0)
  //  �����ȣ�CnV - CNTIN
  //  ģ�� MOD < 0x10000
  //  Ԥ��Ƶ���� PS  < 0x07
  //  Ԥ��Ƶ���� PS ԽСʱ��ģ�� mod ��Խ�󣬼�����Խ��׼��PWM�����Ϊ׼ȷ
  //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
  //  �� (((clk_hz/0x10000 )/ freq ) >> PS ) < 1
  // �� CPWMS = 1 ����˫�߲�׽����Ϊ��
  clk_hz = (core_clk_mhz * 1000 * 1000) >> 1 ; // pllƵ�� / 2
  mod = (clk_hz >> 16 ) / freq ;      // ��ʱ�� mod ����һ��
  ps = 0;
  while((mod >> ps) >= 1)             // �� (mod >> ps) < 1 ���˳� while ѭ�� ������ PS ����Сֵ
  {
    ps++;
  }
  mod = (clk_hz >> ps) / freq;// �� MOD ��ֵ
  //cv = (duty * (mod - 0 + 1)) / PRECISON;
  /******************** ѡ�����ģʽΪ ���ض���PWM *******************/
  //ͨ��״̬���ƣ�����ģʽ��ѡ�� ���ػ��ƽ
  //TPM2_CH0
  TPM_CnSC_REG(TPM2_BASE_PTR, 0) &= ~TPM_CnSC_ELSA_MASK;
  TPM_CnSC_REG(TPM2_BASE_PTR, 0)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
  //TPM2_CH1
  TPM_CnSC_REG(TPM2_BASE_PTR, 1) &= ~TPM_CnSC_ELSA_MASK;
  TPM_CnSC_REG(TPM2_BASE_PTR, 1)  =  TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
  // MSnB:MSnA = 1x       ���ض���PWM
  // ELSnB:ELSnA = 10     �ȸߺ��
  // ELSnB:ELSnA = 11     �ȵͺ��
  /******************** ����ʱ�Ӻͷ�Ƶ ********************/
  //  SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
  //CH0
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock
  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
  TPM_SC_REG(TPM2_BASE_PTR)    = ( 0
                              //| TPM_SC_CPWMS_MASK         //0�������ؼ���ģʽ ��1�� �����ؼ���ģʽѡ�� ��ע���˱�ʾ 0��
                                | TPM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                                | TPM_SC_CMOD(1)
                              );
  TPM_MOD_REG(TPM2_BASE_PTR)   = mod;                        //ģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
  TPM_CnV_REG(TPM2_BASE_PTR, 0) = 0;
  TPM_CNT_REG(TPM2_BASE_PTR)   = 0;                          //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ�����������0��
   
  //CH1
  TPM_SC_REG(TPM2_BASE_PTR)    = ( 0
                              //| TPM_SC_CPWMS_MASK         //0�������ؼ���ģʽ ��1�� �����ؼ���ģʽѡ�� ��ע���˱�ʾ 0��
                                | TPM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                                | TPM_SC_CMOD(1)
                              );
  TPM_MOD_REG(TPM2_BASE_PTR)   = mod;                        //ģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
  TPM_CnV_REG(TPM2_BASE_PTR, 1) = 0;
  TPM_CNT_REG(TPM2_BASE_PTR)   = 0;                          //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ�����������0��
}

//������������ת�Լ�ת��
//����:Duty  ռ�ձ�
//Direction ���� 
//ȡֵ  0  ���� ��1  ǰ����2 �ƶ�
void MotorRun(uint32 Duty,char Direction)
{
  if (Duty > MAX_MOTOR_PWM_VAULE) return;
  uint32 cv;
  uint32 mod;
  do
  {
    mod = TPM_MOD_REG(TPM2_BASE_PTR);        //��ȡ MOD ��ֵ
  }
  while(mod == 0);      //��һ�Σ�������0 ����Ҫ���ȡ���Ρ�
  if (Direction == FORWARD) //ǰ��
  {
    cv = (Duty * (mod - 0 + 1)) / PRECISON;
    // ����FTMͨ��ֵ
    TPM_CnV_REG(TPM2_BASE_PTR,0) = cv;
    TPM_CnV_REG(TPM2_BASE_PTR,1) = 0;
  }
  else if (Direction == BACKWARD) //����
  {
    cv = (Duty * (mod - 0 + 1)) / PRECISON; //����ռ�ձ�
    // ����FTMͨ��ֵ
    TPM_CnV_REG(TPM2_BASE_PTR,0) = 0;
    TPM_CnV_REG(TPM2_BASE_PTR,1) = cv;
  }
  else if (Direction == BRAKING) //�ƶ�
  {
    // ����FTMͨ��ֵ
    TPM_CnV_REG(TPM2_BASE_PTR,0) = 0;
    TPM_CnV_REG(TPM2_BASE_PTR,1) = 0;
  }
}
//���PWM��ʼ����������ʼռ�ձ�Ϊ0
//��ʼƵ��Ϊ300Hz
//ʹ�õ���TMP0��CH0ͨ��
//��Ӧ������   PTE24
void ServoMotorInit()
{
  uint32 clk_hz ;//ʱ��Ƶ��
  uint16 mod;
  uint8  ps;
  uint16 freq = 300;//����Ƶ��
  /******************* ����ʱ�� �� ����IO��*******************/
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;       //ʹ��TPM0ʱ��
  SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
  //PTE24
  PORT_PCR_REG(PORTE_BASE_PTR,24) = 0x03 << PORT_PCR_MUX_SHIFT;
  /*       ����Ƶ������        */
  //  �� CPWMS = 1 ����˫�߲�׽���壬�� PMWƵ�� =  pllƵ��/2 /2 /(2^Ԥ��Ƶ����)/ģ��
  //  �� CPWMS = 0 �������߲�׽���壬�� PMWƵ�� =  pllƵ��/2    /(2^Ԥ��Ƶ����)/ģ��
  //  EPWM������ ��MOD - CNTIN + 0x0001   (CNTIN ��Ϊ0)
  //  �����ȣ�CnV - CNTIN
  //  ģ�� MOD < 0x10000
  //  Ԥ��Ƶ���� PS  < 0x07
  //  Ԥ��Ƶ���� PS ԽСʱ��ģ�� mod ��Խ�󣬼�����Խ��׼��PWM�����Ϊ׼ȷ
  //  MOD  = clk_hz/(freq*(1 << PS)) < 0x10000  ==>  clk_hz/(freq*0x10000) < (1<< PS)  ==>  (clk_hz/(freq*0x10000) >> PS) < 1
  //  �� (((clk_hz/0x10000 )/ freq ) >> PS ) < 1
  // �� CPWMS = 1 ����˫�߲�׽����Ϊ��
  clk_hz = (core_clk_mhz * 1000 * 1000) >> 1 ; // pllƵ�� / 2
  mod = (clk_hz >> 16 ) / freq ;      // ��ʱ�� mod ����һ��
  ps = 0;
  while((mod >> ps) >= 1)             // �� (mod >> ps) < 1 ���˳� while ѭ�� ������ PS ����Сֵ
  {
    ps++;
  }
  mod = (clk_hz >> ps) / freq;// �� MOD ��ֵ
  //cv = (duty * (mod - 0 + 1)) / PRECISON;
  /******************** ѡ�����ģʽΪ ���ض���PWM *******************/
  //ͨ��״̬���ƣ�����ģʽ��ѡ�� ���ػ��ƽ
  //TPM0_CH0
  //AB ��ռ�ձ� = 1-Duty
  //AA ��ռ�ձ� = Duty
  TPM_CnSC_REG(TPM0_BASE_PTR, 0) &= ~TPM_CnSC_ELSA_MASK;
  TPM_CnSC_REG(TPM0_BASE_PTR, 0)  = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK;
  // MSnB:MSnA = 1x       ���ض���PWM
  // ELSnB:ELSnA = 10     �ȸߺ��
  // ELSnB:ELSnA = 11     �ȵͺ��
  /******************** ����ʱ�Ӻͷ�Ƶ ********************/
  //  SIM_SOPT2 &= ~ SIM_SOPT2_TPMSRC_MASK;
  //CH0
  SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);        //     00 Clock disabled  01 MCGFLLCLK clock, or MCGPLLCLK/2  10 OSCERCLK clock   11 MCGIRCLK clock
  SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
  TPM_SC_REG(TPM0_BASE_PTR)    = ( 0
                              //| TPM_SC_CPWMS_MASK         //0�������ؼ���ģʽ ��1�� �����ؼ���ģʽѡ�� ��ע���˱�ʾ 0��
                                | TPM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                                | TPM_SC_CMOD(1)
                              );
  TPM_MOD_REG(TPM0_BASE_PTR)   = mod;                        //ģ��, EPWM������Ϊ ��MOD - CNTIN + 0x0001
  TPM_CnV_REG(TPM0_BASE_PTR, 0) = 0;
  TPM_CNT_REG(TPM0_BASE_PTR)   = 0;                          //��������ֻ�е�16λ���ã�д�κ�ֵ���˼Ĵ�����������0��
}
//����ı�ǶȺ���
//����������뷽ʽ�ĸı�ռ�ձ�
//duty��ռ�ձ�
/*
*�������ƫʱPWM��ռ�ձ�Ϊ��ȡ��ʱ�Ƕ�Ϊ0
*���ƫ����Duty��Ӧ��ϵ��
*�Ƕȣ�  
*Dutyֵ��
*PWM��ռ�ձ� = Duty/1000
*/
void ServoMotorChangeDegree(uint32 Duty)
{
  if(Duty > MAX_SERVOMOTOR_PWM_VAULE || Duty < MIN_SERVOMOTOR_PWM_VAULE) return ;//�޷�����ֹ�����ת
  uint32 mod;
  uint32 cv;
  do
  {
    mod = TPM_MOD_REG(TPM0_BASE_PTR);        //��ȡ MOD ��ֵ
  }
  while(mod == 0);      //��һ�Σ�������0 ����Ҫ���ȡ���Ρ�
  cv = (Duty * (mod - 0 + 1)) / PRECISON;
  // ����FTMͨ��ֵ
  TPM_CnV_REG(TPM0_BASE_PTR,0) = cv;
}