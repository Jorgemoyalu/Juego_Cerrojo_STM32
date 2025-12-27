#include "audio_Jorge.h"

static TIM_HandleTypeDef *htim_audio;

void Audio_Init(TIM_HandleTypeDef *htim_instance) {
    htim_audio = htim_instance;
}

void Audio_PlayTone(uint16_t frequency, uint16_t duration_ms) {
    if (frequency == 0) return;

    // Fórmula: ARR = (1MHz / Freq) - 1
    // (Asumiendo que configuraste el Prescaler para tener 1MHz)
    uint32_t arr_value = (1000000 / frequency) - 1;

    __HAL_TIM_SET_AUTORELOAD(htim_audio, arr_value);
    __HAL_TIM_SET_COMPARE(htim_audio, TIM_CHANNEL_1, arr_value / 2); // 50% Duty Cycle

    HAL_TIM_PWM_Start(htim_audio, TIM_CHANNEL_1);
    HAL_Delay(duration_ms);
    HAL_TIM_PWM_Stop(htim_audio, TIM_CHANNEL_1);
}
