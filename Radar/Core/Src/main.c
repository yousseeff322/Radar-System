#include "STD_type.h"
#include "BIT_MATH.h"
#include "FunHelp.h"

#include "DIO_interface.h"
#include "USART_interface.h"
#include "RCC_interface.h"
#include "SYSTICK_interface.h"
#include "TIM_interface.h"
#include "TIMER2_interface.h"
#include "TIMER3_interface.h"

#include "TFT_interface.h"
#include "SPI_interface.h"
#include "HULTRA_interface.h"
#include "IR_interface.h"

void displayInfo(u16 angle, u16 distance);
void HandleRemoteKey(u8 Copy_u8Key);

// Global variables to control radar operation
u8 radarRunning = 1;  // 1 for running, 0 for stopped
u16 radarSpeed = 50;  // Delay between sweeps, controlling speed

// Main Function
void main(void) {
    // Initialize peripherals
    MRCC_VidInit();
    MRCC_VidEnablePeripheralClock(RCC_APB2, GPIOA_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB2, GPIOB_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB2, AFIO_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB2, SPI1_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB2, TIM1_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB2, TIM8_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB1, TIM2_RCC);
    MRCC_VidEnablePeripheralClock(RCC_APB1, TIM3_RCC);

    /* Pin Initializations */
    MGPIO_VidSetPinDirection(GPIOA, 5, 0b1001);        /* CLK  */
    MGPIO_VidSetPinDirection(GPIOA, 7, 0b1001);        /* MOSI  */
    MGPIO_VidSetPinDirection(GPIOA, PIN0, INPUT_FLOATING);
    MGPIO_VidSetPinDirection(GPIOA, PIN11, OUT_SPEED_2MHZ_PP);    // RED LED
    MGPIO_VidSetPinDirection(GPIOB, PIN1, OUT_SPEED_2MHZ_PP);    // GREEN LED
    MGPIO_VidSetPinDirection(GPIOA, PIN8, OUT_SPEED_2MHZ_AFPP);   // PWM

    MTIMER3_init(PWM_channel_1_IN);
    MTIMER2_init(delay_us);
    HULTRA_attachPin(GPIOA, PIN9, MTIMER3_CH1_PORTA_6);

    /* Init STK */
    MSTK_voidInit();

    /* SPI Init */
    MSPI1_voidInit();

    /* TFT Init */
    HTFT_voidInit();
    HTFT_voidFillDisplay(TFT_BLACK);

    /* PWM Configuration */
    sPWM_Confg_t PWM_Confg;
    PWM_Confg.TimerSelect = PWM_TIMER1;
    PWM_Confg.AutoReloadReg = 20000 - 1;
    PWM_Confg.Prescaler = 7;
    PWM_Confg.CCR1_Status = CCR_ENABLE;
    PWM_Confg.CCR2_Status = CCR_DISABLE;
    PWM_Confg.CCR3_Status = CCR_DISABLE;
    PWM_Confg.CCR4_Status = CCR_DISABLE;
    PWM_voidInit(&PWM_Confg);

    u16 distance = 0;
    u16 angle = 0;

    u8 remoteKey = 0;

    while (1) {
        // Check for remote key input
        remoteKey = HIR_u8GetPressedKey();
        if (remoteKey != NO_KEY_PRESSED) {
            HandleRemoteKey(remoteKey);
        }

        // If radar is running, perform the sweep
        if (radarRunning) {
            // Sweep from left to right (0 to 180 degrees)
            for (angle = 0; angle <= 180; angle += 2) {
                // Update PWM to move servo faster (fewer delays)
                PWM_voidUpdate(PWM_TIMER1, Channel_1, map(angle, 0, 180, 5, 15));

                // Trigger ultrasonic sensor and get distance
                HULTRA_Trig(GPIOA, PIN9);
                distance = HULTRA_Distance(MTIMER3_CH1_PORTA_6);

                // Control LEDs based on distance (< 5 cm for Red LED, >= 5 cm for Green LED)
                if (distance < 5) {
                    MGPIO_VidSetPinValue(GPIOA, PIN11, HIGH); // Red LED ON
                    MGPIO_VidSetPinValue(GPIOB, PIN1, LOW);  // Green LED OFF
                } else {
                    MGPIO_VidSetPinValue(GPIOA, PIN11, LOW);  // Red LED OFF
                    MGPIO_VidSetPinValue(GPIOB, PIN1, HIGH); // Green LED ON
                }

                // Display angle and distance information on TFT
                displayInfo(angle, distance);

                // Adjust delay for speed control
                MTIMER3_delay_ms(radarSpeed);
            }

            // Sweep from right to left (180 to 0 degrees)
            for (angle = 180; angle > 0; angle -= 2) {
                // Update PWM to move servo faster
                PWM_voidUpdate(PWM_TIMER1, Channel_1, map(angle, 0, 180, 5, 15));

                // Trigger ultrasonic sensor and get distance
                HULTRA_Trig(GPIOA, PIN9);
                distance = HULTRA_Distance(MTIMER3_CH1_PORTA_6);

                // Control LEDs based on distance (< 5 cm for Red LED, >= 5 cm for Green LED)
                if (distance < 5) {
                    MGPIO_VidSetPinValue(GPIOA, PIN11, HIGH); // Red LED ON
                    MGPIO_VidSetPinValue(GPIOB, PIN1, LOW);  // Green LED OFF
                } else {
                    MGPIO_VidSetPinValue(GPIOA, PIN11, LOW);  // Red LED OFF
                    MGPIO_VidSetPinValue(GPIOB, PIN1, HIGH); // Green LED ON
                }

                // Display angle and distance information on TFT
                displayInfo(angle, distance);

                // Adjust delay for speed control
                MTIMER3_delay_ms(radarSpeed);
            }
        }
    }
}

// Function to handle remote key inputs
void HandleRemoteKey(u8 Copy_u8Key) {
    switch (Copy_u8Key) {
        case STOP:
            // Toggle radar running state (start/stop)
            radarRunning = !radarRunning;
            break;

        case MODE:
            // Increase speed (reduce delay)
            if (radarSpeed > 10) { // Minimum speed limit
                radarSpeed -= 10;
            }
            break;

        case MUTE:
            // Decrease speed (increase delay)
            if (radarSpeed < 200) { // Maximum speed limit
                radarSpeed += 10;
            }
            break;

        default:
            // Handle undefined keys
            break;
    }
}


void displayInfo(u16 angle, u16 distance) {
    // Define area coordinates for angle and distance display
    u16 x_start = 10;      // Starting x-coordinate
    u16 y_angle = 5;       // Y-coordinate for angle
    u16 y_distance = 20;   // Y-coordinate for distance

    // Display static text for "Angle:" and "Distance:" (This remains unchanged)
    HTFT_voidPrintText((s8*)"Angle:", x_start, y_angle, 1, TFT_WHITE, TFT_BLACK);
    HTFT_voidPrintText((s8*)"Distance:", x_start, y_distance, 1, TFT_WHITE, TFT_BLACK);

    // Static variables to hold the last displayed values
    static u16 lastAngle = 0;
    static u16 lastDistance = 0;

    // Check if the angle has changed
    if (angle != lastAngle) {
        // Clear the previous numeric value of angle
        HTFT_voidDrawRectangle(x_start + 50, y_angle, x_start + 100, y_angle + 10, TFT_BLACK);  // Clear only the number space for angle
        // Display updated angle value
        HTFT_voidprintNumber(angle, x_start + 50, y_angle, 1, TFT_WHITE, TFT_BLACK);  // Update only the angle value
        lastAngle = angle;  // Update the last displayed angle
    }

    // Check if the distance has changed
    if (distance != lastDistance) {
        // Clear the previous numeric value of distance
        HTFT_voidDrawRectangle(x_start + 70, y_distance, x_start + 120, y_distance + 10, TFT_BLACK);  // Clear only the number space for distance
        // Display updated distance value
        HTFT_voidprintNumber(distance, x_start + 70, y_distance, 1, TFT_WHITE, TFT_BLACK);  // Update only the distance value
        lastDistance = distance;  // Update the last displayed distance
    }
}
