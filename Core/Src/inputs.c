/*
 * inputs.c
 * Implementación de drivers de entrada con lógica de tiempos.
 */

#include "inputs.h"

// Referencia al ADC definido en main.c (manejado por el IDE)
extern ADC_HandleTypeDef hadc1;

// --- VARIABLES PRIVADAS PARA LA LÓGICA DE TIEMPOS ---
static uint32_t validar_start_time = 0;
static uint8_t  validar_is_pressed = 0;

static uint32_t menu_start_time = 0;
static uint8_t  menu_is_pressed = 0;

// Umbrales de tiempo (Según chat: 3s para largo)
#define DEBOUNCE_MS     50    // Filtro anti-rebote
#define LONG_PRESS_MS   3000  // 3000 ms = 3 segundos

void Inputs_Init(void) {
    // Si necesitas iniciar algo específico, va aquí.
    // El HAL_ADC_Start se suele llamar en el main, pero por seguridad:
    // HAL_ADC_Start(&hadc1);
}

uint8_t Inputs_ReadPot(uint8_t channel_index) {
    // NOTA: Asumimos que el ADC está configurado.
    // Leemos el valor y lo convertimos a 0-9

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint32_t raw_val = HAL_ADC_GetValue(&hadc1);

        // Escalado: 12 bits (0-4095) -> (0-9)
        // Fórmula: (Valor * 9) / 4095
        return (uint8_t)((raw_val * 9) / 4095);
    }
    return 0; // Valor por defecto si falla
}

ButtonState_t Inputs_ReadValidarBtn(void) {
    // Leemos el pin PA0 (Asumiendo Pull-Down: 1 = Pulsado)
    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    ButtonState_t evento = BTN_IDLE;
    uint32_t ahora = HAL_GetTick();

    if (estado_actual == GPIO_PIN_SET) {
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
            validar_is_pressed = 0; // Reset flag

            // Lógica de tiempos
            if (duracion >= LONG_PRESS_MS) {
                evento = BTN_LONG_CLICK; // > 3 seg (Pista)
            } else if (duracion >= DEBOUNCE_MS) {
                evento = BTN_SHORT_CLICK; // Click normal (Validar)
            }
        }
    }
    return evento;
}

ButtonState_t Inputs_ReadMenuBtn(void) {
    // Leemos el pin PC13 (Botón Menú/Reset)
    GPIO_PinState estado_actual = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

    ButtonState_t evento = BTN_IDLE;
    uint32_t ahora = HAL_GetTick();

    if (estado_actual == GPIO_PIN_SET) {
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
                evento = BTN_LONG_CLICK; // Apagar
            } else if (duracion >= DEBOUNCE_MS) {
                evento = BTN_SHORT_CLICK; // Reset
            }
        }
    }
    return evento;
}
