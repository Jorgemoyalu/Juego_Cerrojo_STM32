/*
 * bluetooth_Gabriela.c
 * Responsable: Gabriela
 *
 * DESCRIPCIÓN:
 * Envío de pistas por UART al módulo Bluetooth.
 */

#include "bluetooth_Gabriela.h"
#include <stdio.h>  // Para sprintf (montar el texto)
#include <string.h> // Para strlen (medir el texto)

#include "main.h"

// Traemos la variable global 'Juego' para saber en qué nivel estás
extern Juego_Handle_t Juego;

extern UART_HandleTypeDef huart1;

void Bluetooth_EnviarPista(uint8_t *codigoSecreto, Dificultad_t dificultad) {

    char buffer_tx[64]; // Buffer para escribir el mensaje

    // Miramos qué candado estás intentando abrir ahora mismo (0, 1, 2 o 3)
    uint8_t indice = Juego.indiceDigitoActual;

    // Sacamos la solución secreta de ESE candado específico
    uint8_t valorSecreto = codigoSecreto[indice];

    // Montamos el mensaje
    // "CANDADO %d" -> Ponemos (indice + 1) para que salga "CANDADO 1" y no "CANDADO 0"
    sprintf(buffer_tx, ">> AYUDA CANDADO %d: LA SOLUCION ES %d\r\n", indice + 1, valorSecreto);

    // ENVIAR
    // Mandamos el mensaje por el cable UART 1 (que es donde tenéis el Bluetooth)
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer_tx, strlen(buffer_tx), 100);
}
