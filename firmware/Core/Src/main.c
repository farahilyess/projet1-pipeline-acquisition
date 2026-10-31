#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "MPU6050.h"
#include "FreeRTOS.h"
#include "task.h"

#define USART_TASK_PERIOD_MS (500UL)

static void convertHexa(uint8_t entree, char* MSB_sortie, char* LSB_sortie){

	uint8_t quartet_haut = (entree >> 4);
	uint8_t quartet_bas = (entree & 0x0FU);

	// Doit aussi etre positif mais comme ce sont des uint la verif est inutile
	if(quartet_haut <= 9U){
		*MSB_sortie = quartet_haut + '0';
	}
	else{
		*MSB_sortie = (quartet_haut - 10U) + 'A';
	}
	// Idem
	if(quartet_bas <= 9U){
		*LSB_sortie = quartet_bas + '0';
	}
	else{
		*LSB_sortie = (quartet_bas - 10U) + 'A';
	}

}


static void usartTask(void * parameters) __attribute__((noreturn));

static void usartTask(void * parameters){
    /* Unused parameters. */
    (void) parameters;
    uint8_t msg = 'Y';

    for(;;)
    {
    	usartSend(msg);
        vTaskDelay(USART_TASK_PERIOD_MS);
    }
}

int main(void){
    static StaticTask_t usartTaskTCB;
    static StackType_t usartTaskStack[configMINIMAL_STACK_SIZE];

	usartInit();

    (void)xTaskCreateStatic(&usartTask,
    						   "USART",
			  configMINIMAL_STACK_SIZE,
			                      NULL,
             configMAX_PRIORITIES - 1U,
			      &(usartTaskStack[0]),
				      &(usartTaskTCB));

    /* Start the scheduler. */
    vTaskStartScheduler();

    for(;;){

    }


}
