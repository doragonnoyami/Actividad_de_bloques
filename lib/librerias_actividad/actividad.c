#include "ACTIVIDAD.h"
#include "stm32f103xb.h"

#define MUX1_IN_0 0
#define MUX1_IN_1 1
#define MUX1_SEL 2
#define MUX1_OUT 0

#define MUX2_IN_0 4
#define MUX2_IN_1 5
#define MUX2_SEL 6
#define MUX2_OUT 5

#define CONT_CLK 3
#define CONT_OUT_0 1
#define CONT_OUT_1 2
#define CONT_OUT_2 3
#define CONT_OUT_3 4

#define SUM_A0 9
#define SUM_A1 10
#define SUM_B0 11
#define SUM_B1 12
#define SUM_CIN 13
#define SUM_Z0 12
#define SUM_Z1 13
#define SUM_COUT 14

int pa[12]={0, 1, 2, 3, 4, 5, 6, 9, 10, 11, 12, 13};
int pb[9]={0, 1, 2, 3, 4, 5, 12, 13, 14};
void actividad_init(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;   
    for(int i=0; i<12; i++){
        if(i<7){
        GPIOA->CRL &= ~(0xF<<(pa[i]*4));
        GPIOA->CRL |=  (0b0100 << (pa[i] * 4));
        }else if(i>=7){
            GPIOA->CRH &= ~(0xF<<((pa[i]%8)*4));
            GPIOA->CRH |=  (0b0100 << ((pa[i]%8) * 4));
        }
    }
    for(int i=0; i<16; i++){
        if(i<6){
            GPIOB->CRL &= ~(0xF<<(pb[i]*4));
            GPIOB->CRL |=  (0b0010 << (pb[i] * 4));
        }else if(i>=6){
            GPIOB->CRH &= ~(0xF<<((pb[i]%8)*4));
            GPIOB->CRH |=  (0b0010 << ((pb[i]%8) * 4));
        }
    }
}
void mux1(){
    uint16_t est_sel = GPIOA -> IDR&(1<<MUX1_SEL);
    if(est_sel==0){
        uint16_t est_in0 = GPIOA -> IDR&(1<<MUX1_IN_0);
        if(est_in0==0){
            GPIOB->BSRR |= (1 << MUX1_OUT); 
        }else{
            GPIOB->BSRR |= (1 << MUX1_OUT+16); 
        }
    }else{
        uint16_t est_in1 = GPIOA -> IDR&(1<<MUX1_IN_1);
        if(est_in1==0){
            GPIOB->BSRR |= (1 << MUX1_OUT); 
        }else{
            GPIOB->BSRR |= (1 << MUX1_OUT+16); 
        }
    }
}
void mux2(){
    uint16_t est_sel = GPIOA -> IDR&(1<<MUX2_SEL);
    if(est_sel==0){
        uint16_t est_in0 = GPIOA -> IDR&(1<<MUX2_IN_0);
        if(est_in0==0){
            GPIOB->BSRR |= (1 << MUX2_OUT); 
        }else{
            GPIOB->BSRR |= (1 << MUX2_OUT+16); 
        }
    }else{
        uint16_t est_in1 = GPIOA -> IDR&(1<<MUX2_IN_1);
        if(est_in1==0){
            GPIOB->BSRR |= (1 << MUX2_OUT); 
        }else{
            GPIOB->BSRR |= (1 << MUX2_OUT+16); 
        }
    }
}
void cont(){
    int pos=0;
    int outs[4]={CONT_OUT_0, CONT_OUT_1, CONT_OUT_2, CONT_OUT_3};
    uint16_t est_clk = GPIOA -> IDR&(1<<CONT_CLK);
    if(est_clk==0){
        for(int i=0; i<4; i++){
            GPIOA->BSRR |= (1 << outs[i]+16);
        }
    }else{
        for(int i=0; i<4; i++){
            GPIOA->BSRR |= (1 << outs[i]+16);
        }
        GPIOA->BSRR |= (1 << outs[pos]);
        pos++;
        pos=pos%4;
        delay_ms(500);
    }
}
void sum(){
    uint16_t est_a0 = GPIOA -> IDR&(1<<SUM_A0);
    uint16_t est_a1 = GPIOA -> IDR&(1<<SUM_A1);
    uint16_t est_b0 = GPIOA -> IDR&(1<<SUM_B0);
    uint16_t est_b1 = GPIOA -> IDR&(1<<SUM_B1);
    uint16_t est_cin = GPIOA -> IDR&(1<<SUM_CIN);

    int aux=0;
    
    if(est_a0==est_b0){
        GPIOB->BSRR |= (1 <<(SUM_Z0+16));
    }else{
        GPIOB->BSRR |= (1 <<SUM_Z0);
    }
    if(est_a1== est_b1){ 
        if(est_a0 && est_b0){ //Si est_a0 y est_b0 es 0
            GPIOB->BSRR |= (1 <<SUM_Z1);
        }else{
            GPIOB->BSRR |= (1 <<(SUM_Z1+16));
        }
    }else{
        if(est_a1 || est_b1){ //Si est_a1 o est_b1 es 1
            if(est_a0 && est_b0){ 
                GPIOB->BSRR |= (1 <<(SUM_Z1+16));
                aux=1;
            }else{
                GPIOB->BSRR |= (1 <<SUM_Z1);
            }
        }
    }
    if(est_cin == (aux)){
        GPIOB->BSRR |= (1 <<(SUM_COUT+16));
    }else{
        GPIOB->BSRR |= (1 <<SUM_COUT);
    }
}
