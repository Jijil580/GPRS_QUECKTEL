/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2021 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/I1C V1.01.07.02 [08 Nov 2021]
* Device(s)    : R5F10NLE
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 11-10-2024
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_tau.h"
#include "r_cg_wdt.h"

#include "r_cg_sau.h"
#include "string.h"
//#include "Quecktel.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
 const char *at_commands[] = {
    "ATI\r\n",
    "ATE0\r\n",
    "AT+QSCLKEX=0\r\n",
    "AT+CEDRXS=0,4\r\n",
    "AT+CEDRXS?\r",
    "AT+QCFG=\"NWSCANMODE\",0\r",
    "AT+QICSGP=1,3,\"tpplus.smwbiot\",\"\",\"\"\r",
    "AT+CPIN?\r",
    "AT+EGMR=0,7\r",
    "AT+CCID\r",
    "AT+CREG?\r",
    "AT+CREG=0\r",
    "AT+CGREG=0\r",
    "AT+COPS?\r",
    "AT+CSQ\r",
    "AT+QICFG=\"passiveclosed\",1\r",
    "AT+QICFG=\"tcp/keepalive\"\r",
    "AT+QIACT=1\r",
    "AT+QIACT?\r",
    "AT+QLTS=2\r",
    "AT+QLTS=2\r",
    "AT+QIOPEN=1,1,\"TCP LISTENER\",\"2401:4900:9831:FD7B::2\",0,8086,1,1\r"
    //"AT+QIOPEN=1,1,\"TCP LISTENER\",\"::1\",0,4059,1,1\r",
   // "AT+QICLOSE=1\r",
    //"AT+QIACT?\r",
    //"AT+QLTS=2\r",
    //"AT+QLTS=2\r",
   // "AT+QIOPEN=1,1,\"TCP LISTENER\",\"2401:4900:9831:FD7B::2\",0,8086,1,1\r"
};




#define TCP_INITIALISED       1
#define TCP_FAILED            0
#define TCP_MODE               1
#define INIT_MODE              0
uint8_t TCP_SUCCESS[]="tcplisten mode\r\n";
uint8_t UART1_RECIEVED_DATA[100];
uint8_t UART1_SEND_DATA[1000];
uint8_t UART0_RECIEVED_DATA[200];
uint8_t UART0_SEND_DATA[100];
uint8_t RECIEVED_DATA_SIZE;
uint8_t MODULE_FEEDBACK[200];
long int TIMER_COUNT;
uint8_t RX0_BUFFER[200];
extern uint8_t DATA_RECIEVED;
uint8_t count=0;
extern uint8_t COMPARE_MATCH1=0;
extern uint8_t TCP_INIT_STATUS;
//uint8_t MODULE_FEEDBACK[1000];
int FEEDBACK_LENGTHG;
extern int  RX0_BUFFER_COUNT;
uint8_t BUF_FLAG=0;
uint8_t TCP_INT_FLAG=0;
uint8_t MODULE_MODE=0;
uint8_t TCP_DATA_BUFFER[4][200];
//uint8_t MAIN_RX_STORE_COUNT;
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void Initialize_Module(void);
void PROCESS_TCP_DATA(void);
static void R_MAIN_UserInit(void);
extern void __delay_ms(unsigned int milliseconds);
uint8_t INIT_MODULE_TO_LISTEN_TCP(void);
uint8_t CHECK_MODULE_RESPONSE(uint8_t *RESPONSE);
void SPLIT_TCP_DATA(uint8_t *BUFFER);
 void __delay_ms(unsigned int milliseconds) {
    volatile unsigned int i, j;
    for (i = 0; i < milliseconds; i++) {
        for (j = 0; j < 1000; j++) {
          //  __asm("nop");  // No operation, just waste some cycles
        }
    }
}

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    R_UART0_Start();
    R_UART1_Start();
    //R_TAU0_Channel0_Start();
    P7&=~(1<<2);
    P7|=(1<<1);
    P7 |=(1<<3);
    __delay_ms(1000);
    __delay_ms(1000);
     P7&=~(1<<3);
    R_UART0_Receive(UART0_RECIEVED_DATA, sizeof(UART0_RECIEVED_DATA));//gprs
    R_UART1_Receive(UART1_RECIEVED_DATA, sizeof(UART1_RECIEVED_DATA));//module mcu
    __delay_ms(500); 
    
    
    
    while (1U)
      {
     	R_WDT_Restart(); 
	
	//if(TCP_INIT_STATUS==0)
	if(MODULE_MODE==INIT_MODE)
	{
		
	   TCP_INIT_STATUS= INIT_MODULE_TO_LISTEN_TCP();
	}
	   
      //  else if(TCP_INIT_STATUS==1)
      if(MODULE_MODE==TCP_MODE)
	{	
	  if(DATA_RECIEVED == 1) // Ensure proper spacing for readability
              {
	     SPLIT_TCP_DATA(&RX0_BUFFER);
	     PROCESS_TCP_DATA();
	      }	
	   TCP_INIT_STATUS=1;
	   //
	   
	}
	//R_UART1_Send(TCP_SUCCESS, sizeof(TCP_SUCCESS));
        ;
	//delay_In_Seconds(1000);
      }
      
      
      
      
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
uint8_t INIT_MODULE_TO_LISTEN_TCP(void) // Fix: Function name formatting (spaces not allowed)
{
    if (DATA_RECIEVED == 1) // Ensure proper spacing for readability
    {
	    
	
        COMPARE_MATCH1=CHECK_MODULE_RESPONSE(RX0_BUFFER);
	if(COMPARE_MATCH1==1)
		{
        	TCP_INT_FLAG++; // Increment `TCP_INT_FLAG`
		COMPARE_MATCH1=0;
		}
        DATA_RECIEVED = 0; // Reset flag
        memset(RX0_BUFFER, 0, 200); // Clear buffer
        RX0_BUFFER_COUNT = 0; // Reset buffer count

        if (TCP_INT_FLAG<=21) // Ensure `TCP_INT_FLAG` is within valid range
       		 {
         	 R_UART0_Send(at_commands[TCP_INT_FLAG], strlen(at_commands[TCP_INT_FLAG])); // Send AT command
		 TCP_INIT_STATUS=0;
		 MODULE_MODE=0;
       		 }
	if(TCP_INT_FLAG>=21)
	{
	  TCP_INIT_STATUS=TCP_INITIALISED ;
	  TCP_INT_FLAG=0;
	  MODULE_MODE=1;
	}
    }
   return TCP_INIT_STATUS;
}
uint8_t CHECK_MODULE_RESPONSE(uint8_t *RESPONSE)
{
	RX0_BUFFER[RX0_BUFFER_COUNT]='\0';
	for (BUF_FLAG = 0;BUF_FLAG < RX0_BUFFER_COUNT;BUF_FLAG++)
        {
            // Check if the current sequence matches "OK\r\n"
            if(RESPONSE[BUF_FLAG] == 'O' && RESPONSE[BUF_FLAG+ 1] == 'K' && RESPONSE[BUF_FLAG+ 2] == '\r' && RESPONSE[BUF_FLAG + 3] == '\n')
            {
                return 1; // Match found, return 1
            }
	}

    return 0; // No match found

}

void PROCESS_TCP_DATA(void)
{
 if (DATA_RECIEVED == 1) // Ensure proper spacing for readability
    {	RX0_BUFFER[RX0_BUFFER_COUNT]='\0';
	DATA_RECIEVED = 0; // Reset flag
        memset(RX0_BUFFER, 0, 200); // Clear buffer
        RX0_BUFFER_COUNT = 0; // Reset buffer count
	memset(TCP_DATA_BUFFER,0,sizeof(TCP_DATA_BUFFER));
    }
}


void SPLIT_TCP_DATA(uint8_t *BUFFER)
{
    int token_index = 0; // Current token index
    int char_index = 0;  // Current character index in a token
    int buffer_i=0;


    for (buffer_i = 0; BUFFER[buffer_i] != '\0';buffer_i++)
    {
        if (BUFFER[buffer_i] ==',') // If the delimiter is found
        {
            TCP_DATA_BUFFER[token_index][char_index] = '\0'; // Null-terminate the current token
            token_index++;                         // Move to the next token
            char_index = 0;                        // Reset character index for the next token

            // Prevent overflow of tokens
            if (token_index >= 3)
                break;
        }
        else
        {
            // Add character to the current token
            TCP_DATA_BUFFER[token_index][char_index++] =BUFFER[buffer_i];

            // Prevent overflow of the current token
            if (char_index >= strlen(RX0_BUFFER) - 1)
            {
                TCP_DATA_BUFFER[token_index][char_index] = '\0'; // Null-terminate the token
                break;
            }
        }
    }

    // Null-terminate the last token
      TCP_DATA_BUFFER[token_index][char_index] = '\0';
}
