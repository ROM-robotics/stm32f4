//	8888888b.   .d88888b.  888b     d888                                    
//	888   Y88b d88P" "Y88b 8888b   d8888                                    
//	888    888 888     888 88888b.d88888                                    
//	888   d88P 888     888 888Y88888P888                                    
//	8888888P"  888     888 888 Y888P 888                                    
//	888 T88b   888     888 888  Y8P  888                                    
//	888  T88b  Y88b. .d88P 888   "   888                                    
//	888   T88b  "Y88888P"  888       888                                    
//	                                                                        
//	                                                                
//	                                                                        
//	     888                                          d8b                   
//	     888                                          Y8P                   
//	     888                                                                
//	 .d88888 888  888 88888b.   8888b.  88888b.d88b.  888  .d8888b .d8888b  
//	d88" 888 888  888 888 "88b     "88b 888 "888 "88b 888 d88P"    88K      
//	888  888 888  888 888  888 .d888888 888  888  888 888 888      "Y8888b. 
//	Y88b 888 Y88b 888 888  888 888  888 888  888  888 888 Y88b.         X88 
//	 "Y88888  "Y88888 888  888 "Y888888 888  888  888 888  "Y8888P  88888P' 
//	              888                                                       
//	         Y8b d88P                                                       
//	          "Y88P"                                                        
// *************************************************************************
#include "main.h"

static _Bool use_clock_output_for_oscilloscope = 1;
int main(void) 
{
	//___ROM_Init();
	___ROM_SystemClock_Config(use_clock_output_for_oscilloscope);
	
	while(1)
	{
		
		
	}
}

void ___ROM_Init(void)
{
	NVIC_SetPriorityGrouping(0x00000003U);
	
	if( SysTick_Config(SystemCoreClock / 1000U) == 0Ul ) { while(1) {} }
	
	uint32_t prioritygroup = 0x00U;
	prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(prioritygroup, 15U, 0U));
}


