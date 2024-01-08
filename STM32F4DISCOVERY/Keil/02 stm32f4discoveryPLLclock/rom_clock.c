#include "rom_clock.h"

// power ပိုင်း လေ့လာပြီး rcc source code အချို့ ကို ပြင်ရန် 

void ___ROM_SystemClock_Config(_Bool use_pc9_clock_output)
{
	ROM_USE_PLL_CLOCK(use_pc9_clock_output);
}

void ROM_USE_PLL_CLOCK(_Bool clock_output)
{
	/* HSE ကို  PLL ရဲ့ source အဖြစ် သုံးပါမည်။ */
	// HSEON bit
	RCC->CR |= ( 1 << 16);
	// HSERDY bit ( clock ကို stable ဖြစ်စေရန် clock cycle အနည်းငယ်စောင့်ပြီး ready bit ကို စောင့်သည်။ )
	while( ! ( RCC->CR & ( 1 << 17 ) ) );
	// PLL ကို ပိတ်ထားမည်။
	RCC->CR &= ~( 1U << 24 );
	// HSE ကို PLL ရဲ့  clock source အဖြစ် သုံးမည်။
	RCC->PLLCFGR |= ( 1 << 22 );
	
	/* PLL 168MHz */
	// reference manual အရ VCO သည် 1 to 2MHz အတွင်း ရှိရမှာမို့  PLLM ကို divide by 8 ထားပြီး  1MHz ရအောင်လုပ်မည်။
	RCC->PLLCFGR &= ~( 0x3FU );
	RCC->PLLCFGR |= ( 1<<2 );           // PLLM = 4
	// VCO input သည် 1MHz ရှိပြီး reference manual အရ PLLN သည် 192 to 432 ရှိရမှာဖြစ်သည်။
	// PLLN ကို 336 ထားပြီး formula အရ  VCO output 336MHz ရသည်။
	RCC->PLLCFGR &= ~( 0x7FC0U);
	RCC->PLLCFGR |= ( 168 << 6 );     // PLLN = 168
	// နောက်တဆင့်အနေနဲ့  PLLP ကို divide by 2 ထားပြီး 168MHz ရပါသည်။
	RCC->PLLCFGR &= ~( 0x03U << 16 ); // PLLP = 2 is 00
	
	// PLLQ
	RCC->PLLCFGR &= ~(15u<<28);
	
	// PLLON bit ( clock ကို stable ဖြစ်စေရန် clock cycle အနည်းငယ်စောင့်ပြီး ready bit ကို စောင့်သည်။ )
	RCC->CR |= ( 1 << 24);

	

	// SW0, SW1 bit များ ကို ကိုင်ပြီး system clock ကို PLL အသုံးပြုခိုင်းသည်။
	RCC->CFGR &= ~(0x3U);  // bit 0 နဲ့ bit 1 ကို 0 ဖြစ်စေ သည်။
	RCC->CFGR |= 0x02;     // 10 က PLL
	
	/* HCLK ကို  prescaler 1 ထားပြီး 168MHz ရစေသည်။ */
	RCC->CFGR &= ~( 0xFU << 4 );

	/* APB1 prescaler 4 , (168/4) = 42MHz */
	RCC->CFGR &= ~( 7U << 10 );
	RCC->CFGR |= ( 5U << 10 );

	/* APB2 prescaler 2 , (168/2) = 84MHz */
	RCC->CFGR &= ~( 7U << 13 );
	RCC->CFGR |= ( 4U << 13 );
	
	if(clock_output)
	{
		// MCO2
		ROM_CLOCK_MEASURE_FROM_PC9_MCO2();
	}
	
	/* Power Interface Clock Enable */
	RCC->APB1ENR |= 1<<28;
	
	/* System configuration controller Clock Enable */
	RCC->APB2ENR |= 1<<14;
	
	// VOS regulator
	//PWR->CSR |= 1<<14;
	
	// FLASH data cache , instruction cache , prefetch enable
	FLASH->ACR |= 1<<8 | 1<<9 | 1<<10; 
	// latency 5 wait states
	FLASH->ACR &= ~(7U<<0);
	FLASH->ACR |= 5u<<0;
	
	// AHB
	RCC->AHB1ENR |= 1 << 2; 		// GPIOC
	
	// ခုကစပြီး MCU သည်  SYSCLK အဖြစ် PLL ကို အသုံးပြုပြီး full speed 168MHz ဖြစ်သည်။
	// PLLRDY bit
	while( ! ( RCC->CR & (1<<25) ) );
	
	if(clock_output)
	{
		ROM_CLOCK_MEASURE_FROM_PC9_GPIO();
	}
}

void ROM_CLOCK_MEASURE_FROM_PC9_GPIO(void)
{
	// clock output //
	
	// pc9 alternate function 
	GPIOC->MODER |= 2<<18;
	
	// pc9 100 MHz
	//GPIOC->OSPEEDR |= 3<<18; // no need
}
void ROM_CLOCK_MEASURE_FROM_PC9_MCO2(void)
{
	// သူက  MCU ရဲ့  clock output ဖြစ် ပါတယ်။ ဒီဖန်ရှင်ကို clock setup မလုပ်ခင် run ရပါမယ်။

	// MCO2 --> သူသည် MCO2 clocu output 2 ဖြစ်ပါတယ်။ clock source သည် 
	// SYSCLK ဆိုရင် 00 ,  
	// PLLI2S ဆိုရင် 01 , 
	// HSE ဆိုရင် 10 , 
	// PLL ဆိုရင် 11 ဖြစ်ပြီး SYSCLK ရွေး ထားသည်။
	RCC->CFGR &= ~(1u << 30);
	RCC->CFGR &= ~(1u << 31);

	// MCO2PRE --> သူသည် prescaler ဖြစ်ပြီး 0 ဆို no division, 
	// 100 ဆို divide by 2, 
	// 101 ဆို divide by 3, 
	// 110 ဆို divide by 4, ( 42MHz )
	// 111 ဆို divide by 5  ( 33.6MHz) 
	// ယခု 110 ရွေးထားသည်။ 168/4 = 42 MHz 
	RCC->CFGR &= ~(7u<<27); // clear
	RCC->CFGR |= 6u<<27;    // 7(33MHz) , 6(42MHz), [ 5(56MHz) --> can't generate ]
	
}
