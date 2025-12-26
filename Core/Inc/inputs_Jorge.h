#ifndef INPUTS_H
#define INPUTS_H

#include "main.h" // Importante para ver el HAL

// --- COLORES LED ---
#define LED_OFF 0
#define LED_ON  1
#define COLOR_ROJO 1
#define COLOR_AMARILLO 2
#define COLOR_VERDE 3

// --- FUNCIONES ---
void Inputs_Init(ADC_HandleTypeDef *hadc_instance);
uint8_t Inputs_ReadPot(uint8_t pot_index); // Devuelve 0-9
uint8_t Inputs_ReadValidarBtn(void);
void Inputs_LedControl(uint8_t led_color, uint8_t state);

#endif
