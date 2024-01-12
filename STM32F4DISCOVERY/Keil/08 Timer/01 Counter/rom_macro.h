/*******************************************************************************
 *                    ROM ROBOTICS Co,.Ltd ( Myanmar )    										 *
 *			(?) COPYLEFT 2023		| www.romrobots.com | (+95) 9-259-288-229 				 *
 *******************************************************************************/ 
#ifndef __ROM_MACRO_H
#define __ROM_MACRO_H
// ************************************** ROM DYNAMIC MACRO
#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define ROM_GPIOA_ODR_Address (GPIOA_BASE + 0x14)
#define ROM_GPIOB_ODR_Address (GPIOB_BASE + 0x14)
#define ROM_GPIOC_ODR_Address (GPIOC_BASE + 0x14)
#define ROM_GPIOD_ODR_Address (GPIOD_BASE + 0x14)
#define ROM_GPIOE_ODR_Address (GPIOE_BASE + 0x14)

#define ROM_GPIOA_IDR_Address (GPIOA_BASE + 0x10)
#define ROM_GPIOB_IDR_Address (GPIOB_BASE + 0x10)
#define ROM_GPIOC_IDR_Address (GPIOC_BASE + 0x10)
#define ROM_GPIOD_IDR_Address (GPIOD_BASE + 0x10)
#define ROM_GPIOE_IDR_Address (GPIOE_BASE + 0x10)

#define PAout(n)   BIT_ADDR(ROM_GPIOA_ODR_Address,n)  
#define PAin(n)    BIT_ADDR(ROM_GPIOA_IDR_Address,n) 

#define PBout(n)   BIT_ADDR(ROM_GPIOB_ODR_Address,n)  
#define PBin(n)    BIT_ADDR(ROM_GPIOB_IDR_Address,n) 

#define PCout(n)   BIT_ADDR(ROM_GPIOC_ODR_Address,n)  
#define PCin(n)    BIT_ADDR(ROM_GPIOC_IDR_Address,n) 

#define PDout(n)   BIT_ADDR(ROM_GPIOD_ODR_Address,n)  
#define PDin(n)    BIT_ADDR(ROM_GPIOD_IDR_Address,n) 

#define PEout(n)   BIT_ADDR(ROM_GPIOE_ODR_Address,n)  
#define PEin(n)    BIT_ADDR(ROM_GPIOE_IDR_Address,n) 

#define green_led 	PDout(12)
#define orange_led 	PDout(13)
#define red_led 		PDout(14)
#define blue_led 		PDout(15)

// for exti pr bit to clear
#define __ROM_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__) (EXTI->PR = (__EXTI_LINE__))

#endif
// ************************************** ROM DYNAMIC MACRO
