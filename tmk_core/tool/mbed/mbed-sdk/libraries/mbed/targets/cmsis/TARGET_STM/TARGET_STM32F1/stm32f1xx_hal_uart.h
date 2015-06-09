/**
  ******************************************************************************
  * @file    stm32f1xx_hal_uart.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    15-December-2014
  * @brief   Header file of UART HAL module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F1xx_HAL_UART_H
#define __STM32F1xx_HAL_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal_def.h"

/** @addtogroup STM32F1xx_HAL_Driver
  * @{
  */

/** @addtogroup UART
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup UART_Exported_Types UART Exported Types
  * @{
  */ 


/** 
  * @brief UART Init Structure definition
  */ 
typedef struct
{
  uint32_t BaudRate;                  /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                           - IntegerDivider = ((PCLKx) / (16 * (huart->Init.BaudRate)))
                                           - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 16) + 0.5 */

  uint32_t WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint32_t Parity;                    /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
 
  uint32_t Mode;                      /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Mode */

  uint32_t HwFlowCtl;                 /*!< Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */
  
  uint32_t OverSampling;              /*!< Specifies whether the Over sampling 8 is enabled or disabled, to achieve higher speed (up to fPCLK/8).
                                           This parameter can be a value of @ref UART_Over_Sampling. This feature is not available 
                                           on STM32F1xx family, so OverSampling parameter should always be set to 16. */ 
}UART_InitTypeDef;

/** 
  * @brief HAL UART State structures definition  
  */ 
typedef enum
{
  HAL_UART_STATE_RESET             = 0x00,    /*!< Peripheral is not initialized                      */
  HAL_UART_STATE_READY             = 0x01,    /*!< Peripheral Initialized and ready for use           */
  HAL_UART_STATE_BUSY              = 0x02,    /*!< an internal process is ongoing                     */
  HAL_UART_STATE_BUSY_TX           = 0x12,    /*!< Data Transmission process is ongoing               */
  HAL_UART_STATE_BUSY_RX           = 0x22,    /*!< Data Reception process is ongoing                  */
  HAL_UART_STATE_BUSY_TX_RX        = 0x32,    /*!< Data Transmission and Reception process is ongoing */
  HAL_UART_STATE_TIMEOUT           = 0x03,    /*!< Timeout state                                      */
  HAL_UART_STATE_ERROR             = 0x04     /*!< Error                                              */
}HAL_UART_StateTypeDef;


/** 
  * @brief  UART handle Structure definition  
  */  
typedef struct
{
  USART_TypeDef                 *Instance;        /*!< UART registers base address        */

  UART_InitTypeDef              Init;             /*!< UART communication parameters      */

  uint8_t                       *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */

  uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */

  uint16_t                      TxXferCount;      /*!< UART Tx Transfer Counter           */

  uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */

  uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */

  uint16_t                      RxXferCount;      /*!< UART Rx Transfer Counter           */  

  DMA_HandleTypeDef             *hdmatx;          /*!< UART Tx DMA Handle parameters      */

  DMA_HandleTypeDef             *hdmarx;          /*!< UART Rx DMA Handle parameters      */

  HAL_LockTypeDef               Lock;             /*!< Locking object                     */

  __IO HAL_UART_StateTypeDef    State;            /*!< UART communication state           */
  
  __IO uint32_t                 ErrorCode;        /*!< UART Error code                    */

}UART_HandleTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup UART_Exported_Constants UART Exported constants
  * @{
  */

/** @defgroup UART_Error_Codes   UART Error Codes
  * @{
  */

#define HAL_UART_ERROR_NONE      ((uint32_t)0x00)    /*!< No error            */
#define HAL_UART_ERROR_PE        ((uint32_t)0x01)    /*!< Parity error        */
#define HAL_UART_ERROR_NE        ((uint32_t)0x02)    /*!< Noise error         */
#define HAL_UART_ERROR_FE        ((uint32_t)0x04)    /*!< frame error         */
#define HAL_UART_ERROR_ORE       ((uint32_t)0x08)    /*!< Overrun error       */
#define HAL_UART_ERROR_DMA       ((uint32_t)0x10)    /*!< DMA transfer error  */
  
/**
  * @}
  */




/** @defgroup UART_Word_Length   UART Word Length
  * @{
  */
#define UART_WORDLENGTH_8B                  ((uint32_t)0x00000000)
#define UART_WORDLENGTH_9B                  ((uint32_t)USART_CR1_M)
/**
  * @}
  */

/** @defgroup UART_Stop_Bits   UART Number of Stop Bits
  * @{
  */
#define UART_STOPBITS_1                     ((uint32_t)0x00000000)
#define UART_STOPBITS_2                     ((uint32_t)USART_CR2_STOP_1)
/**
  * @}
  */ 

/** @defgroup UART_Parity  UART Parity
  * @{
  */ 
#define UART_PARITY_NONE                    ((uint32_t)0x00000000)
#define UART_PARITY_EVEN                    ((uint32_t)USART_CR1_PCE)
#define UART_PARITY_ODD                     ((uint32_t)(USART_CR1_PCE | USART_CR1_PS)) 
/**
  * @}
  */ 

/** @defgroup UART_Hardware_Flow_Control UART Hardware Flow Control
  * @{
  */ 
#define UART_HWCONTROL_NONE                  ((uint32_t)0x00000000)
#define UART_HWCONTROL_RTS                   ((uint32_t)USART_CR3_RTSE)
#define UART_HWCONTROL_CTS                   ((uint32_t)USART_CR3_CTSE)
#define UART_HWCONTROL_RTS_CTS               ((uint32_t)(USART_CR3_RTSE | USART_CR3_CTSE))
/**
  * @}
  */

/** @defgroup UART_Mode UART Transfer Mode
  * @{
  */ 
#define UART_MODE_RX                        ((uint32_t)USART_CR1_RE)
#define UART_MODE_TX                        ((uint32_t)USART_CR1_TE)
#define UART_MODE_TX_RX                     ((uint32_t)(USART_CR1_TE |USART_CR1_RE))

/**
  * @}
  */
    
 /** @defgroup UART_State  UART State
  * @{
  */ 
#define UART_STATE_DISABLE                  ((uint32_t)0x00000000)
#define UART_STATE_ENABLE                   ((uint32_t)USART_CR1_UE)
/**
  * @}
  */

/** @defgroup UART_Over_Sampling UART Over Sampling
  * @{
  */
#define UART_OVERSAMPLING_16                    ((uint32_t)0x00000000)
/**
  * @}
  */

/** @defgroup UART_LIN_Break_Detection_Length  UART LIN Break Detection Length
  * @{
  */  
#define UART_LINBREAKDETECTLENGTH_10B      ((uint32_t)0x00000000)
#define UART_LINBREAKDETECTLENGTH_11B      ((uint32_t)USART_CR2_LBDL)
/**
  * @}
  */

/** @defgroup UART_WakeUp_functions UART Wakeup Functions
  * @{
  */
#define UART_WAKEUPMETHOD_IDLELINE                ((uint32_t)0x00000000)
#define UART_WAKEUPMETHOD_ADDRESSMARK             ((uint32_t)USART_CR1_WAKE)
/**
  * @}
  */

/** @defgroup UART_Flags   UART FLags
  *        Elements values convention: 0xXXXX
  *           - 0xXXXX  : Flag mask in the SR register
  * @{
  */
#define UART_FLAG_CTS                       ((uint32_t)USART_SR_CTS)
#define UART_FLAG_LBD                       ((uint32_t)USART_SR_LBD)
#define UART_FLAG_TXE                       ((uint32_t)USART_SR_TXE)
#define UART_FLAG_TC                        ((uint32_t)USART_SR_TC)
#define UART_FLAG_RXNE                      ((uint32_t)USART_SR_RXNE)
#define UART_FLAG_IDLE                      ((uint32_t)USART_SR_IDLE)
#define UART_FLAG_ORE                       ((uint32_t)USART_SR_ORE)
#define UART_FLAG_NE                        ((uint32_t)USART_SR_NE)
#define UART_FLAG_FE                        ((uint32_t)USART_SR_FE)
#define UART_FLAG_PE                        ((uint32_t)USART_SR_PE)
/**
  * @}
  */

/** @defgroup UART_Interrupt_definition  UART Interrupt Definitions
  *        Elements values convention: 0xY000XXXX
  *           - XXXX  : Interrupt mask (16 bits) in the Y register
  *           - Y  : Interrupt source register (2bits)
  *                 - 0001: CR1 register
  *                 - 0010: CR2 register
  *                 - 0011: CR3 register
  *
  * @{
  */ 

#define UART_IT_PE                       ((uint32_t)(UART_CR1_REG_INDEX << 28 | USART_CR1_PEIE))
#define UART_IT_TXE                      ((uint32_t)(UART_CR1_REG_INDEX << 28 | USART_CR1_TXEIE))
#define UART_IT_TC                       ((uint32_t)(UART_CR1_REG_INDEX << 28 | USART_CR1_TCIE))
#define UART_IT_RXNE                     ((uint32_t)(UART_CR1_REG_INDEX << 28 | USART_CR1_RXNEIE))
#define UART_IT_IDLE                     ((uint32_t)(UART_CR1_REG_INDEX << 28 | USART_CR1_IDLEIE))

#define UART_IT_LBD                      ((uint32_t)(UART_CR2_REG_INDEX << 28 | USART_CR2_LBDIE))

#define UART_IT_CTS                      ((uint32_t)(UART_CR3_REG_INDEX << 28 | USART_CR3_CTSIE))
#define UART_IT_ERR                      ((uint32_t)(UART_CR3_REG_INDEX << 28 | USART_CR3_EIE))

/**
  * @}
  */

/**
  * @}
  */

    
/* Exported macro ------------------------------------------------------------*/
/** @defgroup UART_Exported_Macros UART Exported Macros
  * @{
  */


/** @brief Reset UART handle state
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = HAL_UART_STATE_RESET)

/** @brief  Flush the UART DR register 
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  */
#define __HAL_UART_FLUSH_DRREGISTER(__HANDLE__) ((__HANDLE__)->Instance->DR)

/** @brief  Check whether the specified UART flag is set or not.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @param  __FLAG__: specifies the flag to check.
  *        This parameter can be one of the following values:
  *            @arg UART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5)
  *            @arg UART_FLAG_LBD:  LIN Break detection flag
  *            @arg UART_FLAG_TXE:  Transmit data register empty flag
  *            @arg UART_FLAG_TC:   Transmission Complete flag
  *            @arg UART_FLAG_RXNE: Receive data register not empty flag
  *            @arg UART_FLAG_IDLE: Idle Line detection flag
  *            @arg UART_FLAG_ORE:  OverRun Error flag
  *            @arg UART_FLAG_NE:   Noise Error flag
  *            @arg UART_FLAG_FE:   Framing Error flag
  *            @arg UART_FLAG_PE:   Parity Error flag
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_UART_GET_FLAG(__HANDLE__, __FLAG__) (((__HANDLE__)->Instance->SR & (__FLAG__)) == (__FLAG__))   

/** @brief  Clear the specified UART pending flag.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @param  __FLAG__: specifies the flag to check.
  *          This parameter can be any combination of the following values:
  *            @arg UART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5).
  *            @arg UART_FLAG_LBD:  LIN Break detection flag.
  *            @arg UART_FLAG_TC:   Transmission Complete flag.
  *            @arg UART_FLAG_RXNE: Receive data register not empty flag.
  *   
  * @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
  *          error) and IDLE (Idle line detected) flags are cleared by software 
  *          sequence: a read operation to USART_SR register followed by a read
  *          operation to USART_DR register.
  * @note   RXNE flag can be also cleared by a read to the USART_DR register.
  * @note   TC flag can be also cleared by software sequence: a read operation to 
  *          USART_SR register followed by a write operation to USART_DR register.
  * @note   TXE flag is cleared only by a write to the USART_DR register.
  *   
  * @retval None
  */
#define __HAL_UART_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->SR = ~(__FLAG__))

/** @brief  Clear the UART PE pending flag.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_CLEAR_PEFLAG(__HANDLE__) \
do{                                         \
  __IO uint32_t tmpreg;                     \
  tmpreg = (__HANDLE__)->Instance->SR;      \
  tmpreg = (__HANDLE__)->Instance->DR;      \
  UNUSED(tmpreg);                           \
}while(0)



/** @brief  Clear the UART FE pending flag.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_CLEAR_FEFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  Clear the UART NE pending flag.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_CLEAR_NEFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  Clear the UART ORE pending flag.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_CLEAR_OREFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)

/** @brief  Clear the UART IDLE pending flag.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_CLEAR_IDLEFLAG(__HANDLE__) __HAL_UART_CLEAR_PEFLAG(__HANDLE__)
                                                 
/** @brief  Enable the specified UART interrupt.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @param  __INTERRUPT__: specifies the UART interrupt source to enable.
  *          This parameter can be one of the following values:
  *            @arg UART_IT_CTS:  CTS change interrupt
  *            @arg UART_IT_LBD:  LIN Break detection interrupt
  *            @arg UART_IT_TXE:  Transmit Data Register empty interrupt
  *            @arg UART_IT_TC:   Transmission complete interrupt
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg UART_IT_IDLE: Idle line detection interrupt
  *            @arg UART_IT_PE:   Parity Error interrupt
  *            @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @retval None
  */
#define __HAL_UART_ENABLE_IT(__HANDLE__, __INTERRUPT__)   ((((__INTERRUPT__) >> 28) == UART_CR1_REG_INDEX)? ((__HANDLE__)->Instance->CR1 |= ((__INTERRUPT__) & UART_IT_MASK)): \
                                                           (((__INTERRUPT__) >> 28) == UART_CR2_REG_INDEX)? ((__HANDLE__)->Instance->CR2 |=  ((__INTERRUPT__) & UART_IT_MASK)): \
                                                           ((__HANDLE__)->Instance->CR3 |= ((__INTERRUPT__) & UART_IT_MASK)))


/** @brief  Disable the specified UART interrupt.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @param  __INTERRUPT__: specifies the UART interrupt source to disable.
  *          This parameter can be one of the following values:
  *            @arg UART_IT_CTS:  CTS change interrupt
  *            @arg UART_IT_LBD:  LIN Break detection interrupt
  *            @arg UART_IT_TXE:  Transmit Data Register empty interrupt
  *            @arg UART_IT_TC:   Transmission complete interrupt
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg UART_IT_IDLE: Idle line detection interrupt
  *            @arg UART_IT_PE:   Parity Error interrupt
  *            @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @retval None
  */
#define __HAL_UART_DISABLE_IT(__HANDLE__, __INTERRUPT__)  ((((__INTERRUPT__) >> 28) == UART_CR1_REG_INDEX)? ((__HANDLE__)->Instance->CR1 &= ~((__INTERRUPT__) & UART_IT_MASK)): \
                                                           (((__INTERRUPT__) >> 28) == UART_CR2_REG_INDEX)? ((__HANDLE__)->Instance->CR2 &= ~((__INTERRUPT__) & UART_IT_MASK)): \
                                                           ((__HANDLE__)->Instance->CR3 &= ~ ((__INTERRUPT__) & UART_IT_MASK)))
    
/** @brief  Check whether the specified UART interrupt has occurred or not.
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @param  __IT__: specifies the UART interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg UART_IT_CTS: CTS change interrupt (not available for UART4 and UART5)
  *            @arg UART_IT_LBD: LIN Break detection interrupt
  *            @arg UART_IT_TXE: Transmit Data Register empty interrupt
  *            @arg UART_IT_TC:  Transmission complete interrupt
  *            @arg UART_IT_RXNE: Receive Data register not empty interrupt
  *            @arg UART_IT_IDLE: Idle line detection interrupt
  *            @arg UART_IT_ERR: Error interrupt
  * @retval The new state of __IT__ (TRUE or FALSE).
  */
#define __HAL_UART_GET_IT_SOURCE(__HANDLE__, __IT__) (((((__IT__) >> 28) == UART_CR1_REG_INDEX)? (__HANDLE__)->Instance->CR1:(((((uint32_t)(__IT__)) >> 28) == UART_CR2_REG_INDEX)? \
                                                      (__HANDLE__)->Instance->CR2 : (__HANDLE__)->Instance->CR3)) & (((uint32_t)(__IT__)) & UART_IT_MASK))

/** @brief  Enable CTS flow control 
  *         This macro allows to enable CTS hardware flow control for a given UART instance, 
  *         without need to call HAL_UART_Init() function.
  *         As involving direct access to UART registers, usage of this macro should be fully endorsed by user.
  * @note   As macro is expected to be used for modifying CTS Hw flow control feature activation, without need
  *         for USART instance Deinit/Init, following conditions for macro call should be fulfilled :
  *           - UART instance should have already been initialised (through call of HAL_UART_Init() )
  *           - macro could only be called when corresponding UART instance is disabled (i.e __HAL_UART_DISABLE(__HANDLE__))
  *             and should be followed by an Enable macro (i.e __HAL_UART_ENABLE(__HANDLE__)).                                                                                                                  
  * @param  __HANDLE__: specifies the UART Handle.
  *         This parameter can be any USARTx (supporting the HW Flow control feature).
  *         It is used to select the USART peripheral (USART availability and x value depending on device).
  * @retval None
  */
#define __HAL_UART_HWCONTROL_CTS_ENABLE(__HANDLE__)        \
  do{                                                      \
    SET_BIT((__HANDLE__)->Instance->CR3, USART_CR3_CTSE);  \
    (__HANDLE__)->Init.HwFlowCtl |= USART_CR3_CTSE;        \
  } while(0)

/** @brief  Disable CTS flow control 
  *         This macro allows to disable CTS hardware flow control for a given UART instance, 
  *         without need to call HAL_UART_Init() function.
  *         As involving direct access to UART registers, usage of this macro should be fully endorsed by user.
  * @note   As macro is expected to be used for modifying CTS Hw flow control feature activation, without need
  *         for USART instance Deinit/Init, following conditions for macro call should be fulfilled :
  *           - UART instance should have already been initialised (through call of HAL_UART_Init() )
  *           - macro could only be called when corresponding UART instance is disabled (i.e __HAL_UART_DISABLE(__HANDLE__))
  *             and should be followed by an Enable macro (i.e __HAL_UART_ENABLE(__HANDLE__)). 
  * @param  __HANDLE__: specifies the UART Handle.
  *         This parameter can be any USARTx (supporting the HW Flow control feature).
  *         It is used to select the USART peripheral (USART availability and x value depending on device).
  * @retval None
  */
#define __HAL_UART_HWCONTROL_CTS_DISABLE(__HANDLE__)        \
  do{                                                       \
    CLEAR_BIT((__HANDLE__)->Instance->CR3, USART_CR3_CTSE); \
    (__HANDLE__)->Init.HwFlowCtl &= ~(USART_CR3_CTSE);      \
  } while(0)

/** @brief  Enable RTS flow control 
  *         This macro allows to enable RTS hardware flow control for a given UART instance, 
  *         without need to call HAL_UART_Init() function.
  *         As involving direct access to UART registers, usage of this macro should be fully endorsed by user.
  * @note   As macro is expected to be used for modifying RTS Hw flow control feature activation, without need
  *         for USART instance Deinit/Init, following conditions for macro call should be fulfilled :
  *           - UART instance should have already been initialised (through call of HAL_UART_Init() )
  *           - macro could only be called when corresponding UART instance is disabled (i.e __HAL_UART_DISABLE(__HANDLE__))
  *             and should be followed by an Enable macro (i.e __HAL_UART_ENABLE(__HANDLE__)). 
  * @param  __HANDLE__: specifies the UART Handle.
  *         This parameter can be any USARTx (supporting the HW Flow control feature).
  *         It is used to select the USART peripheral (USART availability and x value depending on device).
  * @retval None
  */
#define __HAL_UART_HWCONTROL_RTS_ENABLE(__HANDLE__)       \
  do{                                                     \
    SET_BIT((__HANDLE__)->Instance->CR3, USART_CR3_RTSE); \
    (__HANDLE__)->Init.HwFlowCtl |= USART_CR3_RTSE;       \
  } while(0)

/** @brief  Disable RTS flow control 
  *         This macro allows to disable RTS hardware flow control for a given UART instance, 
  *         without need to call HAL_UART_Init() function.
  *         As involving direct access to UART registers, usage of this macro should be fully endorsed by user.
  * @note   As macro is expected to be used for modifying RTS Hw flow control feature activation, without need
  *         for USART instance Deinit/Init, following conditions for macro call should be fulfilled :
  *           - UART instance should have already been initialised (through call of HAL_UART_Init() )
  *           - macro could only be called when corresponding UART instance is disabled (i.e __HAL_UART_DISABLE(__HANDLE__))
  *             and should be followed by an Enable macro (i.e __HAL_UART_ENABLE(__HANDLE__)). 
  * @param  __HANDLE__: specifies the UART Handle.
  *         This parameter can be any USARTx (supporting the HW Flow control feature).
  *         It is used to select the USART peripheral (USART availability and x value depending on device).
  * @retval None
  */
#define __HAL_UART_HWCONTROL_RTS_DISABLE(__HANDLE__)       \
  do{                                                      \
    CLEAR_BIT((__HANDLE__)->Instance->CR3, USART_CR3_RTSE);\
    (__HANDLE__)->Init.HwFlowCtl &= ~(USART_CR3_RTSE);     \
  } while(0)


/** @brief  Enable UART
  * @param  __HANDLE__: specifies the UART Handle.
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */ 
#define __HAL_UART_ENABLE(__HANDLE__)               ((__HANDLE__)->Instance->CR1 |=  USART_CR1_UE)

/** @brief  Disable UART
  *         UART Handle selects the USARTx or UARTy peripheral 
  *         (USART,UART availability and x,y values depending on device).
  * @retval None
  */
#define __HAL_UART_DISABLE(__HANDLE__)              ((__HANDLE__)->Instance->CR1 &=  ~USART_CR1_UE)

/**
  * @}
  */


/* Private macros --------------------------------------------------------*/
/** @defgroup UART_Private_Macros   UART Private Macros
  * @{
  */

#define UART_CR1_REG_INDEX               1    
#define UART_CR2_REG_INDEX               2    
#define UART_CR3_REG_INDEX               3    

#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)         (((_PCLK_)*25)/(4*(_BAUD_)))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)     (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)     (((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100)) * 16 + 50) / 100)
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)         ((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4)|(UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0F))

#define IS_UART_WORD_LENGTH(LENGTH)       (((LENGTH) == UART_WORDLENGTH_8B) || \
                                           ((LENGTH) == UART_WORDLENGTH_9B))
#define IS_UART_LIN_WORD_LENGTH(LENGTH)   ((LENGTH) == UART_WORDLENGTH_8B)

#define IS_UART_STOPBITS(STOPBITS)     (((STOPBITS) == UART_STOPBITS_1) || \
                                        ((STOPBITS) == UART_STOPBITS_2))

#define IS_UART_PARITY(PARITY)         (((PARITY) == UART_PARITY_NONE) || \
                                        ((PARITY) == UART_PARITY_EVEN) || \
                                        ((PARITY) == UART_PARITY_ODD))

#define IS_UART_HARDWARE_FLOW_CONTROL(CONTROL)\
                                       (((CONTROL) == UART_HWCONTROL_NONE) || \
                                        ((CONTROL) == UART_HWCONTROL_RTS) || \
                                        ((CONTROL) == UART_HWCONTROL_CTS) || \
                                        ((CONTROL) == UART_HWCONTROL_RTS_CTS))

#define IS_UART_MODE(MODE)             ((((MODE) & (~((uint32_t)UART_MODE_TX_RX))) == 0x00) && \
                                        ((MODE) != (uint32_t)0x00000000))

#define IS_UART_STATE(STATE)           (((STATE) == UART_STATE_DISABLE) || \
                                        ((STATE) == UART_STATE_ENABLE))

#define IS_UART_OVERSAMPLING(SAMPLING)      ((SAMPLING) == UART_OVERSAMPLING_16)
#define IS_UART_LIN_OVERSAMPLING(SAMPLING)  ((SAMPLING) == UART_OVERSAMPLING_16)

#define IS_UART_LIN_BREAK_DETECT_LENGTH(LENGTH) (((LENGTH) == UART_LINBREAKDETECTLENGTH_10B) || \
                                                 ((LENGTH) == UART_LINBREAKDETECTLENGTH_11B))

#define IS_UART_WAKEUPMETHOD(WAKEUP)   (((WAKEUP) == UART_WAKEUPMETHOD_IDLELINE) || \
                                        ((WAKEUP) == UART_WAKEUPMETHOD_ADDRESSMARK))

                                
/** Check UART Baud rate
  *         __BAUDRATE__: Baudrate specified by the user
  *         The maximum Baud Rate is derived from the maximum clock on APB (i.e. 72 MHz) 
  *         divided by the smallest oversampling used on the USART (i.e. 16) 
  * Retrun : TRUE or FALSE
  */
#define IS_UART_BAUDRATE(__BAUDRATE__) ((__BAUDRATE__) < 4500001)

/** Check UART Node Address
  *         __ADDRESS__: UART Node address specified by the user
  *         UART Node address is used in Multi processor communication for wakeup 
  *         with address mark detection. 
  *         This parameter must be a number between Min_Data = 0 and Max_Data = 15 
  * Return : TRUE or FALSE
  */
#define IS_UART_ADDRESS(__ADDRESS__) ((__ADDRESS__) <= 0xF)

/** UART interruptions flag mask
  */ 
#define UART_IT_MASK  ((uint32_t) USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE | \
                                  USART_CR1_IDLEIE | USART_CR2_LBDIE | USART_CR3_CTSIE | USART_CR3_EIE )

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup UART_Exported_Functions UART Exported Functions
  * @{
  */
  
/** @addtogroup UART_Exported_Functions_Group1 Initialization and de-initialization functions 
  * @{
  */

/* Initialization and de-initialization functions  ****************************/
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength);
HAL_StatusTypeDef HAL_MultiProcessor_Init(UART_HandleTypeDef *huart, uint8_t Address, uint32_t WakeUpMethod);
HAL_StatusTypeDef HAL_UART_DeInit (UART_HandleTypeDef *huart);
void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group2 IO operation functions 
  * @{
  */

/* IO operation functions *****************************************************/
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_DMAPause(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DMAResume(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart);
void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group3 Peripheral Control functions
  * @{
  */

/* Peripheral Control functions  ************************************************/
HAL_StatusTypeDef HAL_LIN_SendBreak(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_MultiProcessor_EnterMuteMode(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_MultiProcessor_ExitMuteMode(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_EnableTransmitter(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_HalfDuplex_EnableReceiver(UART_HandleTypeDef *huart);

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions_Group4 Peripheral State and Errors functions 
  * @{
  */

/* Peripheral State and Errors functions  **************************************************/
HAL_UART_StateTypeDef HAL_UART_GetState(UART_HandleTypeDef *huart);
uint32_t              HAL_UART_GetError(UART_HandleTypeDef *huart);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_UART_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
