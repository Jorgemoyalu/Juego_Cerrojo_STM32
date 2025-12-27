/*
 * inputs.h
 *
 * MÓDULO DE ENTRADAS (Drivers Hardware)
 * Adaptado a requisitos de Organización:
 * - Sin control de LEDs (movido a display)
 * - Con lógica de tiempos para botones (Corto vs Largo)
 *
 * Responsable: Moya
 */

#ifndef INC_INPUTS_H_
#define INC_INPUTS_H_

#include "main.h"

// Definición de estados para la lógica de los botones
// Esto permite a la Lógica del Juego saber qué ha pasado exactamente
typedef enum {
    BTN_IDLE = 0,       // No pulsado o sin evento
    BTN_SHORT_CLICK,    // Pulsación corta (< 3 segundos) -> Validar / Reset
    BTN_LONG_CLICK      // Pulsación larga (> 3 segundos) -> Pista / Apagar
} ButtonState_t;

// --- FUNCIONES PÚBLICAS ---

// Inicialización de hardware de entradas
void Inputs_Init(void);

// Lee el valor de un potenciómetro y lo escala de 0 a 9
// channel_index: 0 a 3 (Para los 4 potenciómetros)
uint8_t Inputs_ReadPot(uint8_t channel_index);

// Lógica del Botón VALIDAR (PA0)
// Devuelve: BTN_SHORT_CLICK (Validar) o BTN_LONG_CLICK (Pedir Pista)
ButtonState_t Inputs_ReadValidarBtn(void);

// Lógica del Botón MENU/RESET (PC13)
// Devuelve: BTN_SHORT_CLICK (Reset) o BTN_LONG_CLICK (Apagar)
ButtonState_t Inputs_ReadMenuBtn(void);

#endif /* INC_INPUTS_H_ */
