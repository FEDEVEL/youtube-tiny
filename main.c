/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC16LF1829
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"

#define KXTJ3_I2C_ADDR 0x0E


void WakeUpInterrupt(void);
void ButtonInterrupt(void);

extern uint8_t intensity_red = 0;

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    //Set Weakup Interrupt
    //EXT_INT_Initialize();
    INT_SetInterruptHandler(WakeUpInterrupt);     //Register the interrupt Handler    
    //EXT_INT_fallingEdgeSet(); //set the edge
    //EXT_INT_InterruptFlagClear(); //clear the flag
    //EXT_INT_InterruptEnable(); //enable interrupt 
    
    //Set Button Interrupt
    //PIN_MANAGER_Initialize();
    IOCBF7_SetInterruptHandler(ButtonInterrupt);
    
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1D, 0b10000000); //reset
    DELAY_milliseconds(1000);
    
    uint8_t data;
    data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x0F);
    data = data + 1;
    
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1E, 0b00101000);
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1F, 0b10111111);    
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1B, 0b01000000);    
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1B, 0b01000010);
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1B, 0b11000010); 
    
    data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x1B);
    data = data + 1;
    
    
    EPWM1_LoadDutyValue(0);
    EPWM2_LoadDutyValue(0);
    PWM4_LoadDutyValue(0);
        
    while (1)
    {
        // Add your application code

        data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x16);
        data = data + 1;
        data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x17);          
        data = data + 1;
        data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x18);
        data = data + 1;
        data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x1A); //clear interrupt
        data = data + 1;        
    
        //BLUE
        EPWM1_LoadDutyValue(0);
        DELAY_milliseconds(1000);         
        EPWM1_LoadDutyValue(1);
        DELAY_milliseconds(1000);        
        EPWM1_LoadDutyValue(10);
        DELAY_milliseconds(1000);  
        EPWM1_LoadDutyValue(50);
        DELAY_milliseconds(1000);        
        EPWM1_LoadDutyValue(100);
        DELAY_milliseconds(1000);  
        EPWM1_LoadDutyValue(0);        
        
        //RED LED
        /*
        EPWM2_LoadDutyValue(0);
        DELAY_milliseconds(1000);         
        EPWM2_LoadDutyValue(1);
        DELAY_milliseconds(1000);        
        EPWM2_LoadDutyValue(10);
        DELAY_milliseconds(1000);  
        EPWM2_LoadDutyValue(50);
        DELAY_milliseconds(1000);        
        EPWM2_LoadDutyValue(100);
        DELAY_milliseconds(1000);
        EPWM2_LoadDutyValue(0); 
         */
        
        //GREEN
        PWM4_LoadDutyValue(0);
        DELAY_milliseconds(1000);         
        PWM4_LoadDutyValue(1);
        DELAY_milliseconds(1000);        
        PWM4_LoadDutyValue(10);
        DELAY_milliseconds(1000);  
        PWM4_LoadDutyValue(50);
        DELAY_milliseconds(1000);        
        PWM4_LoadDutyValue(100);
        DELAY_milliseconds(1000); 
        PWM4_LoadDutyValue(0);        
        
        EPWM1_LoadDutyValue(0);
        //EPWM2_LoadDutyValue(0);
        PWM4_LoadDutyValue(0);
        DELAY_milliseconds(1000);  
        EPWM1_LoadDutyValue(1);
        //EPWM2_LoadDutyValue(1);        
        PWM4_LoadDutyValue(1);
        DELAY_milliseconds(1000);   
        EPWM1_LoadDutyValue(10);
        //EPWM2_LoadDutyValue(10);        
        PWM4_LoadDutyValue(10);
        DELAY_milliseconds(1000); 
        EPWM1_LoadDutyValue(50);
        //EPWM2_LoadDutyValue(50);        
        PWM4_LoadDutyValue(50);
        DELAY_milliseconds(1000);
        EPWM1_LoadDutyValue(100);
        //EPWM2_LoadDutyValue(100);        
        PWM4_LoadDutyValue(100);
        DELAY_milliseconds(1000); 
        EPWM1_LoadDutyValue(0);
        //EPWM2_LoadDutyValue(0);         
        PWM4_LoadDutyValue(0);        
    }
}

//Wakeup Interrupt handler
void WakeUpInterrupt(void){
    EPWM2_LoadDutyValue(intensity_red + 10);
}

//Button Interrupt handler
void ButtonInterrupt(void){
    EPWM2_LoadDutyValue(intensity_red + 10);
}
/**
 End of File
*/