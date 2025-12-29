/*
 * audio_Jorge.c
 * Implementación del driver de Buzzer pasivo usando PWM.
 */

#include "audio_Jorge.h"

// Puntero privado al Timer que controla el PWM
static TIM_HandleTypeDef *audio_htim = NULL;
// Canal del PWM (Ajustar si no es CHANNEL_1, por defecto solemos usar el 1)
#define AUDIO_CHANNEL TIM_CHANNEL_1

void Audio_Init(TIM_HandleTypeDef *htim_ptr) {
    audio_htim = htim_ptr;
    // Nos aseguramos de que el PWM empiece apagado
    __HAL_TIM_SET_COMPARE(audio_htim, AUDIO_CHANNEL, 0);
    HAL_TIM_PWM_Start(audio_htim, AUDIO_CHANNEL);
}

void Audio_PlayTone(uint16_t frequency, uint16_t duration_ms) {
    if (audio_htim == NULL || frequency == 0) {
        // Si no está iniciado o frec es 0, paramos sonido
        __HAL_TIM_SET_COMPARE(audio_htim, AUDIO_CHANNEL, 0);
        return;
    }

    // 1. Calcular el Auto-Reload Register (ARR) para la frecuencia deseada
    // Fórmula: Frecuencia_PWM = Reloj_Timer / (Prescaler * ARR)
    // Asumimos que el reloj base del timer tras prescaler es 1 MHz (1 us por tick)
    // Si tu reloj es diferente, esto sonará más agudo o grave, pero funcionará.
    uint32_t arr_value = (1000000 / frequency) - 1;

    // 2. Configurar Frecuencia
    __HAL_TIM_SET_AUTORELOAD(audio_htim, arr_value);

    // 3. Configurar Duty Cycle al 50% (Volumen máximo estable)
    __HAL_TIM_SET_COMPARE(audio_htim, AUDIO_CHANNEL, arr_value / 2);

    // 4. Esperar la duración (Bloqueante simple para efectos cortos)
    HAL_Delay(duration_ms);

    // 5. Apagar sonido
    __HAL_TIM_SET_COMPARE(audio_htim, AUDIO_CHANNEL, 0);
}

// --- EFECTOS DE SONIDO ---

void Audio_Play_Tic(void) {
    Audio_PlayTone(NOTE_E5, 50); // Agudo muy corto
}

void Audio_Play_Tac(void) {
    Audio_PlayTone(NOTE_C5, 50); // Un poco más grave
}

void Audio_Play_Validar(void) {
    // Doble tono ascendente "Ding-Ding"
    Audio_PlayTone(NOTE_A5, 100);
    HAL_Delay(50);
    Audio_PlayTone(NOTE_C5, 200);
}

void Audio_Play_Error(void) {
    // Tono grave y largo "Meeeeec"
    Audio_PlayTone(NOTE_G4, 150);
    Audio_PlayTone(NOTE_F4, 400);
}

void Audio_Play_Victoria(void) {
    // Fanfarria rápida: Do - Mi - Sol - Do(alto)
    Audio_PlayTone(NOTE_C4, 150);
    Audio_PlayTone(NOTE_E4, 150);
    Audio_PlayTone(NOTE_G4, 150);
    HAL_Delay(50);
    Audio_PlayTone(NOTE_C5, 400);
}

void Audio_Play_GameOver(void) {
    // Tristeza descendente
    Audio_PlayTone(NOTE_G4, 300);
    Audio_PlayTone(NOTE_E4, 300);
    Audio_PlayTone(NOTE_C4, 600);
}
