/*
 * ranking_Tudor.h
 *
 *  Created on: Dec 26, 2025
 *      Author: gfernandez
 */

#ifndef INC_RANKING_TUDOR_H_
#define INC_RANKING_TUDOR_H_

#include "main.h"

#define MAX_JUGADORES 10


typedef struct {
    uint16_t id_usuario;  // ID del 0000 al 9999
    uint32_t puntuacion;  // Puntos conseguidos
} Jugador;

extern Jugador Top10[MAX_JUGADORES];

void Ranking_Borrar_Todo(void);
void Ranking_Init(void);
void Ranking_Actualizar(uint16_t id, uint32_t puntos);

#endif /* INC_RANKING_TUDOR_H_ */
