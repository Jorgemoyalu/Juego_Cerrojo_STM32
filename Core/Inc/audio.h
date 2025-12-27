#ifndef AUDIO_H
#define AUDIO_H

#include "main.h"

// --- NOTAS MUSICALES ---
#define NOTE_C4 261
#define NOTE_E4 329
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_WIN 1500
#define NOTE_LOSE 100

// --- FUNCIONES ---
void Audio_Init(TIM_HandleTypeDef *htim_instance);
void Audio_PlayTone(uint16_t frequency, uint16_t duration_ms);

#endif
