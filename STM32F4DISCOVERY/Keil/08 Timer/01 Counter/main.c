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

static _Bool use_clock_output_for_oscilloscope = 0;

int main(void) 
{
	___ROM_Init();
	___ROM_SystemClock_Config(use_clock_output_for_oscilloscope);
	
	___ROM_GPIO_INIT();
	___ROM_PA0_EXTI_Init();
	___ROM_ROS2_HZ(1000);
	
	while(1)
	{
		
		
	}
}

void ___ROM_Init(void)
{
	//___ROM_SysTick_Init();
}


