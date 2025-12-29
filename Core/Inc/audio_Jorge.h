/*
 * audio_Jorge.h
 *
 * MÓDULO DE AUDIO (PWM Buzzer)
 * Responsable: Jorge
 *
 * Funcionalidades:
 * - Generación de tonos PWM.
 * - Efectos de sonido (Tic-Tac, Acierto, Error).
 * - Melodías (Victoria, Derrota).
 */

#ifndef INC_AUDIO_JORGE_H_
#define INC_AUDIO_JORGE_H_

#include "main.h"

// --- NOTAS MUSICALES (Frecuencias en Hz) ---
#define NOTE_C4  261
#define NOTE_D4  294
#define NOTE_E4  329
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  493
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880

// --- FUNCIONES PÚBLICAS ---

// Inicializa el driver de audio con el Timer correspondiente
void Audio_Init(TIM_HandleTypeDef *htim_ptr);

// Reproduce un tono puro (bloqueante o no, depende de la duración)
// frequency: Hz (ej. 440)
// duration_ms: Tiempo en milisegundos
void Audio_PlayTone(uint16_t frequency, uint16_t duration_ms);

// --- EFECTOS PREDEFINIDOS (API para Juego) ---
void Audio_Play_Tic(void);      // Sonido corto "Tic"
void Audio_Play_Tac(void);      // Sonido corto "Tac" (más grave)
void Audio_Play_Validar(void);  // "Ding" (Acierto)
void Audio_Play_Error(void);    // "Meeec" (Error)
void Audio_Play_Victoria(void); // Melodía ganadora
void Audio_Play_GameOver(void); // Melodía perdedora

#endif /* INC_AUDIO_JORGE_H_ */
