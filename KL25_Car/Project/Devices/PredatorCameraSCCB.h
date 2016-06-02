/*****************************************
文 件 名：PredatorCameraSCCB.h
描    述：摄像头SCCB操作
修改日期：2016-3-11
*****************************************/
#ifndef _PREDATORCAMERASCCB_H_
#define _PREDATORCAMERASCCB_H_


//定义PTC的端口  ——by 蓝宙
#define PTC10_OUT    PTC_BASE_PTR->PDORs.PDOR10
#define PTC11_OUT    PTC_BASE_PTR->PDORs.PDOR11
#define PTC11_IN    PTC_BASE_PTR->PDIRs.PDIR11
#define DDRC11      PTC_BASE_PTR->DDRs.DDR11 //方向控制

#define BFSDA      PTC11_OUT
#define BFCLK      PTC10_OUT
#define BFSDAI     PTC11_IN
#define BFDDRA     DDRC11 


#define IICEorr    (0)
#define IICOK      (1)

void BFdelay_1us(uint8_t us) ;
void BFDly_ms(uint8_t ms) ;
void iic_start(void) ;
void iic_stop(void) ;
void slave_ACK(void) ;
void slave_NOACK(void) ;
uint8_t check_ACK(void);
void IICSendByte(uint8_t ch) ;
uint8_t IICreceiveByte(void) ;
uint8_t writeNbyte(uint8_t slave_add, uint8_t *slave_data,uint16_t n) ;
uint8_t receiveNbyte(uint8_t slave_add,uint8_t *rece_data, uint16_t n) ;
uint8_t LandzoIICEEROM_INIT(void) ;
void IICWriteGpio_Init(void);

#endif