/**
  ******************************************************************************
  * @file    stm32l0xx_hal_tim.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    06-February-2015
  * @brief   TIM HAL module driver.
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Timer (TIM) peripheral:
  *           + Timer Base Initialization
  *           + Timer Base Start
  *           + Timer Base Start Interruption
  *           + Timer Base Start DMA
  *           + Timer Output Compare/PWM Initialization
  *           + Timer Output Compare/PWM Channel Configuration
  *           + Timer Output Compare/PWM  Start
  *           + Timer Output Compare/PWM  Start Interruption
  *           + Timer Output Compare/PWM Start DMA
  *           + Timer Input Capture Initialization
  *           + Timer Input Capture Channel Configuration
  *           + Timer Input Capture Start
  *           + Timer Input Capture Start Interruption
  *           + Timer Input Capture Start DMA
  *           + Timer One Pulse Initialization
  *           + Timer One Pulse Channel Configuration
  *           + Timer One Pulse Start
  *           + Timer Encoder Interface Initialization
  *           + Timer Encoder Interface Start
  *           + Timer Encoder Interface Start Interruption
  *           + Timer Encoder Interface Start DMA
  *           + Timer OCRef clear configuration
  *           + Timer External Clock configuration
  *           + Timer Complementary signal bread and dead time configuration
  *           + Timer Master and Slave synchronization configuration
  @verbatim
  ==============================================================================
                      ##### TIMER Generic features #####
  ==============================================================================
    [..] The Timer features include: 
         (#) 16-bit up, down, up/down auto-reload counter.
         (#) 16-bit programmable prescaler allowing dividing (also on the fly) the counter clock
             frequency either by any factor between 1 and 65536.
         (#) Up to 4 independent channels for:
           (++) Input Capture
           (++) Output Compare
           (++) PWM generation (Edge and Center-aligned Mode)
           (++) One-pulse mode output
         (#) Synchronization circuit to control the timer with external signals and to interconnect
            several timers together.
         (#) Supports incremental (quadrature) encoder and hall-sensor circuitry for positioning
           purposes                
   
            ##### How to use this driver #####
================================================================================
    [..]
     (#) Initialize the TIM low level resources by implementing the following functions 
         depending from feature used :
           (++) Time Base : HAL_TIM_Base_MspInit() 
           (++) Input Capture : HAL_TIM_IC_MspInit()
           (++) Output Compare : HAL_TIM_OC_MspInit()
           (++) PWM generation : HAL_TIM_PWM_MspInit()
           (++) One-pulse mode output : HAL_TIM_OnePulse_MspInit()
           (++) Encoder mode output : HAL_TIM_Encoder_MspInit()
           
     (#) Initialize the TIM low level resources :
        (##) Enable the TIM interface clock using __HAL_RCC_TIMx_CLK_ENABLE(); 
        (##) TIM pins configuration
            (+++) Enable the clock for the TIM GPIOs using the following function:
             __HAL_RCC_GPIOx_CLK_ENABLE();   
            (+++) Configure these TIM pins in Alternate function mode using HAL_GPIO_Init();  

     (#) The external Clock can be configured, if needed (the default clock is the internal clock from the APBx), 
         using the following function:
         HAL_TIM_ConfigClockSource, the clock configuration should be done before any start function.
  
     (#) Configure the TIM in the desired functioning mode using one of the 
         initialization function of this driver:
         (++) HAL_TIM_Base_Init: to use the Timer to generate a simple time base
         (++) HAL_TIM_OC_Init and HAL_TIM_OC_ConfigChannel: to use the Timer to generate an 
              Output Compare signal.
         (++) HAL_TIM_PWM_Init and HAL_TIM_PWM_ConfigChannel: to use the Timer to generate a 
              PWM signal.
         (++) HAL_TIM_IC_Init and HAL_TIM_IC_ConfigChannel: to use the Timer to measure an 
              external signal.
         (++) HAL_TIM_OnePulse_Init and HAL_TIM_OnePulse_ConfigChannel: to use the Timer in One Pulse Mode.
         (++) HAL_TIM_Encoder_Init: to use the Timer Encoder Interface.

     (#) Activate the TIM peripheral using one of the start functions: 
         HAL_TIM_Base_Start(), HAL_TIM_Base_Start_DMA(), HAL_TIM_Base_Start_IT(),
         HAL_TIM_OC_Start(), HAL_TIM_OC_Start_DMA(), HAL_TIM_OC_Start_IT(),
         HAL_TIM_IC_Start(), HAL_TIM_IC_Start_DMA(), HAL_TIM_IC_Start_IT(),
         HAL_TIM_PWM_Start(), HAL_TIM_PWM_Start_DMA(), HAL_TIM_PWM_Start_IT(),
         HAL_TIM_OnePulse_Start(), HAL_TIM_OnePulse_Start_IT(),
         HAL_TIM_Encoder_Start(), HAL_TIM_Encoder_Start_DMA() or HAL_TIM_Encoder_Start_IT()

     (#) The DMA Burst is managed with the two following functions:
         HAL_TIM_DMABurst_WriteStart
         HAL_TIM_DMABurst_ReadStart
  
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/** @addtogroup STM32L0xx_HAL_Driver
  * @{
  */

/** @addtogroup TIM
  * @brief TIM HAL module driver
  * @{
  */

#ifdef HAL_TIM_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure);
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
static void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config);
static void TIM_TI1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);
static void TIM_TI1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);
static void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);
static void TIM_TI2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter);
static void TIM_TI3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);
static void TIM_TI4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection, uint32_t TIM_ICFilter);
static void TIM_ETR_SetConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter);
static void TIM_ITRx_SetConfig(TIM_TypeDef* TIMx, uint16_t InputTriggerSource);
static void TIM_CCxChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelState);
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma);
static void TIM_DMATriggerCplt(DMA_HandleTypeDef *hdma);
static void TIM_SlaveTimer_SetConfig(TIM_HandleTypeDef *htim,TIM_SlaveConfigTypeDef * sSlaveConfig);

/*******************************************************************************/
/* Exported functions ---------------------------------------------------------*/
/*******************************************************************************/

/** @addtogroup TIM_Exported_Functions
  * @{
  */

/** @addtogroup TIM_Exported_Functions_Group1
 *  @brief    Time Base functions
 *
@verbatim
  ==============================================================================
              ##### Timer Base functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM base.
    (+) De-initialize the TIM base.
    (+) Start the Timer Base.
    (+) Stop the Timer Base.
    (+) Start the Timer Base and enable interrupt.
    (+) Stop the Timer Base and disable interrupt.
    (+) Start the Timer Base and enable DMA transfer.
    (+) Stop the Timer Base and disable DMA transfer.
 
@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Time base Unit according to the specified
  *         parameters in the TIM_HandleTypeDef and create the associated handle.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_HandleTypeDef *htim)
{ 
  /* Check the TIM handle allocation */
  if(htim == NULL)
  {
    return HAL_ERROR;
  }
  
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance)); 
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_PRESCALER(htim->Init.Prescaler));
  
  if(htim->State == HAL_TIM_STATE_RESET)
  {  
    /* Init the low level hardware : GPIO, CLOCK, NVIC */
    HAL_TIM_Base_MspInit(htim);
  }
    
  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;
  
  /* Set the Time Base configuration */
  TIM_Base_SetConfig(htim->Instance, &htim->Init); 
  
  /* Initialize the TIM state*/
  htim->State= HAL_TIM_STATE_READY;
  
  return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM Base peripheral
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIM_Base_MspDeInit(htim);
  
  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;
  
  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes TIM Base MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief  Starts the TIM Base generation.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  
  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;
  
  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Change the TIM state*/
  htim->State= HAL_TIM_STATE_READY;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Base generation.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  
  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;
  
  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the TIM state*/
  htim->State= HAL_TIM_STATE_READY;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Base generation in interrupt mode.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  
  /* Enable the TIM Update interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Base generation in interrupt mode.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  /* Disable the TIM Update interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_UPDATE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Base generation in DMA mode.
  * @param  htim : TIM handle
  * @param  pData: The source Buffer address.
  * @param  Length: The length of data to be transferred from memory to peripheral.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));
  
  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if((pData == 0 ) && (Length > 0))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  /* Set the DMA Period elapsed callback */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;

  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;
  
  /* Enable the DMA Stream */
  HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData, (uint32_t)&htim->Instance->ARR, Length);
  
  /* Enable the TIM Update DMA request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Base generation in DMA mode.
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_UPDATE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);
    
  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */


/** @addtogroup TIM_Exported_Functions_Group2
 *  @brief    Time Output Compare functions
 *
@verbatim
  ==============================================================================
                  ##### Timer Output Compare functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM Output Compare.
    (+) De-initialize the TIM Output Compare.
    (+) Start the Timer Output Compare.
    (+) Stop the Timer Output Compare.
    (+) Start the Timer Output Compare and enable interrupt.
    (+) Stop the Timer Output Compare and disable interrupt.
    (+) Start the Timer Output Compare and enable DMA transfer.
    (+) Stop the Timer Output Compare and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Output Compare according to the specified
  *         parameters in the TIM_HandleTypeDef and create the associated handle.
  * @param  htim: TIM Output Compare handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Init(TIM_HandleTypeDef* htim)
{
  /* Check the TIM handle allocation */
  if(htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_PRESCALER(htim->Init.Prescaler));

  if(htim->State == HAL_TIM_STATE_RESET)
  {
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA*/
    HAL_TIM_OC_MspInit(htim);
  }
  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;

  /* Init the base time for the Output Compare */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);
  
  /* Initialize the TIM state*/
  htim->State= HAL_TIM_STATE_READY;
  
  return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM peripheral
  * @param  htim: TIM Output Compare handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

   htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
  HAL_TIM_OC_MspDeInit(htim);

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM Output Compare MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_OC_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes TIM Output Compare MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_OC_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_OC_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief  Starts the TIM Output Compare signal generation.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Enable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);
  
  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Output Compare signal generation.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
  
  /* Disable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);
  
  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Output Compare signal generation in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Enable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);
    }
    break;

    default:
    break;
  }

  /* Enable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Output Compare signal generation in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
    }
    break;

    default:
    break;
  }

  /* Disable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Output Compare signal generation in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  pData: The source Buffer address.
  * @param  Length: The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if(((uint32_t)pData == 0 ) && (Length > 0))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length);

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length);

      /* Enable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,Length);

      /* Enable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
     /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData, (uint32_t)&htim->Instance->CCR4, Length);

      /* Enable the TIM Capture/Compare 4 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Enable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Output Compare signal generation in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Disable the Output compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group3
 *  @brief    Time PWM functions
 *
@verbatim
  ==============================================================================
                          ##### Timer PWM functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM OPWM.
    (+) De-initialize the TIM PWM.
    (+) Start the Timer PWM.
    (+) Stop the Timer PWM.
    (+) Start the Timer PWM and enable interrupt.
    (+) Stop the Timer PWM and disable interrupt.
    (+) Start the Timer PWM and enable DMA transfer.
    (+) Stop the Timer PWM and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM PWM Time Base according to the specified
  *         parameters in the TIM_HandleTypeDef and create the associated handle.
  * @param  htim : TIM handle
  * @retval HAL status
  */


HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim)
{
  /* Check the TIM handle allocation */
  if(htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_PRESCALER(htim->Init.Prescaler));

  if(htim->State == HAL_TIM_STATE_RESET)
  {
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_PWM_MspInit(htim);
  }

  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;

  /* Init the base time for the PWM */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Initialize the TIM state*/
  htim->State= HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM peripheral
  * @param  htim : TIM handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
  HAL_TIM_PWM_MspDeInit(htim);

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM PWM MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_PWM_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes TIM PWM MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_PWM_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief  Starts the PWM signal generation.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the PWM signal generation.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Disable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the PWM signal generation in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channel to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Enable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);
    }
    break;

    default:
    break;
  }

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the PWM signal generation in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT (TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
    }
    break;

    default:
    break;
  }

  /* Disable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM PWM signal generation in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  pData: The source Buffer address. This buffer contains the values
  *                which will be loaded inside the capture/compare registers.
  * @param  Length: The length of data to be transferred from memory to TIM peripheral
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if(((uint32_t)pData == 0 ) && (Length > 0))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData, (uint32_t)&htim->Instance->CCR1, Length);

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData, (uint32_t)&htim->Instance->CCR2, Length);

      /* Enable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData, (uint32_t)&htim->Instance->CCR3,Length);

      /* Enable the TIM Output Capture/Compare 3 request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
     /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData, (uint32_t)&htim->Instance->CCR4, Length);

      /* Enable the TIM Capture/Compare 4 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Enable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM PWM signal generation in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Disable the Capture compare channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group4
 *  @brief    Time Input Capture functions
 *
@verbatim
  ==============================================================================
              ##### Timer Input Capture functions #####
  ==============================================================================
 [..]
   This section provides functions allowing to:
   (+) Initialize and configure the TIM Input Capture.
   (+) De-initialize the TIM Input Capture.
   (+) Start the Timer Input Capture.
   (+) Stop the Timer Input Capture.
   (+) Start the Timer Input Capture and enable interrupt.
   (+) Stop the Timer Input Capture and disable interrupt.
   (+) Start the Timer Input Capture and enable DMA transfer.
   (+) Stop the Timer Input Capture and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Input Capture Time base according to the specified
  *         parameters in the TIM_HandleTypeDef and create the associated handle.
  * @param  htim: TIM Input Capture handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Init(TIM_HandleTypeDef *htim)
{
  /* Check the TIM handle allocation */
  if(htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_PRESCALER(htim->Init.Prescaler));

  if(htim->State == HAL_TIM_STATE_RESET)
  {
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_IC_MspInit(htim);
  }

  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;

  /* Init the base time for the input capture */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Initialize the TIM state*/
  htim->State= HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM peripheral
  * @param  htim: TIM Input Capture handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC and DMA */
  HAL_TIM_IC_MspDeInit(htim);

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM INput Capture MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_IC_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes TIM Input Capture MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_IC_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_IC_MspDeInit could be implemented in the user file
   */
}
/**
  * @brief  Starts the TIM Input Capture measurement.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Start (TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Input Capture measurement.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  /* Disable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Input Capture measurement in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Start_IT (TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Enable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Enable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Enable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Enable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC4);
    }
    break;

    default:
    break;
  }
  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Input Capture measurement in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Disable the TIM Capture/Compare 4 interrupt */
      __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC4);
    }
    break;

    default:
    break;
  }

  /* Disable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Input Capture measurement on in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channels to be enabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @param  pData: The destination Buffer address.
  * @param  Length: The length of data to be transferred from TIM peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));

  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if((pData == 0 ) && (Length > 0))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 2  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->CCR3, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 3  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->CCR4, (uint32_t)pData, Length);

      /* Enable the TIM Capture/Compare 4  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Enable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Input Capture measurement on in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel : TIM Channels to be disabled
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(htim->Instance, Channel));
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Disable the TIM Capture/Compare 1 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    }
    break;

    case TIM_CHANNEL_2:
    {
      /* Disable the TIM Capture/Compare 2 DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;

    case TIM_CHANNEL_3:
    {
      /* Disable the TIM Capture/Compare 3  DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC3);
    }
    break;

    case TIM_CHANNEL_4:
    {
      /* Disable the TIM Capture/Compare 4  DMA request */
      __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC4);
    }
    break;

    default:
    break;
  }

  /* Disable the Input Capture channel */
  TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group5
 *  @brief    Time One Pulse functions
 *
@verbatim
  ==============================================================================
                        ##### Timer One Pulse functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM One Pulse.
    (+) De-initialize the TIM One Pulse.
    (+) Start the Timer One Pulse.
    (+) Stop the Timer One Pulse.
    (+) Start the Timer One Pulse and enable interrupt.
    (+) Stop the Timer One Pulse and disable interrupt.
    (+) Start the Timer One Pulse and enable DMA transfer.
    (+) Stop the Timer One Pulse and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM One Pulse Time Base according to the specified
  *         parameters in the TIM_HandleTypeDef and create the associated handle.
  * @param  htim: TIM OnePulse handle
  * @param  OnePulseMode: Select the One pulse mode.
  *         This parameter can be one of the following values:
  *            @arg TIM_OPMODE_SINGLE: Only one pulse will be generated.
  *            @arg TIM_OPMODE_REPETITIVE: Repetitive pulses will be generated.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, uint32_t OnePulseMode)
{
  /* Check the TIM handle allocation */
  if(htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
  assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
  assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
  assert_param(IS_TIM_OPM_MODE(OnePulseMode));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_PRESCALER(htim->Init.Prescaler));

  if(htim->State == HAL_TIM_STATE_RESET)
  {
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_OnePulse_MspInit(htim);
  }

  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;

  /* Configure the Time base in the One Pulse Mode */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Reset the OPM Bit */
  htim->Instance->CR1 &= ~TIM_CR1_OPM;

  /* Configure the OPM Mode */
  htim->Instance->CR1 |= OnePulseMode;

  /* Initialize the TIM state*/
  htim->State= HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM One Pulse
  * @param  htim: TIM One Pulse handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIM_OnePulse_MspDeInit(htim);

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM One Pulse MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_OnePulse_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_OnePulse_MspInit could be implemented in the user file
   */
}

/**
  * @brief  DeInitializes TIM One Pulse MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_OnePulse_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_OnePulse_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief  Starts the TIM One Pulse signal generation.
  * @param  htim : TIM handle
  * @param  OutputChannel : TIM Channels to be enabled.
  *          This parameter is not used since both channels TIM_CHANNEL_1 and
  *          TIM_CHANNEL_2 are automatically selected.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Enable the Capture compare and the Input Capture channels
    (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
    if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
    if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
    in all combinations, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be enabled together

    No need to enable the counter, it's enabled automatically by hardware
    (the counter starts in response to a stimulus and generate a pulse */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM One Pulse signal generation.
  * @param  htim : TIM handle
  * @param  OutputChannel : TIM Channels to be disable.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Disable the Capture compare and the Input Capture channels
  (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
  if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
  if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
  in all combinations, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be disabled together */

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM One Pulse signal generation in interrupt mode.
  * @param  htim : TIM handle
  * @param  OutputChannel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Start_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Enable the Capture compare and the Input Capture channels
    (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
    if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
    if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
    in all combinations, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be enabled together

    No need to enable the counter, it's enabled automatically by hardware
    (the counter starts in response to a stimulus and generate a pulse */

  /* Enable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);

  /* Enable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);

  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM One Pulse signal generation in interrupt mode.
  * @param  htim : TIM handle
  * @param  OutputChannel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_Stop_IT(TIM_HandleTypeDef *htim, uint32_t OutputChannel)
{
  /* Disable the TIM Capture/Compare 1 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);

  /* Disable the TIM Capture/Compare 2 interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);

  /* Disable the Capture compare and the Input Capture channels
  (in the OPM Mode the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2)
  if TIM_CHANNEL_1 is used as output, the TIM_CHANNEL_2 will be used as input and
  if TIM_CHANNEL_1 is used as input, the TIM_CHANNEL_2 will be used as output
  in all combinations, the TIM_CHANNEL_1 and TIM_CHANNEL_2 should be disabled together */
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
  TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);

  /* Disable the Peripheral */
   __HAL_TIM_DISABLE(htim);

  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group6
 *  @brief    Time Encoder functions
 *
@verbatim
  ==============================================================================
                          ##### Timer Encoder functions #####
  ==============================================================================
  [..]
    This section provides functions allowing to:
    (+) Initialize and configure the TIM Encoder.
    (+) De-initialize the TIM Encoder.
    (+) Start the Timer Encoder.
    (+) Stop the Timer Encoder.
    (+) Start the Timer Encoder and enable interrupt.
    (+) Stop the Timer Encoder and disable interrupt.
    (+) Start the Timer Encoder and enable DMA transfer.
    (+) Stop the Timer Encoder and disable DMA transfer.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Encoder Interface and create the associated handle.
  * @param  htim: TIM Encoder Interface handle
  * @param  sConfig: TIM Encoder Interface configuration structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim,  TIM_Encoder_InitTypeDef* sConfig)
{
  uint32_t tmpsmcr = 0;
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;

  /* Check the TIM handle allocation */
  if(htim == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
  assert_param(IS_TIM_ENCODER_MODE(sConfig->EncoderMode));
  assert_param(IS_TIM_IC_SELECTION(sConfig->IC1Selection));
  assert_param(IS_TIM_IC_SELECTION(sConfig->IC2Selection));
  assert_param(IS_TIM_IC_POLARITY(sConfig->IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(sConfig->IC2Polarity));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC1Prescaler));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->IC2Prescaler));
  assert_param(IS_TIM_IC_FILTER(sConfig->IC1Filter));
  assert_param(IS_TIM_IC_FILTER(sConfig->IC2Filter));
  assert_param(IS_TIM_PERIOD(htim->Init.Period));
  assert_param(IS_TIM_PRESCALER(htim->Init.Prescaler));

  if(htim->State == HAL_TIM_STATE_RESET)
  {
    /* Init the low level hardware : GPIO, CLOCK, NVIC and DMA */
    HAL_TIM_Encoder_MspInit(htim);
  }

  /* Set the TIM state */
  htim->State= HAL_TIM_STATE_BUSY;

  /* Reset the SMS bits */
  htim->Instance->SMCR &= ~TIM_SMCR_SMS;

  /* Configure the Time base in the Encoder Mode */
  TIM_Base_SetConfig(htim->Instance, &htim->Init);

  /* Get the TIMx SMCR register value */
  tmpsmcr = htim->Instance->SMCR;

  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = htim->Instance->CCMR1;

  /* Get the TIMx CCER register value */
  tmpccer = htim->Instance->CCER;

  /* Set the encoder Mode */
  tmpsmcr |= sConfig->EncoderMode;

  /* Select the Capture Compare 1 and the Capture Compare 2 as input */
  tmpccmr1 &= ~(TIM_CCMR1_CC1S | TIM_CCMR1_CC2S);
  tmpccmr1 |= (sConfig->IC1Selection | (sConfig->IC2Selection << 8));

  /* Set the the Capture Compare 1 and the Capture Compare 2 prescalers and filters */
  tmpccmr1 &= ~(TIM_CCMR1_IC1PSC | TIM_CCMR1_IC2PSC);
  tmpccmr1 &= ~(TIM_CCMR1_IC1F | TIM_CCMR1_IC2F);
  tmpccmr1 |= sConfig->IC1Prescaler | (sConfig->IC2Prescaler << 8);
  tmpccmr1 |= (sConfig->IC1Filter << 4) | (sConfig->IC2Filter << 12);

  /* Set the TI1 and the TI2 Polarities */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
  tmpccer &= ~(TIM_CCER_CC1NP | TIM_CCER_CC2NP);
  tmpccer |= sConfig->IC1Polarity | (sConfig->IC2Polarity << 4);

  /* Write to TIMx SMCR */
  htim->Instance->SMCR = tmpsmcr;

  /* Write to TIMx CCMR1 */
  htim->Instance->CCMR1 = tmpccmr1;

  /* Write to TIMx CCER */
  htim->Instance->CCER = tmpccer;

  /* Initialize the TIM state*/
  htim->State= HAL_TIM_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  DeInitializes the TIM Encoder interface
  * @param  htim: TIM Encoder handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_DeInit(TIM_HandleTypeDef *htim)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));

  htim->State = HAL_TIM_STATE_BUSY;

  /* Disable the TIM Peripheral Clock */
  __HAL_TIM_DISABLE(htim);

  /* DeInit the low level hardware: GPIO, CLOCK, NVIC */
  HAL_TIM_Encoder_MspDeInit(htim);

  /* Change TIM state */
  htim->State = HAL_TIM_STATE_RESET;

  /* Release Lock */
  __HAL_UNLOCK(htim);

  return HAL_OK;
}


/**
  * @brief  Initializes the TIM Encoder Interface MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_Encoder_MspInit could be implemented in the user file
   */
}


/**
  * @brief  DeInitializes TIM Encoder Interface MSP.
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_Encoder_MspDeInit could be implemented in the user file
   */
}

/**
  * @brief  Starts the TIM Encoder Interface.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
  
  /* Enable the encoder interface channels */
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      break; 
    }
    case TIM_CHANNEL_2:
    { 
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE); 
      break;
    }  
    default :
    {
     TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
     TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
     break; 
    }
  }  
  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Encoder Interface.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
    assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    
   /* Disable the Input Capture channels 1 and 2
    (in the EncoderInterface the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2) */ 
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
      break; 
    }
    case TIM_CHANNEL_2:
    { 
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE); 
      break;
    }  
    default :
    {
     TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE);
     TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE);
     break; 
    }
  }  
  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Encoder Interface in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
  
  /* Enable the encoder interface channels */
  /* Enable the capture compare Interrupts 1 and/or 2 */
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
      break; 
    }
    case TIM_CHANNEL_2:
    { 
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
      __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2); 
      break;
    }  
    default :
    {
     TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
     TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
     __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC1);
     __HAL_TIM_ENABLE_IT(htim, TIM_IT_CC2);
     break; 
    }
  }
  
  /* Enable the Peripheral */
  __HAL_TIM_ENABLE(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Encoder Interface in interrupt mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be disabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    
  /* Disable the Input Capture channels 1 and 2
    (in the EncoderInterface the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2) */ 
  if(Channel == TIM_CHANNEL_1)
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE); 
    
    /* Disable the capture compare Interrupts 1 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
  }  
  else if(Channel == TIM_CHANNEL_2)
  {  
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE); 
    
    /* Disable the capture compare Interrupts 2 */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
  }  
  else
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE); 
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE); 
    
    /* Disable the capture compare Interrupts 1 and 2 */
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC1);
    __HAL_TIM_DISABLE_IT(htim, TIM_IT_CC2);
  }
    
  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);
  
  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Starts the TIM Encoder Interface in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL : TIM Channel 1 and 2 selected
  * @param  pData1: The destination Buffer address for IC1.
  * @param  pData2: The destination Buffer address for IC2.
  * @param  Length: The length of data to be transferred from TIM peripheral to memory.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Start_DMA(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *pData1, uint32_t *pData2, uint16_t Length)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));
  
  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if((((pData1 == 0) || (pData2 == 0) )) && (Length > 0)) 
    {
      return HAL_ERROR;                                    
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }  
   
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;
      
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t )pData1, Length); 
      
      /* Enable the TIM Input Capture DMA request */      
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
            
      /* Enable the Peripheral */
      __HAL_TIM_ENABLE(htim);
      
      /* Enable the Capture compare channel */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
    }
    break;
    
    case TIM_CHANNEL_2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError;
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData2, Length);
      
      /* Enable the TIM Input Capture  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
     
      /* Enable the Peripheral */
      __HAL_TIM_ENABLE(htim);
      
      /* Enable the Capture compare channel */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
    }
    break;
    
    case TIM_CHANNEL_ALL:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;
      
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->CCR1, (uint32_t)pData1, Length);
      
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;
      
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->CCR2, (uint32_t)pData2, Length);
          
     /* Enable the Peripheral */
      __HAL_TIM_ENABLE(htim);
      
      /* Enable the Capture compare channel */
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_ENABLE);
      TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_ENABLE);
      
      /* Enable the TIM Input Capture  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
      /* Enable the TIM Input Capture  DMA request */
      __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    }
    break;
    
    default:
    break;
  }  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM Encoder Interface in DMA mode.
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_Encoder_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMA_CC_INSTANCE(htim->Instance));
  
  /* Disable the Input Capture channels 1 and 2
    (in the EncoderInterface the two possible channels that can be used are TIM_CHANNEL_1 and TIM_CHANNEL_2) */ 
  if(Channel == TIM_CHANNEL_1)
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE); 
    
    /* Disable the capture compare DMA Request 1 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
  }  
  else if(Channel == TIM_CHANNEL_2)
  {  
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE); 
    
    /* Disable the capture compare DMA Request 2 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
  }  
  else
  {
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_1, TIM_CCx_DISABLE); 
    TIM_CCxChannelCmd(htim->Instance, TIM_CHANNEL_2, TIM_CCx_DISABLE); 
    
    /* Disable the capture compare DMA Request 1 and 2 */
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC1);
    __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_CC2);
  }
  
  /* Disable the Peripheral */
  __HAL_TIM_DISABLE(htim);
  
  /* Change the htim state */
  htim->State = HAL_TIM_STATE_READY;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group7
 *  @brief    IRQ handler management
 *
@verbatim
  ==============================================================================
                        ##### IRQ handler management #####
  ==============================================================================
  [..]
    This section provides Timer IRQ handler function.

@endverbatim
  * @{
  */
/**
  * @brief  This function handles TIM interrupts requests.
  * @param  htim: TIM  handle
  * @retval None
  */
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)
{
  /* Capture compare 1 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC1) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC1) !=RESET)
    {
      {
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
        htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;

        /* Input capture event */
        if((htim->Instance->CCMR1 & TIM_CCMR1_CC1S) != 0x00)
        {
          HAL_TIM_IC_CaptureCallback(htim);
        }
        /* Output compare event */
        else
        {
          HAL_TIM_OC_DelayElapsedCallback(htim);
          HAL_TIM_PWM_PulseFinishedCallback(htim);
        }
        htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
      }
    }
  }
  /* Capture compare 2 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC2) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC2) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC2);
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
      /* Input capture event */
      if((htim->Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00)
      {
        HAL_TIM_IC_CaptureCallback(htim);
      }
      /* Output compare event */
      else
      {
        HAL_TIM_OC_DelayElapsedCallback(htim);
        HAL_TIM_PWM_PulseFinishedCallback(htim);
      }
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
  }
  /* Capture compare 3 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC3) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC3) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
      /* Input capture event */
      if((htim->Instance->CCMR2 & TIM_CCMR2_CC3S) != 0x00)
      {
        HAL_TIM_IC_CaptureCallback(htim);
      }
      /* Output compare event */
      else
      {
        HAL_TIM_OC_DelayElapsedCallback(htim);
        HAL_TIM_PWM_PulseFinishedCallback(htim);
      }
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
  }
  /* Capture compare 4 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC4) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_CC4) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC4);
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
      /* Input capture event */
      if((htim->Instance->CCMR2 & TIM_CCMR2_CC4S) != 0x00)
      {
        HAL_TIM_IC_CaptureCallback(htim);
      }
      /* Output compare event */
      else
      {
        HAL_TIM_OC_DelayElapsedCallback(htim);
        HAL_TIM_PWM_PulseFinishedCallback(htim);
      }
      htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    }
  }
  /* TIM Update event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_UPDATE) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
      HAL_TIM_PeriodElapsedCallback(htim);
    }
  }
  /* TIM Trigger detection event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_TRIGGER) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(htim, TIM_IT_TRIGGER) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_TRIGGER);
      HAL_TIM_TriggerCallback(htim);
    }
  }
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group8
 *  @brief    Peripheral Control functions
 *
@verbatim
  ==============================================================================
             ##### Peripheral Control functions #####
  ==============================================================================
 [..]
   This section provides functions allowing to:
      (+) Configure The Input Output channels for OC, PWM, IC or One Pulse mode.
      (+) Configure External Clock source.
      (+) Configure Master and the Slave synchronization.
      (+) Configure the DMA Burst Mode.

@endverbatim
  * @{
  */
/**
  * @brief  Initializes the TIM Output Compare Channels according to the specified
  *         parameters in the TIM_OC_InitTypeDef.
  * @param  htim : TIM handle
  * @param  sConfig: TIM Output Compare configuration structure
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OC_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel)
{
  /* Check the parameters */ 
  assert_param(IS_TIM_CHANNELS(Channel)); 
  assert_param(IS_TIM_OC_MODE(sConfig->OCMode));
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));
  
  /* Check input state */
  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      /* Configure the TIM Channel 1 in Output Compare */
      TIM_OC1_SetConfig(htim->Instance, sConfig);
    }
    break;
    
    case TIM_CHANNEL_2:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      /* Configure the TIM Channel 2 in Output Compare */
      TIM_OC2_SetConfig(htim->Instance, sConfig);
    }
    break;
    
    case TIM_CHANNEL_3:
    {
       assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));
      /* Configure the TIM Channel 3 in Output Compare */
      TIM_OC3_SetConfig(htim->Instance, sConfig);
    }
    break;
    
    case TIM_CHANNEL_4:
    {
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));
      /* Configure the TIM Channel 4 in Output Compare */
      TIM_OC4_SetConfig(htim->Instance, sConfig);
    }
    break;
    
    default:
    break;    
  }
  htim->State = HAL_TIM_STATE_READY;
  
  __HAL_UNLOCK(htim);
  
  return HAL_OK;
}

/**
  * @brief  Initializes the TIM Input Capture Channels according to the specified
  *         parameters in the TIM_IC_InitTypeDef.
  * @param  htim : TIM handle
  * @param  sConfig: TIM Input Capture configuration structure
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_IC_ConfigChannel(TIM_HandleTypeDef *htim, TIM_IC_InitTypeDef* sConfig, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
  assert_param(IS_TIM_IC_POLARITY(sConfig->ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(sConfig->ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(sConfig->ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(sConfig->ICFilter));

  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  if (Channel == TIM_CHANNEL_1)
  {
    /* TI1 Configuration */
    TIM_TI1_SetConfig(htim->Instance,
               sConfig->ICPolarity,
               sConfig->ICSelection,
               sConfig->ICFilter);

    /* Reset the IC1PSC Bits */
    htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;

    /* Set the IC1PSC value */
    htim->Instance->CCMR1 |= sConfig->ICPrescaler;
  }
  else if (Channel == TIM_CHANNEL_2)
  {
    /* TI2 Configuration */
    assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    
    TIM_TI2_SetConfig(htim->Instance,
                      sConfig->ICPolarity,
                      sConfig->ICSelection,
                      sConfig->ICFilter);
               
    /* Reset the IC2PSC Bits */
    htim->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;

    /* Set the IC2PSC value */
    htim->Instance->CCMR1 |= (sConfig->ICPrescaler << 8);
  }
  else if (Channel == TIM_CHANNEL_3)
  {
    /* TI3 Configuration */
    assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

    TIM_TI3_SetConfig(htim->Instance,
               sConfig->ICPolarity,
               sConfig->ICSelection,
               sConfig->ICFilter);
               
    /* Reset the IC3PSC Bits */
    htim->Instance->CCMR2 &= ~TIM_CCMR2_IC3PSC;

    /* Set the IC3PSC value */
    htim->Instance->CCMR2 |= sConfig->ICPrescaler;
  }
  else
  {
    /* TI4 Configuration */
    assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

    TIM_TI4_SetConfig(htim->Instance,
               sConfig->ICPolarity,
               sConfig->ICSelection,
               sConfig->ICFilter);

    /* Reset the IC4PSC Bits */
    htim->Instance->CCMR2 &= ~TIM_CCMR2_IC4PSC;

    /* Set the IC4PSC value */
    htim->Instance->CCMR2 |= (sConfig->ICPrescaler << 8);
  }

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM PWM  channels according to the specified
  *         parameters in the TIM_OC_InitTypeDef.
  * @param  htim : TIM handle
  * @param  sConfig: TIM PWM configuration structure
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel)
{
  __HAL_LOCK(htim);

  /* Check the parameters */
  assert_param(IS_TIM_CHANNELS(Channel));
  assert_param(IS_TIM_PWM_MODE(sConfig->OCMode));
  assert_param(IS_TIM_OC_POLARITY(sConfig->OCPolarity));
  assert_param(IS_TIM_FAST_STATE(sConfig->OCFastMode));
  
  htim->State = HAL_TIM_STATE_BUSY;

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      /* Configure the Channel 1 in PWM mode */
      TIM_OC1_SetConfig(htim->Instance, sConfig);

      /* Set the Preload enable bit for channel1 */
      htim->Instance->CCMR1 |= TIM_CCMR1_OC1PE;

      /* Configure the Output Fast mode */
      htim->Instance->CCMR1 &= ~TIM_CCMR1_OC1FE;
      htim->Instance->CCMR1 |= sConfig->OCFastMode;
    }
    break;

    case TIM_CHANNEL_2:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      /* Configure the Channel 2 in PWM mode */
      TIM_OC2_SetConfig(htim->Instance, sConfig);

      /* Set the Preload enable bit for channel2 */
      htim->Instance->CCMR1 |= TIM_CCMR1_OC2PE;

      /* Configure the Output Fast mode */
      htim->Instance->CCMR1 &= ~TIM_CCMR1_OC2FE;
      htim->Instance->CCMR1 |= sConfig->OCFastMode << 8;
    }
    break;

    case TIM_CHANNEL_3:
    {
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));
      /* Configure the Channel 3 in PWM mode */
      TIM_OC3_SetConfig(htim->Instance, sConfig);

      /* Set the Preload enable bit for channel3 */
      htim->Instance->CCMR2 |= TIM_CCMR2_OC3PE;

     /* Configure the Output Fast mode */
      htim->Instance->CCMR2 &= ~TIM_CCMR2_OC3FE;
      htim->Instance->CCMR2 |= sConfig->OCFastMode;
    }
    break;

    case TIM_CHANNEL_4:
    {
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));
      /* Configure the Channel 4 in PWM mode */
      TIM_OC4_SetConfig(htim->Instance, sConfig);

      /* Set the Preload enable bit for channel4 */
      htim->Instance->CCMR2 |= TIM_CCMR2_OC4PE;

     /* Configure the Output Fast mode */
      htim->Instance->CCMR2 &= ~TIM_CCMR2_OC4FE;
      htim->Instance->CCMR2 |= sConfig->OCFastMode << 8;
    }
    break;

    default:
    break;
  }
  
  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Initializes the TIM One Pulse Channels according to the specified
  *         parameters in the TIM_OnePulse_InitTypeDef.
  * @param  htim : TIM handle
  * @param  sConfig: TIM One Pulse configuration structure
  * @param  OutputChannel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @param  InputChannel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_OnePulse_ConfigChannel(TIM_HandleTypeDef *htim,  TIM_OnePulse_InitTypeDef* sConfig, uint32_t OutputChannel,  uint32_t InputChannel)
{
  TIM_OC_InitTypeDef temp1;

  /* Check the parameters */
  assert_param(IS_TIM_OPM_CHANNELS(OutputChannel));
  assert_param(IS_TIM_OPM_CHANNELS(InputChannel));

  if(OutputChannel != InputChannel)
  {
    __HAL_LOCK(htim);
  
    htim->State = HAL_TIM_STATE_BUSY;

    /* Extract the Ouput compare configuration from sConfig structure */
    temp1.OCMode = sConfig->OCMode;
    temp1.Pulse = sConfig->Pulse;
    temp1.OCPolarity = sConfig->OCPolarity;

    switch (OutputChannel)
    {
      case TIM_CHANNEL_1:
      {
        assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

        TIM_OC1_SetConfig(htim->Instance, &temp1);
      }
      break;
      case TIM_CHANNEL_2:
      {
        assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

        TIM_OC2_SetConfig(htim->Instance, &temp1);
      }
      break;
      default:
      break;
    }
    switch (InputChannel)
    {
      case TIM_CHANNEL_1:
      {
        assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      
        TIM_TI1_SetConfig(htim->Instance, sConfig->ICPolarity,
                        sConfig->ICSelection, sConfig->ICFilter);

        /* Reset the IC1PSC Bits */
        htim->Instance->CCMR1 &= ~TIM_CCMR1_IC1PSC;

        /* Select the Trigger source */
        htim->Instance->SMCR &= ~TIM_SMCR_TS;
        htim->Instance->SMCR |= TIM_TS_TI1FP1;

        /* Select the Slave Mode */
        htim->Instance->SMCR &= ~TIM_SMCR_SMS;
        htim->Instance->SMCR |= TIM_SLAVEMODE_TRIGGER;
      }
      break;
      case TIM_CHANNEL_2:
      {
        assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

        TIM_TI2_SetConfig(htim->Instance, sConfig->ICPolarity,
                 sConfig->ICSelection, sConfig->ICFilter);

        /* Reset the IC2PSC Bits */
        htim->Instance->CCMR1 &= ~TIM_CCMR1_IC2PSC;

        /* Select the Trigger source */
        htim->Instance->SMCR &= ~TIM_SMCR_TS;
        htim->Instance->SMCR |= TIM_TS_TI2FP2;

        /* Select the Slave Mode */
        htim->Instance->SMCR &= ~TIM_SMCR_SMS;
        htim->Instance->SMCR |= TIM_SLAVEMODE_TRIGGER;
      }
      break;

      default:
      break;
    }

    htim->State = HAL_TIM_STATE_READY;

    __HAL_UNLOCK(htim);

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Configure the DMA Burst to transfer Data from the memory to the TIM peripheral
  * @param  htim : TIM handle
  * @param  BurstBaseAddress: TIM Base address from when the DMA will starts the Data write.
  *         This parameters can be on of the following values:
  *            @arg TIM_DMABASE_CR1  
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2  
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT   
  *            @arg TIM_DMABASE_PSC  
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3 
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_DCR
  * @param  BurstRequestSrc: TIM DMA Request sources.
  *         This parameters can be on of the following values:
  *            @arg TIM_DMA_UPDATE: TIM update Interrupt source
  *            @arg TIM_DMA_CC1: TIM Capture Compare 1 DMA source
  *            @arg TIM_DMA_CC2: TIM Capture Compare 2 DMA source
  *            @arg TIM_DMA_CC3: TIM Capture Compare 3 DMA source
  *            @arg TIM_DMA_CC4: TIM Capture Compare 4 DMA source
  *            @arg TIM_DMA_TRIGGER: TIM Trigger DMA source
  * @param  BurstBuffer: The Buffer address.
  * @param  BurstLength: DMA Burst length. This parameter can be one value
  *         between TIM_DMABURSTLENGTH_1TRANSFER   and TIM_DMABURSTLENGTH_18TRANSFERS  .
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress, uint32_t BurstRequestSrc,
                                              uint32_t* BurstBuffer, uint32_t  BurstLength)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));
  assert_param(IS_TIM_DMA_BASE(BurstBaseAddress));
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));
  assert_param(IS_TIM_DMA_LENGTH(BurstLength));
  
  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if((BurstBuffer == 0 ) && (BurstLength > 0)) 
    {
      return HAL_ERROR;                                    
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch(BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {  
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)BurstBuffer, (uint32_t)&htim->Instance->DMAR, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC1:
    {  
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)BurstBuffer, (uint32_t)&htim->Instance->DMAR, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC2:
    {  
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)BurstBuffer, (uint32_t)&htim->Instance->DMAR, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC3:
    {  
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)BurstBuffer, (uint32_t)&htim->Instance->DMAR, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC4:
    {  
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)BurstBuffer, (uint32_t)&htim->Instance->DMAR, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_TRIGGER:
    {  
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferCpltCallback = TIM_DMATriggerCplt;
     
      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_TRIGGER], (uint32_t)BurstBuffer, (uint32_t)&htim->Instance->DMAR, ((BurstLength) >> 8) + 1);
    }
    break;
    default:
    break;  
  }
   /* configure the DMA Burst Mode */
   htim->Instance->DCR = BurstBaseAddress | BurstLength;

   /* Enable the TIM DMA Request */
   __HAL_TIM_ENABLE_DMA(htim, BurstRequestSrc);

   htim->State = HAL_TIM_STATE_READY;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stops the TIM DMA Burst mode
  * @param  htim : TIM handle
  * @param  BurstRequestSrc: TIM DMA Request sources to disable
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_WriteStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));
  
  /* Abort the DMA transfer (at least disable the DMA channel) */
  switch(BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_UPDATE]); 
    }
    break;
    case TIM_DMA_CC1:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC1]);     
    }
    break;
    case TIM_DMA_CC2:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC2]);     
    }
    break;
    case TIM_DMA_CC3:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC3]);     
    }
    break;
    case TIM_DMA_CC4:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC4]);     
    }
    break;
    case TIM_DMA_TRIGGER:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_TRIGGER]);     
    }
    break;
    default:
    break;  
  }
  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, BurstRequestSrc);
      
  /* Return function status */
  return HAL_OK;  
}

/**
  * @brief  Configure the DMA Burst to transfer Data from the TIM peripheral to the memory
  * @param  htim : TIM handle
  * @param  BurstBaseAddress: TIM Base address from when the DMA will starts the Data read.
  *         This parameters can be on of the following values:
  *            @arg TIM_DMABASE_CR1
  *            @arg TIM_DMABASE_CR2
  *            @arg TIM_DMABASE_SMCR
  *            @arg TIM_DMABASE_DIER
  *            @arg TIM_DMABASE_SR
  *            @arg TIM_DMABASE_EGR
  *            @arg TIM_DMABASE_CCMR1
  *            @arg TIM_DMABASE_CCMR2  
  *            @arg TIM_DMABASE_CCER
  *            @arg TIM_DMABASE_CNT
  *            @arg TIM_DMABASE_PSC
  *            @arg TIM_DMABASE_ARR
  *            @arg TIM_DMABASE_CCR1
  *            @arg TIM_DMABASE_CCR2
  *            @arg TIM_DMABASE_CCR3
  *            @arg TIM_DMABASE_CCR4
  *            @arg TIM_DMABASE_DCR
  * @param  BurstRequestSrc: TIM DMA Request sources.
  *         This parameters can be on of the following values:
  *            @arg TIM_DMA_UPDATE: TIM update Interrupt source
  *            @arg TIM_DMA_CC1: TIM Capture Compare 1 DMA source
  *            @arg TIM_DMA_CC2: TIM Capture Compare 2 DMA source
  *            @arg TIM_DMA_CC3: TIM Capture Compare 3 DMA source
  *            @arg TIM_DMA_CC4: TIM Capture Compare 4 DMA source
  *            @arg TIM_DMA_TRIGGER: TIM Trigger DMA source
  * @param  BurstBuffer: The Buffer address.
  * @param  BurstLength: DMA Burst length. This parameter can be one value
  *         between TIM_DMABURSTLENGTH_1TRANSFER   and TIM_DMABURSTLENGTH_18TRANSFERS  .
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStart(TIM_HandleTypeDef *htim, uint32_t BurstBaseAddress, uint32_t BurstRequestSrc,
                                             uint32_t  *BurstBuffer, uint32_t  BurstLength)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMABURST_INSTANCE(htim->Instance));
  assert_param(IS_TIM_DMA_BASE(BurstBaseAddress));
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));
  assert_param(IS_TIM_DMA_LENGTH(BurstLength));
  
  if((htim->State == HAL_TIM_STATE_BUSY))
  {
     return HAL_BUSY;
  }
  else if((htim->State == HAL_TIM_STATE_READY))
  {
    if((BurstBuffer == 0 ) && (BurstLength > 0))
    {
      return HAL_ERROR;
    }
    else
    {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  }
  switch(BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
       HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC1:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC2:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC3:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError ;
  
      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_CC4:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMACaptureCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer, ((BurstLength) >> 8) + 1);
    }
    break;
    case TIM_DMA_TRIGGER:
    {
      /* Set the DMA Period elapsed callback */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferCpltCallback = TIM_DMATriggerCplt;

      /* Set the DMA error callback */
      htim->hdma[TIM_DMA_ID_TRIGGER]->XferErrorCallback = TIM_DMAError ;

      /* Enable the DMA Stream */
      HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_TRIGGER], (uint32_t)&htim->Instance->DMAR, (uint32_t)BurstBuffer, ((BurstLength) >> 8) + 1);
    }
    break;
    default:
    break;
  }

  /* configure the DMA Burst Mode */
  htim->Instance->DCR = BurstBaseAddress | BurstLength;

  /* Enable the TIM DMA Request */
  __HAL_TIM_ENABLE_DMA(htim, BurstRequestSrc);

  htim->State = HAL_TIM_STATE_READY;
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Stop the DMA burst reading
  * @param  htim : TIM handle
  * @param  BurstRequestSrc: TIM DMA Request sources to disable.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_DMABurst_ReadStop(TIM_HandleTypeDef *htim, uint32_t BurstRequestSrc)
{
  /* Check the parameters */
  assert_param(IS_TIM_DMA_SOURCE(BurstRequestSrc));
  
  /* Abort the DMA transfer (at least disable the DMA channel) */
  switch(BurstRequestSrc)
  {
    case TIM_DMA_UPDATE:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_UPDATE]); 
    }
    break;
    case TIM_DMA_CC1:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC1]);     
    }
    break;
    case TIM_DMA_CC2:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC2]);     
    }
    break;
    case TIM_DMA_CC3:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC3]);     
    }
    break;
    case TIM_DMA_CC4:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_CC4]);     
    }
    break;
    case TIM_DMA_TRIGGER:
    {  
      HAL_DMA_Abort(htim->hdma[TIM_DMA_ID_TRIGGER]);     
    }
    break;
    default:
    break;  
  }

  /* Disable the TIM Update DMA request */
  __HAL_TIM_DISABLE_DMA(htim, BurstRequestSrc);

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Generate a software event
  * @param  htim : TIM handle
  * @param  EventSource: specifies the event source.
  *          This parameter can be one of the following values:
  *            @arg TIM_EventSource_Update: Timer update Event source
  *            @arg TIM_EVENTSOURCE_CC1: Timer Capture Compare 1 Event source
  *            @arg TIM_EventSource_CC2: Timer Capture Compare 2 Event source
  *            @arg TIM_EventSource_CC3: Timer Capture Compare 3 Event source
  *            @arg TIM_EventSource_CC4: Timer Capture Compare 4 Event source
  *            @arg TIM_EVENTSOURCE_TRIGGER  : Timer Trigger Event source
  * @note   TIM6 can only generate an update event.
  * @retval HAL status
  */

HAL_StatusTypeDef HAL_TIM_GenerateEvent(TIM_HandleTypeDef *htim, uint32_t EventSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_INSTANCE(htim->Instance));
  assert_param(IS_TIM_EVENT_SOURCE(EventSource));

  /* Process Locked */
  __HAL_LOCK(htim);

  /* Change the TIM state */
  htim->State = HAL_TIM_STATE_BUSY;
  
  /* Set the event sources */
  htim->Instance->EGR = EventSource;

  /* Change the TIM state */
  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);
  
  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Configures the OCRef clear feature
  * @param  htim : TIM handle
  * @param  sClearInputConfig: pointer to a TIM_ClearInputConfigTypeDef structure that
  *         contains the OCREF clear feature and parameters for the TIM peripheral.
  * @param  Channel: specifies the TIM Channel.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_ConfigOCrefClear(TIM_HandleTypeDef *htim, TIM_ClearInputConfigTypeDef * sClearInputConfig, uint32_t Channel)
{
  /* Check the parameters */
  assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
  assert_param(IS_TIM_CHANNELS(Channel));
  assert_param(IS_TIM_CLEARINPUT_SOURCE(sClearInputConfig->ClearInputSource));
  assert_param(IS_TIM_CLEARINPUT_POLARITY(sClearInputConfig->ClearInputPolarity));
  assert_param(IS_TIM_CLEARINPUT_PRESCALER(sClearInputConfig->ClearInputPrescaler));
  assert_param(IS_TIM_CLEARINPUT_FILTER(sClearInputConfig->ClearInputFilter));

  /* Process Locked */
  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  if(sClearInputConfig->ClearInputSource == TIM_CLEARINPUTSOURCE_ETR)
  {
    TIM_ETR_SetConfig(htim->Instance,
                      sClearInputConfig->ClearInputPrescaler,
                      sClearInputConfig->ClearInputPolarity,
                      sClearInputConfig->ClearInputFilter);
  }

  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      if(sClearInputConfig->ClearInputState != RESET)
      {
        /* Enable the Ocref clear feature for Channel 1 */
        htim->Instance->CCMR1 |= TIM_CCMR1_OC1CE;
      }
      else
      {
        /* Disable the Ocref clear feature for Channel 1 */
        htim->Instance->CCMR1 &= ~TIM_CCMR1_OC1CE;
      }
    }    
    break;
    case TIM_CHANNEL_2:    
    { 
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance)); 
      if(sClearInputConfig->ClearInputState != RESET)  
      {
        /* Enable the Ocref clear feature for Channel 2 */
        htim->Instance->CCMR1 |= TIM_CCMR1_OC2CE;
      }
      else
      {
        /* Disable the Ocref clear feature for Channel 2 */
        htim->Instance->CCMR1 &= ~TIM_CCMR1_OC2CE;      
      }
    } 
    break;
    case TIM_CHANNEL_3:   
    {  
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));
      if(sClearInputConfig->ClearInputState != RESET)  
      {
        /* Enable the Ocref clear feature for Channel 3 */
        htim->Instance->CCMR2 |= TIM_CCMR2_OC3CE;
      }
      else
      {
        /* Disable the Ocref clear feature for Channel 3 */
        htim->Instance->CCMR2 &= ~TIM_CCMR2_OC3CE;      
      }
    } 
    break;
    case TIM_CHANNEL_4:    
    {  
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));
      if(sClearInputConfig->ClearInputState != RESET)  
      {
        /* Enable the Ocref clear feature for Channel 4 */
        htim->Instance->CCMR2 |= TIM_CCMR2_OC4CE;
      }
      else
      {
        /* Disable the Ocref clear feature for Channel 4 */
        htim->Instance->CCMR2 &= ~TIM_CCMR2_OC4CE;      
      }
    } 
    break;
    default:  
    break;
  } 

  htim->State = HAL_TIM_STATE_READY;
  
  __HAL_UNLOCK(htim);
  
  return HAL_OK;  
}  

/**
  * @brief   Configures the clock source to be used
  * @param  htim : TIM handle
  * @param  sClockSourceConfig: pointer to a TIM_ClockConfigTypeDef structure that
  *         contains the clock source information for the TIM peripheral. 
  * @retval HAL status
  */ 
HAL_StatusTypeDef HAL_TIM_ConfigClockSource(TIM_HandleTypeDef *htim, TIM_ClockConfigTypeDef * sClockSourceConfig)    
{
  uint32_t tmpsmcr = 0;
    
  /* Process Locked */
  __HAL_LOCK(htim);
  
  htim->State = HAL_TIM_STATE_BUSY;
  
  /* Check the parameters */
  assert_param(IS_TIM_CLOCKSOURCE(sClockSourceConfig->ClockSource));
  assert_param(IS_TIM_CLOCKPOLARITY(sClockSourceConfig->ClockPolarity));
  assert_param(IS_TIM_CLOCKPRESCALER(sClockSourceConfig->ClockPrescaler));
  assert_param(IS_TIM_CLOCKFILTER(sClockSourceConfig->ClockFilter));
  
  /* Reset the SMS, TS, ECE, ETPS and ETRF bits */
  tmpsmcr = htim->Instance->SMCR;
  tmpsmcr &= ~(TIM_SMCR_SMS | TIM_SMCR_TS);
  tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);
  htim->Instance->SMCR = tmpsmcr;
  
  switch (sClockSourceConfig->ClockSource)
  {
    case TIM_CLOCKSOURCE_INTERNAL:
    { 
      assert_param(IS_TIM_INSTANCE(htim->Instance));      
      /* Disable slave mode to clock the prescaler directly with the internal clock */
      htim->Instance->SMCR &= ~TIM_SMCR_SMS;
    }
    break;
    
    case TIM_CLOCKSOURCE_ETRMODE1:
    {
      assert_param(IS_TIM_ETR_INSTANCE(htim->Instance));
      /* Configure the ETR Clock source */
      TIM_ETR_SetConfig(htim->Instance, 
                        sClockSourceConfig->ClockPrescaler, 
                        sClockSourceConfig->ClockPolarity, 
                        sClockSourceConfig->ClockFilter);
      /* Get the TIMx SMCR register value */
      tmpsmcr = htim->Instance->SMCR;
      /* Reset the SMS and TS Bits */
      tmpsmcr &= ~(TIM_SMCR_SMS | TIM_SMCR_TS);
      /* Select the External clock mode1 and the ETRF trigger */
      tmpsmcr |= (TIM_SLAVEMODE_EXTERNAL1 | TIM_CLOCKSOURCE_ETRMODE1);
      /* Write to TIMx SMCR */
      htim->Instance->SMCR = tmpsmcr;
    }
    break;
    
    case TIM_CLOCKSOURCE_ETRMODE2:
    {
      assert_param(IS_TIM_ETR_INSTANCE(htim->Instance));
      /* Configure the ETR Clock source */
      TIM_ETR_SetConfig(htim->Instance, 
                        sClockSourceConfig->ClockPrescaler, 
                        sClockSourceConfig->ClockPolarity,
                        sClockSourceConfig->ClockFilter);
      /* Enable the External clock mode2 */
      htim->Instance->SMCR |= TIM_SMCR_ECE;
    }
    break;
    
    case TIM_CLOCKSOURCE_TI1:
    {
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      TIM_TI1_ConfigInputStage(htim->Instance, 
                        sClockSourceConfig->ClockPolarity, 
                        sClockSourceConfig->ClockFilter);
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI1);
    }
    break;
    case TIM_CLOCKSOURCE_TI2:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      TIM_TI2_ConfigInputStage(htim->Instance, 
                        sClockSourceConfig->ClockPolarity, 
                        sClockSourceConfig->ClockFilter);
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI2);
    }
    break;
    case TIM_CLOCKSOURCE_TI1ED:
    {
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      TIM_TI1_ConfigInputStage(htim->Instance, 
                        sClockSourceConfig->ClockPolarity,
                        sClockSourceConfig->ClockFilter);
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_TI1ED);
    }
    break;
    case TIM_CLOCKSOURCE_ITR0:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_ITR0);
    }
    break;
    case TIM_CLOCKSOURCE_ITR1:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_ITR1);
    }
    break;
    case TIM_CLOCKSOURCE_ITR2:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_ITR2);
    }
    break;
    case TIM_CLOCKSOURCE_ITR3:
    {
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      TIM_ITRx_SetConfig(htim->Instance, TIM_CLOCKSOURCE_ITR3);
    }
    break;
    
    default:
    break;    
  }
  htim->State = HAL_TIM_STATE_READY;
  
  __HAL_UNLOCK(htim);
  
  return HAL_OK;
}

/**
  * @brief  Selects the signal connected to the TI1 input: direct from CH1_input
  *         or a XOR combination between CH1_input, CH2_input & CH3_input
  * @param  htim : TIM handle
  * @param  TI1_Selection: Indicate whether or not channel 1 is connected to the
  *         output of a XOR gate.
  *         This parameter can be one of the following values:
  *            @arg TIM_TI1SELECTION_CH1: The TIMx_CH1 pin is connected to TI1 input
  *            @arg TIM_TI1SELECTION_XORCOMBINATION: The TIMx_CH1, CH2 and CH3
  *            pins are connected to the TI1 input (XOR combination)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_ConfigTI1Input(TIM_HandleTypeDef *htim, uint32_t TI1_Selection)
{
  uint32_t tmpcr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_XOR_INSTANCE(htim->Instance)); 
  assert_param(IS_TIM_TI1SELECTION(TI1_Selection));

  /* Get the TIMx CR2 register value */
  tmpcr2 = htim->Instance->CR2;

  /* Reset the TI1 selection */
  tmpcr2 &= ~TIM_CR2_TI1S;

  /* Set the the TI1 selection */
  tmpcr2 |= TI1_Selection;
  
  /* Write to TIMxCR2 */
  htim->Instance->CR2 = tmpcr2;

  return HAL_OK;
}
                                                
/**
  * @brief  Configures the TIM in Slave mode
  * @param  htim : TIM handle
  * @param  sSlaveConfig: pointer to a TIM_SlaveConfigTypeDef structure that
  *         contains the selected trigger (internal trigger input, filtered
  *         timer input or external trigger input) and the ) and the Slave 
  *         mode (Disable, Reset, Gated, Trigger, External clock mode 1). 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchronization(TIM_HandleTypeDef *htim, TIM_SlaveConfigTypeDef * sSlaveConfig)
{
  /* Check the parameters */
  assert_param(IS_TIM_SLAVE_INSTANCE(htim->Instance));
  assert_param(IS_TIM_SLAVE_MODE(sSlaveConfig->SlaveMode));
  assert_param(IS_TIM_TRIGGER_SELECTION(sSlaveConfig->InputTrigger));
   
  __HAL_LOCK(htim);
  
  htim->State = HAL_TIM_STATE_BUSY;

  /* Configuration in slave mode */
  TIM_SlaveTimer_SetConfig(htim, sSlaveConfig);

  /* Disable Trigger Interrupt */
  __HAL_TIM_DISABLE_IT(htim, TIM_IT_TRIGGER);
  
  /* Disable Trigger DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_TRIGGER);

  /* Set the new state */
  htim->State = HAL_TIM_STATE_READY;
     
  __HAL_UNLOCK(htim);  
  
  return HAL_OK;
} 

/**
  * @brief  Configures the TIM in Slave mode in interrupt mode
  * @param  htim : TIM handle.
  * @param  sSlaveConfig: pointer to a TIM_SlaveConfigTypeDef structure that
  *         contains the selected trigger (internal trigger input, filtered
  *         timer input or external trigger input) and the ) and the Slave
  *         mode (Disable, Reset, Gated, Trigger, External clock mode 1).
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_TIM_SlaveConfigSynchronization_IT(TIM_HandleTypeDef *htim,
                                                        TIM_SlaveConfigTypeDef * sSlaveConfig)
{
  /* Check the parameters */
  assert_param(IS_TIM_SLAVE_INSTANCE(htim->Instance));
  assert_param(IS_TIM_SLAVE_MODE(sSlaveConfig->SlaveMode));
  assert_param(IS_TIM_TRIGGER_SELECTION(sSlaveConfig->InputTrigger));

  __HAL_LOCK(htim);

  htim->State = HAL_TIM_STATE_BUSY;

  TIM_SlaveTimer_SetConfig(htim, sSlaveConfig);

  /* Enable Trigger Interrupt */
  __HAL_TIM_ENABLE_IT(htim, TIM_IT_TRIGGER);

  /* Disable Trigger DMA request */
  __HAL_TIM_DISABLE_DMA(htim, TIM_DMA_TRIGGER);

  htim->State = HAL_TIM_STATE_READY;

  __HAL_UNLOCK(htim);

  return HAL_OK;
}

/**
  * @brief  Read the captured value from Capture Compare unit
  * @param  htim : TIM handle
  * @param  Channel: TIM Channels to be enabled.
  *          This parameter can be one of the following values:
  *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
  *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
  *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
  *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
  * @retval Captured value
  */
uint32_t HAL_TIM_ReadCapturedValue(TIM_HandleTypeDef *htim, uint32_t Channel)
{
  uint32_t tmpreg = 0;
  
  __HAL_LOCK(htim);
  
  switch (Channel)
  {
    case TIM_CHANNEL_1:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      
      /* Return the capture 1 value */
      tmpreg = htim->Instance->CCR1;
      
      break;
    }
    case TIM_CHANNEL_2:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      
      /* Return the capture 2 value */
      tmpreg = htim->Instance->CCR2;
      
      break;
    }
    
    case TIM_CHANNEL_3:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));
      
      /* Return the capture 3 value */
      tmpreg = htim->Instance->CCR3;
      
      break;
    }
    
    case TIM_CHANNEL_4:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));
      
      /* Return the capture 4 value */
      tmpreg = htim->Instance->CCR4;
      
      break;
    }
    
    default:
    break;  
  }
     
  __HAL_UNLOCK(htim);  
  return tmpreg;
}

/**
  * @}
  */
  
/** @addtogroup TIM_Exported_Functions_Group9
 *  @brief    TIM Callbacks functions 
 *
@verbatim
  ==============================================================================
                        ##### TIM Callbacks functions #####
  ==============================================================================  
 [..]  
   This section provides TIM callback functions:
   (+) Timer Period elapsed callback
   (+) Timer Output Compare callback
   (+) Timer Input capture callback
   (+) Timer Trigger callback
   (+) Timer Error callback

@endverbatim
  * @{
  */

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
  
}
/**
  * @brief  Output Compare callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_OC_DelayElapsedCallback could be implemented in the user file
   */
}
/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim: TIM IC handle
  * @retval None
  */
__weak void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_IC_CaptureCallback could be implemented in the user file
   */
}

/**
  * @brief  PWM Pulse finished callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PWM_PulseFinishedCallback could be implemented in the user file
   */
}

/**
  * @brief  Hall Trigger detection callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_TriggerCallback could be implemented in the user file
   */
}

/**
  * @brief  Timer error callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
__weak void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_TIM_ErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @addtogroup TIM_Exported_Functions_Group10
 *  @brief   Peripheral State functions 
 *
@verbatim   
  ==============================================================================
                      ##### Peripheral State functions #####
  ==============================================================================  
  [..]
    This subsection permits to get in run-time the status of the peripheral 
    and the data flow.

@endverbatim
  * @{
  */

/**
  * @brief  Return the TIM Base state
  * @param  htim : TIM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_Base_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  Return the TIM OC state
  * @param  htim: TIM Ouput Compare handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_OC_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  Return the TIM PWM state
  * @param  htim : TIM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_PWM_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  Return the TIM Input Capture state
  * @param  htim : TIM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_IC_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  Return the TIM One Pulse Mode state
  * @param  htim: TIM OPM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_OnePulse_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}

/**
  * @brief  Return the TIM Encoder Mode state
  * @param  htim : TIM handle
  * @retval HAL state
  */
HAL_TIM_StateTypeDef HAL_TIM_Encoder_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
}



/**
  * @brief  TIM DMA error callback 
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void TIM_DMAError(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  htim->State= HAL_TIM_STATE_READY;
   
  HAL_TIM_ErrorCallback(htim);
}

/**
  * @brief  TIM DMA Delay Pulse complete callback. 
  * @param  hdma : pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void TIM_DMADelayPulseCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  htim->State= HAL_TIM_STATE_READY; 
  
  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
  }
  HAL_TIM_PWM_PulseFinishedCallback(htim);

  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}
/**
  * @brief  TIM DMA Capture complete callback. 
  * @param  hdma : pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void TIM_DMACaptureCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  htim->State= HAL_TIM_STATE_READY;
  
  if (hdma == htim->hdma[TIM_DMA_ID_CC1])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC2])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_2;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC3])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_3;
  }
  else if (hdma == htim->hdma[TIM_DMA_ID_CC4])
  {
    htim->Channel = HAL_TIM_ACTIVE_CHANNEL_4;
  }
  
  HAL_TIM_IC_CaptureCallback(htim); 
  
  htim->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
}


/**
  * @}
  */

/**
  * @}
  */
/*************************************************************/
/* Private functions                                         */
/*************************************************************/

/** @defgroup TIM_Private_Functions TIM Private Functions
  * @{
  */
/**
  * @brief  TIM DMA Period Elapse complete callback. 
  * @param  hdma : pointer to DMA handle.
  * @retval None
  */
static void TIM_DMAPeriodElapsedCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
  
  htim->State= HAL_TIM_STATE_READY;
  
  HAL_TIM_PeriodElapsedCallback(htim);
}


/**
  * @brief  TIM DMA Trigger callback. 
  * @param  hdma : pointer to DMA handle.
  * @retval None
  */
static void TIM_DMATriggerCplt(DMA_HandleTypeDef *hdma)
{
  TIM_HandleTypeDef* htim = ( TIM_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;  
  
  htim->State= HAL_TIM_STATE_READY; 
  
  HAL_TIM_TriggerCallback(htim);
}

/**
  * @brief  Time Base configuration
  * @param  TIMx : TIM peripheral
  * @param   Structure : TIM Base configuration structure
  * @retval None
  */
static void TIM_Base_SetConfig(TIM_TypeDef *TIMx, TIM_Base_InitTypeDef *Structure)
{
  uint32_t tmpcr1 = 0;
  tmpcr1 = TIMx->CR1;
  
  /* Set TIM Time Base Unit parameters ---------------------------------------*/
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)
  {
    /* Select the Counter Mode */
    tmpcr1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    tmpcr1 |= Structure->CounterMode;
  }
 
  if(IS_TIM_CC1_INSTANCE(TIMx) != RESET)  
  {
    /* Set the clock division */
    tmpcr1 &= ~TIM_CR1_CKD;
    tmpcr1 |= (uint32_t)Structure->ClockDivision;
  }

  TIMx->CR1 = tmpcr1;

  /* Set the Autoreload value */
  TIMx->ARR = (uint32_t)Structure->Period ;
 
  /* Set the Prescaler value */
  TIMx->PSC = (uint32_t)Structure->Prescaler;

  /* Generate an update event to reload the Prescaler value immediatly */
  TIMx->EGR = TIM_EGR_UG;
}

/**
  * @brief  Time Ouput Compare 1 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config: The ouput configuration structure
  * @retval None
  */
static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx = 0;
  uint32_t tmpccer = 0;
  uint32_t tmpcr2 = 0;  

  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= ~TIM_CCER_CC1E;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 = TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= ~TIM_CCMR1_OC1M;
  tmpccmrx &= ~TIM_CCMR1_CC1S;
  /* Select the Output Compare Mode */
  tmpccmrx |= OC_Config->OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC1P;
  /* Set the Output Compare Polarity */
  tmpccer |= OC_Config->OCPolarity;

  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;
  
  /* Set the Capture Compare Register value */
  TIMx->CCR1 = OC_Config->Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;  
} 

/**
  * @brief  Time Ouput Compare 2 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config: The ouput configuration structure
  * @retval None
  */
static void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx = 0;
  uint32_t tmpccer = 0;
  uint32_t tmpcr2 = 0;
   
  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;
  
  /* Get the TIMx CCER register value */  
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 = TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR1_OC2M;
  tmpccmrx &= ~TIM_CCMR1_CC2S;
  
  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8);
  
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC2P;
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 4);
    
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;
  
  /* Set the Capture Compare Register value */
  TIMx->CCR2 = OC_Config->Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Time Ouput Compare 3 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config: The ouput configuration structure
  * @retval None
  */
static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx = 0;
  uint32_t tmpccer = 0;
  uint32_t tmpcr2 = 0;   

  /* Disable the Channel 3: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC3E;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 = TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR2_OC3M;
  tmpccmrx &= ~TIM_CCMR2_CC3S;  
  /* Select the Output Compare Mode */
  tmpccmrx |= OC_Config->OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC3P;
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 8);
    
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;
  
  /* Set the Capture Compare Register value */
  TIMx->CCR3 = OC_Config->Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Time Ouput Compare 4 configuration
  * @param  TIMx to select the TIM peripheral
  * @param  OC_Config: The ouput configuration structure
  * @retval None
  */
static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
  uint32_t tmpccmrx = 0;
  uint32_t tmpccer = 0;
  uint32_t tmpcr2 = 0;

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= ~TIM_CCER_CC4E;
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 = TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= ~TIM_CCMR2_OC4M;
  tmpccmrx &= ~TIM_CCMR2_CC4S;
  
  /* Select the Output Compare Mode */
  tmpccmrx |= (OC_Config->OCMode << 8);
  
  /* Reset the Output Polarity level */
  tmpccer &= ~TIM_CCER_CC4P;
  /* Set the Output Compare Polarity */
  tmpccer |= (OC_Config->OCPolarity << 12);
   
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 */  
  TIMx->CCMR2 = tmpccmrx;
    
  /* Set the Capture Compare Register value */
  TIMx->CCR4 = OC_Config->Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI1 as Input.
  * @param  TIMx to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge  
  * @param  TIM_ICSelection: specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSelection_DirectTI: TIM Input 1 is selected to be connected to IC1.
  *            @arg TIM_ICSelection_IndirectTI: TIM Input 1 is selected to be connected to IC2.
  *            @arg TIM_ICSelection_TRC: TIM Input 1 is selected to be connected to TRC.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI1_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;
  
  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= ~TIM_CCER_CC1E;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;

  /* Select the Input */
  if(IS_TIM_CC2_INSTANCE(TIMx) != RESET)
  {
    tmpccmr1 &= ~TIM_CCMR1_CC1S;
    tmpccmr1 |= TIM_ICSelection;
  } 
  else
  {
    tmpccmr1 &= ~TIM_CCMR1_CC1S;
    tmpccmr1 |= TIM_CCMR1_CC1S_0;
  }
 
  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;
  tmpccmr1 |= ((TIM_ICFilter << 4) & TIM_CCMR1_IC1F);

  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  tmpccer |= (TIM_ICPolarity & (TIM_CCER_CC1P | TIM_CCER_CC1NP));

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the Polarity and Filter for TI1.
  * @param  TIMx to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI1_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;
  
  /* Disable the Channel 1: Reset the CC1E Bit */
  tmpccer = TIMx->CCER;
  TIMx->CCER &= ~TIM_CCER_CC1E;
  tmpccmr1 = TIMx->CCMR1;    
  
  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC1F;
  tmpccmr1 |= (TIM_ICFilter << 4);
  
  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= ~(TIM_CCER_CC1P | TIM_CCER_CC1NP);
  tmpccer |= TIM_ICPolarity;
  
  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI2 as Input.
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge   
  * @param  TIM_ICSelection: specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSelection_DirectTI: TIM Input 2 is selected to be connected to IC2.
  *            @arg TIM_ICSelection_IndirectTI: TIM Input 2 is selected to be connected to IC1.
  *            @arg TIM_ICSelection_TRC: TIM Input 2 is selected to be connected to TRC.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI2_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;

  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  
  /* Select the Input */
  tmpccmr1 &= ~TIM_CCMR1_CC2S;
  tmpccmr1 |= (TIM_ICSelection << 8);
  
  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC2F;
  tmpccmr1 |= ((TIM_ICFilter << 12) & TIM_CCMR1_IC2F);

  /* Select the Polarity and set the CC2E Bit */
  tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
  tmpccer |= ((TIM_ICPolarity << 4) & (TIM_CCER_CC2P | TIM_CCER_CC2NP));

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the Polarity and Filter for TI2.
  * @param  TIMx to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI2_ConfigInputStage(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;
  
  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= ~TIM_CCER_CC2E;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  
  /* Set the filter */
  tmpccmr1 &= ~TIM_CCMR1_IC2F;
  tmpccmr1 |= (TIM_ICFilter << 12);

  /* Select the Polarity and set the CC2E Bit */
  tmpccer &= ~(TIM_CCER_CC2P | TIM_CCER_CC2NP);
  tmpccer |= (TIM_ICPolarity << 4);

  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI3 as Input.
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge         
  * @param  TIM_ICSelection: specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSelection_DirectTI: TIM Input 3 is selected to be connected to IC3.
  *            @arg TIM_ICSelection_IndirectTI: TIM Input 3 is selected to be connected to IC4.
  *            @arg TIM_ICSelection_TRC: TIM Input 3 is selected to be connected to TRC.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI3_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr2 = 0;
  uint32_t tmpccer = 0;

  /* Disable the Channel 3: Reset the CC3E Bit */
  TIMx->CCER &= ~TIM_CCER_CC3E;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;

  /* Select the Input */
  tmpccmr2 &= ~TIM_CCMR2_CC3S;
  tmpccmr2 |= TIM_ICSelection;

  /* Set the filter */
  tmpccmr2 &= ~TIM_CCMR2_IC3F;
  tmpccmr2 |= ((TIM_ICFilter << 4) & TIM_CCMR2_IC3F);

  /* Select the Polarity and set the CC3E Bit */
  tmpccer &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);
  tmpccer |= ((TIM_ICPolarity << 8) & (TIM_CCER_CC3P | TIM_CCER_CC3NP));

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI4 as Input.
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ICPolarity : The Input Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICPolarity_Rising
  *            @arg TIM_ICPolarity_Falling
  *            @arg TIM_ICPolarity_BothEdge     
  * @param  TIM_ICSelection: specifies the input to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_ICSelection_DirectTI: TIM Input 4 is selected to be connected to IC4.
  *            @arg TIM_ICSelection_IndirectTI: TIM Input 4 is selected to be connected to IC3.
  *            @arg TIM_ICSelection_TRC: TIM Input 4 is selected to be connected to TRC.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *          This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TIM_TI4_SetConfig(TIM_TypeDef *TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr2 = 0;
  uint32_t tmpccer = 0;

  /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= ~TIM_CCER_CC4E;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;

  /* Select the Input */
  tmpccmr2 &= ~TIM_CCMR2_CC4S;
  tmpccmr2 |= (TIM_ICSelection << 8);

  /* Set the filter */
  tmpccmr2 &= ~TIM_CCMR2_IC4F;
  tmpccmr2 |= ((TIM_ICFilter << 12) & TIM_CCMR2_IC4F);

  /* Select the Polarity and set the CC4E Bit */
  tmpccer &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);
  tmpccer |= ((TIM_ICPolarity << 12) & (TIM_CCER_CC4P | TIM_CCER_CC4NP));

  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
}

/**
  * @brief  Selects the Input Trigger source
  * @param  TIMx to select the TIM peripheral
  * @param  InputTriggerSource: The Input Trigger source.
  *          This parameter can be one of the following values:
  *            @arg TIM_TS_ITR0: Internal Trigger 0
  *            @arg TIM_TS_ITR1: Internal Trigger 1
  *            @arg TIM_TS_ITR2: Internal Trigger 2
  *            @arg TIM_TS_ITR3: Internal Trigger 3
  *            @arg TIM_TS_TI1F_ED: TI1 Edge Detector
  *            @arg TIM_TS_TI1FP1: Filtered Timer Input 1
  *            @arg TIM_TS_TI2FP2: Filtered Timer Input 2
  *            @arg TIM_TS_ETRF: External Trigger input
  * @retval None
  */
static void TIM_ITRx_SetConfig(TIM_TypeDef *TIMx, uint16_t InputTriggerSource)
{
  uint32_t tmpsmcr = 0;
  
   /* Get the TIMx SMCR register value */
   tmpsmcr = TIMx->SMCR;
   /* Reset the TS Bits */
   tmpsmcr &= ~TIM_SMCR_TS;
   /* Set the Input Trigger source and the slave mode*/
   tmpsmcr |= InputTriggerSource | TIM_SLAVEMODE_EXTERNAL1;
   /* Write to TIMx SMCR */
   TIMx->SMCR = tmpsmcr;
}
/**
  * @brief  Configures the TIMx External Trigger (ETR).
  * @param  TIMx to select the TIM peripheral
  * @param  TIM_ExtTRGPrescaler: The external Trigger Prescaler.
  *          This parameter can be one of the following values:
  *            @arg TIM_ExtTRGPSC_DIV1: ETRP Prescaler OFF.
  *            @arg TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2.
  *            @arg TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4.
  *            @arg TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
  * @param  TIM_ExtTRGPolarity: The external Trigger Polarity.
  *          This parameter can be one of the following values:
  *            @arg TIM_ExtTRGPolarity_Inverted: active low or falling edge active.
  *            @arg TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
  * @param  ExtTRGFilter: External Trigger Filter.
  *          This parameter must be a value between 0x00 and 0x0F
  * @retval None
  */
static void TIM_ETR_SetConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler,
                       uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter)
{
  uint32_t tmpsmcr = 0;

  tmpsmcr = TIMx->SMCR;

  /* Reset the ETR Bits */
  tmpsmcr &= ~(TIM_SMCR_ETF | TIM_SMCR_ETPS | TIM_SMCR_ECE | TIM_SMCR_ETP);

  /* Set the Prescaler, the Filter value and the Polarity */
  tmpsmcr |= (uint32_t)(TIM_ExtTRGPrescaler | (TIM_ExtTRGPolarity | (ExtTRGFilter << 8)));

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
} 

/**
  * @brief  Enables or disables the TIM Capture Compare Channel x.
  * @param  TIMx to select the TIM peripheral
  * @param  Channel: specifies the TIM Channel
  *          This parameter can be one of the following values:
  *            @arg TIM_Channel_1: TIM Channel 1
  *            @arg TIM_Channel_2: TIM Channel 2
  *            @arg TIM_Channel_3: TIM Channel 3
  *            @arg TIM_Channel_4: TIM Channel 4
  * @param  ChannelState: specifies the TIM Channel CCxE bit new state.
  *          This parameter can be: TIM_CCx_ENABLE or TIM_CCx_Disable. 
  * @retval None
  */
static void TIM_CCxChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelState)
{
  uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_TIM_CCX_INSTANCE(TIMx,Channel));

  tmp = TIM_CCER_CC1E << Channel;

  /* Reset the CCxE Bit */
  TIMx->CCER &= ~tmp;

  /* Set or reset the CCxE Bit */ 
  TIMx->CCER |= (uint32_t)(ChannelState << Channel);
}
/**
  * @brief  Set the slave timer configuration.
  * @param  htim : TIM handle
  * @param  sSlaveConfig: pointer to a TIM_SlaveConfigTypeDef structure that
  *         contains the selected trigger (internal trigger input, filtered
  *         timer input or external trigger input) and the ) and the Slave
  *         mode (Disable, Reset, Gated, Trigger, External clock mode 1).
  * @retval None
  */
static void TIM_SlaveTimer_SetConfig(TIM_HandleTypeDef *htim,
                                     TIM_SlaveConfigTypeDef * sSlaveConfig)
{
  uint32_t tmpsmcr = 0;
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;

  /* Get the TIMx SMCR register value */
  tmpsmcr = htim->Instance->SMCR;

  /* Reset the Trigger Selection Bits */
  tmpsmcr &= ~TIM_SMCR_TS;
  /* Set the Input Trigger source */
  tmpsmcr |= sSlaveConfig->InputTrigger;

  /* Reset the slave mode Bits */
  tmpsmcr &= ~TIM_SMCR_SMS;
  /* Set the slave mode */
  tmpsmcr |= sSlaveConfig->SlaveMode;

  /* Write to TIMx SMCR */
  htim->Instance->SMCR = tmpsmcr;

  /* Configure the trigger prescaler, filter, and polarity */
  switch (sSlaveConfig->InputTrigger)
  {
  case TIM_TS_ETRF:
    {
      /* Check the parameters */
      assert_param(IS_TIM_ETR_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERPRESCALER(sSlaveConfig->TriggerPrescaler));
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));
      /* Configure the ETR Trigger source */
      TIM_ETR_SetConfig(htim->Instance,
                        sSlaveConfig->TriggerPrescaler,
                        sSlaveConfig->TriggerPolarity,
                        sSlaveConfig->TriggerFilter);
    }
    break;

  case TIM_TS_TI1F_ED:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));

      /* Disable the Channel 1: Reset the CC1E Bit */
      tmpccer = htim->Instance->CCER;
      htim->Instance->CCER &= ~TIM_CCER_CC1E;
      tmpccmr1 = htim->Instance->CCMR1;

      /* Set the filter */
      tmpccmr1 &= ~TIM_CCMR1_IC1F;
      tmpccmr1 |= ((sSlaveConfig->TriggerFilter) << 4);

      /* Write to TIMx CCMR1 and CCER registers */
      htim->Instance->CCMR1 = tmpccmr1;
      htim->Instance->CCER = tmpccer;

    }
    break;

  case TIM_TS_TI1FP1:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));

      /* Configure TI1 Filter and Polarity */
      TIM_TI1_ConfigInputStage(htim->Instance,
                               sSlaveConfig->TriggerPolarity,
                               sSlaveConfig->TriggerFilter);
    }
    break;

  case TIM_TS_TI2FP2:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
      assert_param(IS_TIM_TRIGGERPOLARITY(sSlaveConfig->TriggerPolarity));
      assert_param(IS_TIM_TRIGGERFILTER(sSlaveConfig->TriggerFilter));

      /* Configure TI2 Filter and Polarity */
      TIM_TI2_ConfigInputStage(htim->Instance,
                                sSlaveConfig->TriggerPolarity,
                                sSlaveConfig->TriggerFilter);
    }
    break;

  case TIM_TS_ITR0:
    {
      /* Check the parameter */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    }
    break;

  case TIM_TS_ITR1:
    {
      /* Check the parameter */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    }
    break;

  case TIM_TS_ITR2:
    {
      /* Check the parameter */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    }
    break;

  case TIM_TS_ITR3:
    {
      /* Check the parameter */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));
    }
    break;

  default:
    break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* HAL_TIM_MODULE_ENABLED */
/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

