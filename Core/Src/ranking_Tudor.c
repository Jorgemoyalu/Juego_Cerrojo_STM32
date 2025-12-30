/*
 * ranking_Tudor.c
 * Author: Tudor
 * DESCRIPCIÓN:
 * Implementación de la base de datos del Top 10
 */
#include "ranking_Tudor.h"
#include <string.h>

Jugador Top10[MAX_JUGADORES];

// Borra todos los records y deja la tabla limpia
void Ranking_Borrar_Todo(void) {
    for (int i = 0; i < MAX_JUGADORES; i++) {
        Top10[i].id_usuario = 0;
        Top10[i].puntuacion = 99999; // Volver a valor infinito
    }
}

// Ponemos la tabla a cero al arrancar
void Ranking_Init(void) {
    for (int i = 0; i < MAX_JUGADORES; i++) {
        Top10[i].id_usuario = 0;
        // Ponemos un valor muy alto (99999) para que el primer jugador que juegue
        // tenga mejor tiempo seguro y entre en la lista.
        Top10[i].puntuacion = 99999;
    }
}

//Comprueba si el tiempo nuevo entra en el TOP 10
void Ranking_Actualizar(uint16_t id, uint32_t puntos) {

    if (puntos < Top10[MAX_JUGADORES - 1].puntuacion) {

        // Borramos al ultimo y ponemos el nuevo en su sitio
        Top10[MAX_JUGADORES - 1].id_usuario = id;
        Top10[MAX_JUGADORES - 1].puntuacion = puntos;

        // Ahora hay que ordenar la lista para que los mejores suban arriba.
        for (int i = 0; i < MAX_JUGADORES - 1; i++) {
            for (int j = 0; j < MAX_JUGADORES - i - 1; j++) {

                if (Top10[j].puntuacion > Top10[j + 1].puntuacion) {
                    Jugador temp = Top10[j];
                    Top10[j] = Top10[j + 1];
                    Top10[j + 1] = temp;
                }
            }
        }
    }
}
