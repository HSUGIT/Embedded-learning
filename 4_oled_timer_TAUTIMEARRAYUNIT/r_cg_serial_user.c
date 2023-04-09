/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G13 V2.00.00.07 [22 Feb 2013]
* Device(s)    : R5F100LG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 2023/3/28
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTIIC01 r_iic01_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "oled.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t   g_iic01_master_status_flag;  /* iic01 start flag for send address check by master mode */
extern volatile uint8_t * gp_iic01_tx_address;         /* iic01 send data pointer by master mode */
extern volatile uint16_t  g_iic01_tx_count;            /* iic01 send data size by master mode */
extern volatile uint8_t * gp_iic01_rx_address;         /* iic01 receive data pointer by master mode */
extern volatile uint16_t  g_iic01_rx_count;            /* iic01 receive data size by master mode */
extern volatile uint16_t  g_iic01_rx_length;           /* iic01 receive data length by master mode */
/* Start user code for global. Do not edit comment generated here */
extern uint8_t  end_if;
extern uint8_t  end_rxif;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_iic01_interrupt
* Description  : This function is INTIIC01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_iic01_interrupt(void)
{
    if ((SSR01 & _0002_SAU_PARITY_ERROR) == 0U)
    {
        /* Control for master send */
        if ((g_iic01_master_status_flag & _01_SAU_IIC_SEND_FLAG) == 1U)
        {
            if (g_iic01_tx_count > 0U)
            {
                SIO01 = *gp_iic01_tx_address;
                gp_iic01_tx_address++;
                g_iic01_tx_count--;
            }
            else
            {
                R_IIC01_StopCondition();
                r_iic01_callback_master_sendend();
            }
        }
        /* Control for master receive */
        else 
        {
            if ((g_iic01_master_status_flag & _04_SAU_IIC_SENDED_ADDRESS_FLAG) == 0U)
            {
                ST0 |= _0002_SAU_CH1_STOP_TRG_ON;
                SCR01 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
                SCR01 |= _4000_SAU_RECEPTION;
                SS0 |= _0002_SAU_CH1_START_TRG_ON;
                g_iic01_master_status_flag |= _04_SAU_IIC_SENDED_ADDRESS_FLAG;
                
                if (g_iic01_rx_length == 1U)
                {
                    SOE0 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable IIC01 out */
                }
                
                SIO01 = 0xFFU;
            }
            else
            {
                if (g_iic01_rx_count < g_iic01_rx_length)
                {
                    *gp_iic01_rx_address = SIO01;
                    gp_iic01_rx_address++;
                    g_iic01_rx_count++;
                    
                    if (g_iic01_rx_count == (g_iic01_rx_length - 1U))
                    {
                        SOE0 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable IIC01 out */
                        SIO01 = 0xFFU;
                    }
                    else if (g_iic01_rx_count == g_iic01_rx_length)
                    {
                        R_IIC01_StopCondition();
                        r_iic01_callback_master_receiveend();
                    }
                    else
                    {
                        SIO01 = 0xFFU;
                    }
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iic01_callback_master_receiveend
* Description  : This function is a callback function when IIC01 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic01_callback_master_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    end_rxif = 1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic01_callback_master_sendend
* Description  : This function is a callback function when IIC01 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic01_callback_master_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    end_if = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
