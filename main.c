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
void TimerInterrupt(void);

//extern uint8_t intensity_red = 0;
//extern uint8_t intensity_green = 0;
//extern uint8_t intensity_blue = 0;

//Simple LED effect
void patronus(void)
{
    EPWM1_LoadDutyValue(0); //BLUE
    EPWM2_LoadDutyValue(0); //RED
    PWM4_LoadDutyValue(0); //GREEN
    
    uint8_t i;
    
    //start with blue
    for (i=0;i<20;i++)
    {
        EPWM1_LoadDutyValue(i);
        DELAY_milliseconds(100);
    }
    
    //change to white
    for (i=0;i<20;i++)
    {
        EPWM2_LoadDutyValue(i);
        PWM4_LoadDutyValue(i);
        DELAY_milliseconds(100);
    }      
    
    //go to the full white intensity
    for (i=20;i<100;i++)
    {
        EPWM1_LoadDutyValue(i); //BLUE
        EPWM2_LoadDutyValue(i); //RED
        PWM4_LoadDutyValue(i); //GREEN 
        DELAY_milliseconds(10);
    }    
    
    DELAY_milliseconds(3000); //full brightness for 3s
    
    //slowly turn off the LEDs
    for (i=100;i>1;i--)
    {
        EPWM1_LoadDutyValue(i); //BLUE
        EPWM2_LoadDutyValue(i); //RED
        PWM4_LoadDutyValue(i); //GREEN 
        DELAY_milliseconds(10);
    }        
    EPWM1_LoadDutyValue(0); //BLUE
    EPWM2_LoadDutyValue(0); //RED
    PWM4_LoadDutyValue(0); //GREEN    
}

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    //Handle interrupts
    INT_SetInterruptHandler(WakeUpInterrupt); //Set Weakup Interrupt from accelerometer handler   
    IOCBF7_SetInterruptHandler(ButtonInterrupt); //Set Button Interrupt handler
    TMR4_SetInterruptHandler(TimerInterrupt); //Set Timer interrupt handler
    
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

    //initialize accelerometer
    //this still needs to be adjusted, this code doesnt follow the recommended start up sequence
    // read more here: https://kionixfs.azureedge.net/en/document/TN017-Power-On-Procedure.pdf
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1D, 0b10000000); // reset acceletometer
    DELAY_milliseconds(1000); //wait until the reset sequence is finished
    
    uint8_t data;
    data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x0F); // Read accelerometer ID, it suppose to be 0x35
    
    //set registers for what we need to test
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1E, 0b00101000);
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1F, 0b10111111);    
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1B, 0b00000000);    
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1B, 0b00000010);
    I2C1_Write1ByteRegister(KXTJ3_I2C_ADDR, 0x1B, 0b10000010); 
    
    data = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x1B); //read this register to clear interrupts
    
    //Turn OFF all the LEDs
    EPWM1_LoadDutyValue(0);
    EPWM2_LoadDutyValue(0);
    PWM4_LoadDutyValue(0);
    
    //TMR4_StartTimer(); for future use  
    
    int8_t data_x;
    int8_t data_y;
    int8_t data_z;
    
    while (1)
    {
        //test code - turns on different LED color when PCB is oriented different ways
        data_x = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x07);
        data_y = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x09);
        data_z = I2C1_Read1ByteRegister(KXTJ3_I2C_ADDR, 0x0B);
        
        if (data_x > 0)
            EPWM1_LoadDutyValue(data_x);
        else
            EPWM1_LoadDutyValue(data_x * (-1));

        if (data_y > 0)
            EPWM2_LoadDutyValue(data_y);
        else
            EPWM2_LoadDutyValue(data_y * (-1));
        
        if (data_z > 0)
            PWM4_LoadDutyValue(data_z);
        else
            PWM4_LoadDutyValue(data_z * (-1));           
    }
}

//Wakeup Interrupt handler
void WakeUpInterrupt(void){
    //when accelerometer detects a movement, wake up the MCU
}

//Button Interrupt handler
void ButtonInterrupt(void){
    patronus(); //test a LED effect when button is pressed
}

//Timer Interrupt handler
void TimerInterrupt(void)
{
    TMR4_StopTimer(); //after timer reaches 0, stop it
}

/**
 End of File
*/