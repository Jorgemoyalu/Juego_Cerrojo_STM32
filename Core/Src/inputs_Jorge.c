#include "inputs.h"

static ADC_HandleTypeDef *hadc_local; // Variable privada

void Inputs_Init(ADC_HandleTypeDef *hadc_instance) {
    hadc_local = hadc_instance;
}

uint8_t Inputs_ReadPot(uint8_t pot_index) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;

    // Seleccionamos qué canal leer
    switch(pot_index) {
        case 1: sConfig.Channel = ADC_CHANNEL_1; break;
        case 2: sConfig.Channel = ADC_CHANNEL_2; break;
        case 3: sConfig.Channel = ADC_CHANNEL_3; break;
        case 4: sConfig.Channel = ADC_CHANNEL_4; break;
        default: return 0;
    }

    if (HAL_ADC_ConfigChannel(hadc_local, &sConfig) != HAL_OK) return 0;

    HAL_ADC_Start(hadc_local);
    if (HAL_ADC_PollForConversion(hadc_local, 10) == HAL_OK) {
        uint32_t val = HAL_ADC_GetValue(hadc_local);
        HAL_ADC_Stop(hadc_local);
        // Truco matemático: 4096 / 10 = ~410 por número
        uint8_t digit = val / 410;
        return (digit > 9) ? 9 : digit;
    }
    return 0;
}

uint8_t Inputs_ReadValidarBtn(void) {
    // Leemos el botón PA0 (Botón Azul User o el externo que hayas puesto)
    // Usamos GPIOA y GPIO_PIN_0 directamente para evitar errores de etiquetas
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
        HAL_Delay(20); // Anti-rebote
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) {
            while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET); // Esperar a soltar
            return 1;
        }
    }
    return 0;
}

void Inputs_LedControl(uint8_t led_color, uint8_t state) {
    GPIO_PinState pState = (state == LED_ON) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    // Usamos el Puerto D (GPIOD) que es donde están los LEDs en la F411 Discovery
    switch(led_color) {
        case COLOR_VERDE:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, pState); // Pin 12 = Verde
            break;
        case COLOR_AMARILLO:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, pState); // Pin 13 = Naranja/Amarillo
            break;
        case COLOR_ROJO:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, pState); // Pin 14 = Rojo
            break;
    }
}
