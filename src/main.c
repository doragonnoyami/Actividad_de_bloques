#include "stm32f103xb.h"
#include "Actividad.h"

int main(void){
    actividad_init();
    while(1){
        mux1();
        mux2();
        cont();
        sum();
    }
}