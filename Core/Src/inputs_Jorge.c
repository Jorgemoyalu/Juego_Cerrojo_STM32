/*
 * inputs.c
 * CORREGIDO POR TUDOR
 * - Añadida selección de canal ADC para leer los 4 potenciómetros.
 * - Invertida la lógica de botones para Pull-Up (0 = Pulsado).
 */

#include "inputs_Jorge.h"

// Referencia al ADC definido en main.c
extern ADC_HandleTypeDef hadc1;

static uint32_t validar_start_time = 0;
static uint8_t  validar_is_pressed = 0;

static uint32_t menu_start_time = 0;
static uint8_t  menu_is_pressed = 0;

#define DEBOUNCE_MS     50
#define LONG_PRESS_MS   3000

void Inputs_Init(void) {
}

uint8_t Inputs_ReadPot(uint8_t channel_index) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    sConfig.Rank = 1;

    switch(channel_index) {
        case 0: sConfig.Channel = ADC_CHANNEL_1; break; // PA1
        case 1: sConfig.Channel = ADC_CHANNEL_2; break; // PA2
        case 2: sConfig.Channel = ADC_CHANNEL_3; break; // PA3
        case 3: sConfig.Channel = ADC_CHANNEL_4; break; // PA4
        default: return 0;
    }

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) return 0;

    // Limpieza
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    volatile uint32_t basura = HAL_ADC_GetValue(&hadc1);
    (void)basura;

    // Lectura
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 20) == HAL_OK) {
        uint32_t raw_val = HAL_ADC_GetValue(&hadc1);
        if (raw_val < 400)  return 0;
        if (raw_val < 800)  return 1;
        if (raw_val < 1200) return 2;
        if (raw_val < 1600) return 3;
        if (raw_val < 2000) return 4;
        if (raw_val < 2400) return 5;
        if (raw_val < 2800) return 6;
        if (raw_val < 3200) return 7;
        if (raw_val < 3400) return 8;
        return 9;
    }
    return 0;
}
ButtonState_t Inputs_ReadValidarBtn(void) {
    // Leemos el pin PA0
    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    ButtonState_t evento = BTN_IDLE;
    uint32_t ahora = HAL_GetTick();

    if (estado_actual == GPIO_PIN_RESET) {
        // BOTÓN PRESIONADO
        if (!validar_is_pressed) {
            validar_is_pressed = 1;
            validar_start_time = ahora;
        }
    }
    else {
        // BOTÓN SOLTADO
        if (validar_is_pressed) {
            uint32_t duracion = ahora - validar_start_time;
            validar_is_pressed = 0;

            if (duracion >= LONG_PRESS_MS) {
                evento = BTN_LONG_CLICK;
            } else if (duracion >= DEBOUNCE_MS) {
                evento = BTN_SHORT_CLICK;
            }
        }
    }
    return evento;
}

ButtonState_t Inputs_ReadMenuBtn(void) {
    // Leemos el pin PC13
    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

    ButtonState_t evento = BTN_IDLE;
    uint32_t ahora = HAL_GetTick();

    if (estado_actual == GPIO_PIN_RESET) {
        if (!menu_is_pressed) {
            menu_is_pressed = 1;
            menu_start_time = ahora;
        }
    }
    else {
        if (menu_is_pressed) {
            uint32_t duracion = ahora - menu_start_time;
            menu_is_pressed = 0;

            if (duracion >= LONG_PRESS_MS) {
                evento = BTN_LONG_CLICK;
            } else if (duracion >= DEBOUNCE_MS) {
                evento = BTN_SHORT_CLICK;
            }
        }
    }
    return evento;
}
