/*
 * ranking_Tudor.c
 * Author: Tudor
 * DESCRIPCIÓN:
 * Implementación de la base de datos del Top 10
 */
#include "ranking_Tudor.h"
#include <string.h>

// Matriz de 3 dificultades x 10 jugadores
Jugador Top10[NUM_DIFICULTADES][MAX_JUGADORES];

// Borra todos los records y deja la tabla limpia
void Ranking_Borrar_Todo(void) {
	Ranking_Init();
}


// Ponemos la tabla a cero al arrancar
void Ranking_Init(void) {
	// Limpiamos las 3 listas
	    for(int d=0; d<NUM_DIFICULTADES; d++) {
	        for (int i = 0; i < MAX_JUGADORES; i++) {
	            Top10[d][i].id_usuario = 0;
	            Top10[d][i].puntuacion = 99999;
	        }
	    }
	}

void Ranking_Actualizar(uint16_t id, uint32_t puntos, uint8_t dificultad) {

    // Seguridad: si la dificultad viene mal, no hacemos nada
    if(dificultad >= NUM_DIFICULTADES) return;

    // Solo miramos la lista de ESA dificultad
    if (puntos < Top10[dificultad][MAX_JUGADORES - 1].puntuacion) {

        // Insertamos al final
        Top10[dificultad][MAX_JUGADORES - 1].id_usuario = id;
        Top10[dificultad][MAX_JUGADORES - 1].puntuacion = puntos;

        // Ordenamos lista
        for (int i = 0; i < MAX_JUGADORES - 1; i++) {
            for (int j = 0; j < MAX_JUGADORES - i - 1; j++) {
                if (Top10[dificultad][j].puntuacion > Top10[dificultad][j + 1].puntuacion) {
                    Jugador temp = Top10[dificultad][j];
                    Top10[dificultad][j] = Top10[dificultad][j + 1];
                    Top10[dificultad][j + 1] = temp;
                }
            }
        }
    }
}
