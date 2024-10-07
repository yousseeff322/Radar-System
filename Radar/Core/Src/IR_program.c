#include "STD_type.h"
#include "BIT_math.h"
#include "SYSTICK_interface.h"

#include "IR_interface.h"
#include "IR_private.h"
#include "IR_config.h"

volatile u8   u8StartFlag       = 0;
volatile u32  u32FrameData[50]  = {0};  // Storing time intervals for the IR frame
volatile u8   u8EdgeCounter     = 0;
volatile u8   u8Data            = 0;   // Variable to store the extracted key

void(* IR_CallBack)(u8 Copy_u8Key);    // Function pointer for the callback

void HIR_voidGetFrame(void)
{
    if (u8StartFlag == 0)
    {
        /* Start Timer for first edge */
        MSTK_voidSetIntervalSingle(100000, HIR_voidExtractKey);
        u8StartFlag = 1;
    }
    else
    {
        u32FrameData[u8EdgeCounter] = MSTK_u32GetElapsedTime();
        MSTK_voidSetIntervalSingle(100000, HIR_voidExtractKey);  // Reset timer after each edge
        u8EdgeCounter++;
    }
}

void HIR_voidExtractKey(void)
{
    u8Data = 0;

    /* Check for a valid frame start (leading pulse length between 10ms and 14ms) */
    if ((u32FrameData[0] >= 10000) && (u32FrameData[0] <= 14000) && u8EdgeCounter >= 33)  // NEC has 33 edges
    {
        /* Extract the 8-bit key from the frame */
        for (u8 i = 0; i < 8; i++)
        {
            if ((u32FrameData[17 + i] >= 2000) && (u32FrameData[17 + i] <= 2300))
            {
                SET_BIT(u8Data, i);  // Data bit is '1'
            }
            else if ((u32FrameData[17 + i] >= 1000) && (u32FrameData[17 + i] <= 1300))
            {
                CLR_BIT(u8Data, i);  // Data bit is '0'
            }
        }

        /* Call the callback function if set */
        if (IR_CallBack != NULL)
        {
            IR_CallBack(u8Data);
        }
    }
    else
    {
        /* Invalid Frame or incomplete data */
    }

    /* Reset all variables for the next frame */
    u8StartFlag     = 0;
    u8EdgeCounter   = 0;
    for (u8 i = 0; i < 50; i++) {  // Clear the entire array
        u32FrameData[i] = 0;
    }
}

void HIR_voidSetCallBack(void(*ptr)(u8 Copy_u8Key))
{
    IR_CallBack = ptr;
}

u8 HIR_u8GetPressedKey(void)
{
    u8 tempKey = u8Data;
    u8Data = NO_KEY_PRESSED;  // Clear the key after reading it
    return tempKey;
}

