#ifndef INC_GAME_LOGIC_H_
#define INC_GAME_LOGIC_H_

#include "main.h"
#include "inputs_Jorge.h"   // Necesita leer tus botones
#include "display_Tudor.h"  // Necesita pintar en la pantalla

/*
 * MÓDULO LÓGICA DEL JUEGO (FSM)
 * Responsable: GABRI
 *
 * Funcionalidades:
 * - Máquina de Estados (Standby, Login, Jugar, Ranking...).
 * - Control de tiempos de partida.
 */

typedef enum {
    STATE_STANDBY,
    STATE_LOGIN,
    STATE_DIFFICULTY,
    STATE_PLAYING,
    STATE_VICTORY,
    STATE_GAMEOVER,
    STATE_RANKING
} GameState_t;

// Función principal a llamar en el bucle del main
void Game_Update(void);

#endif
