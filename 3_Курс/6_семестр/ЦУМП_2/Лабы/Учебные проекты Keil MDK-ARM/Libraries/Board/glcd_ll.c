/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : glcd_ll.c
 *    Description : GLCD low level functions
 *
 *    History :
 *    1. Date        : December 2, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    2. ????        : 2013
 *       ?????       : ?????
 *       ????????    : ?????? ???????????? "includes.h", ????????? "arm_comm.h",
 *                     "stm32f4xx_gpio.h", "stm32f4xx_tim.h"
 *
 *    3. ????        : ????, 2015
 *       ?????       : ?????
 *       ????????    : ????????????? ?????? ???????? ??? ?????????:
 *                     - ?? ?????????                 : TIM3_CH3
 *                     - ??? ?????????? "USE_LCD_TIM1": TIM1_CH2N 
 *                     - ??? ?????????? "USE_LCD_TIM8": TIM8_CH2N
 *
 *    4. ????        : ???, 2019
 *       ?????       : ?????
 *       ????????    : ???????? ?????? ?? SPI ???????? ? ???????????? GLCD_delay().
 *
 *
**************************************************************************/
#include "arm_comm.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "glcd_ll.h"

static Int32U Width;

/*************************************************************************
 * ???????? ??? ???????????? SPI
 *************************************************************************/
static int glcd_cnt_delay;
static __inline void GLCD_delay() 
{ 
  glcd_cnt_delay = 1;
  while (--glcd_cnt_delay);
}

/*************************************************************************
 * Function Name: GLCD_SetReset
 * Parameters: Boolean State
 * Return: none
 *
 * Description: Set reset pin state
 *
 *************************************************************************/
void GLCD_SetReset (Boolean State)
{
  GLCD_delay();
  GPIO_WriteBit(LCD_RST_PORT,LCD_RST_MASK,(State)?Bit_SET:Bit_RESET);
  GLCD_delay();
}

/*************************************************************************
 * Function Name: GLCD_SetBacklight
 * Parameters: Int8U Light
 * Return: none
 *
 * Description: Set backlight pin state
 *
 *************************************************************************/
void GLCD_Backlight (Int8U Light)
{
#if defined (USE_LCD_TIM1) || defined (USE_LCD_TIM8)
  LCD_TIM->CCR2 = Light;
#else
  LCD_TIM->CCR3 = Light;
#endif  
}

/*************************************************************************
 * Function Name: GLCD_LLInit
 * Parameters: none
 * Return: none
 *
 * Description: Init Reset and Backlight control outputs
 *
 *************************************************************************/
void GLCD_LLInit (void)
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

  /* Enable GPIO clock and release reset*/
  RCC_AHB1PeriphClockCmd(LCD_RST_CLK | LCD_BL_CLK, ENABLE);
  RCC_AHB1PeriphResetCmd(LCD_RST_CLK | LCD_BL_CLK,DISABLE);

  /*LCD Reset pin init*/  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

  GPIO_InitStructure.GPIO_Pin = LCD_RST_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

  GLCD_SetReset(0);

	/* LCD backlight Init*/ 
  // PWM DAC (TIMx_CHx)
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = LCD_BL_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_BL_PORT, &GPIO_InitStructure);

  GPIO_PinAFConfig(LCD_BL_PORT,LCD_BL_PIN_SOURCE,LCD_BL_PIN_AF);

  // Init PWM TIM3, TIM1 or TIM8
  // Enable Timer clock and release reset
#if defined (USE_LCD_TIM1) || defined (USE_LCD_TIM8)
  RCC_APB2PeriphClockCmd(RCC_APBPeriph_TIM,ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APBPeriph_TIM,DISABLE);
#else
  RCC_APB1PeriphClockCmd(RCC_APBPeriph_TIM,ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APBPeriph_TIM,DISABLE);
#endif
  TIM_InternalClockConfig(LCD_TIM);

  // Time base configuration
  TIM_TimeBaseStructure.TIM_Prescaler = 140;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 0xFF; // 8 bit resolution
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(LCD_TIM,&TIM_TimeBaseStructure);

  // Channel Configuration in PWM mode
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x00;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
#if defined (USE_LCD_TIM1) || defined (USE_LCD_TIM8)
  TIM_OC2Init(LCD_TIM,&TIM_OCInitStructure);
  TIM_CtrlPWMOutputs(LCD_TIM, ENABLE);
#else
  TIM_OC3Init(LCD_TIM,&TIM_OCInitStructure);
#endif  
  // Double buffered
  TIM_ARRPreloadConfig(LCD_TIM,ENABLE);
  // TIM counter enable
  TIM_Cmd(LCD_TIM,ENABLE);

  GLCD_Backlight(0);
}

/*************************************************************************
 * Function Name: LcdSpiChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: SPI Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void GLCD_SPI_ChipSelect (Boolean Select)
{
  //GLCD_delay();
  GPIO_WriteBit(LCD_CS_PORT,LCD_CS_MASK,(Select)?Bit_RESET:Bit_SET);
  //GLCD_delay();
}

/*************************************************************************
 * Function Name: LcdSpiSetWordWidth
 * Parameters: Int32U Data
 * Return: Boolean
 *
 * Description: Set SPI word width
 *
 *************************************************************************/
Boolean GLCD_SPI_SetWordWidth (Int32U Data)
{
  if(   (8 != Data)
     && (9 != Data))
  {
    return(FALSE);
  }

  Width = Data;
  return(TRUE);
}

/*************************************************************************
 * Function Name: LcdSpiSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI clock
 *
 *************************************************************************/
/*
Int32U GLCD_SPI_SetClockFreq (Int32U Frequency)
{
Int32U Fspi = SYS_GetFpclk(Ssp1_PCLK_OFFSET);
Int32U Div = 2;
  while((Div * Frequency) < Fspi)
  {
    if((Div += 2) == 254)
    {
      break;
    }
  }
  Ssp1CPSR = Div;
  return(Fspi/Div);
}
*/
/*************************************************************************
 * Function Name: GLCD_SPI_Init
 * Parameters: Int32U Clk, Int32U Width
 * Return: none
 *
 * Description: Init SPI
 *
 *************************************************************************/
void GLCD_SPI_Init(Int32U Clk, Int32U Width)
{
GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable GPIO clock and release reset*/
  RCC_AHB1PeriphClockCmd(LCD_CS_CLK | LCD_SPI_MISO_CLK | 
                         LCD_SPI_MOSI_CLK | LCD_SPI_SCLK_CLK, ENABLE);
  RCC_AHB1PeriphResetCmd(LCD_CS_CLK | LCD_SPI_MISO_CLK | 
                         LCD_SPI_MOSI_CLK | LCD_SPI_SCLK_CLK, DISABLE);

  /*LCD_CS*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

  GPIO_InitStructure.GPIO_Pin = LCD_CS_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
  /*LCD_SPI_SCLK*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCLK_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_SPI_SCLK_PORT, &GPIO_InitStructure);

  /*LCD_SPI_MOSI*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_SPI_MOSI_PORT, &GPIO_InitStructure);

  /*LCD_SPI_MISO*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

  GPIO_InitStructure.GPIO_Pin = LCD_SPI_MISO_MASK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_SPI_MISO_PORT, &GPIO_InitStructure);
  // Chip select
  GLCD_SPI_ChipSelect(0);
  GLCD_SPI_CLK_H();

  // Chip select
  GLCD_SPI_ChipSelect(FALSE);

  // Set data width
  GLCD_SPI_SetWordWidth(Width);

}

/*************************************************************************
 * Function Name: GLCD_SPI_TranserByte
 * Parameters: Int32U Data
 * Return: Int32U
 *
 * Description: Transfer byte from SPI
 *
 *************************************************************************/
Int32U GLCD_SPI_TranserByte (Int32U Data)
{
  Int32U InData = 0;
  Int32U Mask;
  //int i;
  
  for (Mask = 1UL << (Width-1); Mask; Mask >>= 1)
  {
    // Clock Low
    GLCD_SPI_CLK_L();
    // Set Data
    if (Mask & Data)
    {
      GLCD_SPI_MOSI_H();
    }
    else
    {
      GLCD_SPI_MOSI_L();
    }
    InData <<= 1;
    // Clock High
    GLCD_delay();

    GLCD_SPI_CLK_H();

    // Get Data
    if (GLCD_SPI_MISO())
    {
      ++InData;
    }
    GLCD_delay();
  }
  return(InData);
}

/*************************************************************************
 * Function Name: GLCD_SPI_SendBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Write block of data to SPI
 *
 *************************************************************************/
void GLCD_SPI_SendBlock (pInt8U pData, Int32U Size)
{
Int32U OutCount = Size;
  while (OutCount--)
  {
    GLCD_SPI_TranserByte(*pData++ | 0x100);
  }
}

/*************************************************************************
 * Function Name: GLCD_SPI_ReceiveBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Read block of data from SPI
 *
 *************************************************************************/
void GLCD_SPI_ReceiveBlock (pInt8U pData, Int32U Size)
{
  while (Size)
  {
    *pData++ = GLCD_SPI_TranserByte(0xFFFF0);
    --Size;
  }
}
