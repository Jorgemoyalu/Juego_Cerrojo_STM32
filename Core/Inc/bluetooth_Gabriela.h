/*
 * bluetooth_Gabriela.h
 * Responsable: Gabriela
 *
 * DESCRIPCIÓN:
 * Cabecera para el módulo de comunicaciones Bluetooth.
 */

#ifndef INC_BLUETOOTH_GABRIELA_H_
#define INC_BLUETOOTH_GABRIELA_H_

#include "main.h"            // Para tipos de datos básicos
#include "juego_Gabriela.h"  // Para el tipo "Dificultad_t"

// FUNCIÓN PÚBLICA
// Envía por UART la pista del número que te tiene atascada
void Bluetooth_EnviarPista(uint8_t *codigoSecreto, Dificultad_t dificultad);

#endif /* INC_BLUETOOTH_GABRIELA_H_ */
