#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "main.h"

/*
 * MÓDULO DISPLAY & FEEDBACK
 * Responsable: TUDOR
 */

 // 1. Definición de estados para el semáforo (LEDs)
typedef enum {
    LED_APAGADO = 0,
    LED_FRIO_ROJO,
    LED_TEMPLADO_AMARILLO,
    LED_CALIENTE_VERDE,
    LED_VICTORIA // Enciende todos a la vez
} EstadoSemaforo;

// 2. Estructura para guardar los valores de los 4 potenciómetros
typedef struct {
    uint8_t digito[4]; // Guardaremos valores del 0 al 9
} EntradasUsuario;

// --- FUNCIONES PÚBLICAS (PROTOTIPOS) ---
void Display_Init(void);
void Leer_Potenciometros(ADC_HandleTypeDef *hadc, EntradasUsuario *misInputs);
void Actualizar_Semaforo(EstadoSemaforo estado);
void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto);
void Display_LCD_Limpiar(void);
#endif
