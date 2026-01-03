/*
 * juego_Gabriela.h
 * Responsable: Gabriela
 *
 * DESCRIPCIÓN:
 * Cabecera pública del módulo de juego.
 * Define los estados, la estructura de datos y las funciones accesibles.
 */

#ifndef INC_JUEGO_GABRIELA_H_
#define INC_JUEGO_GABRIELA_H_

#include "main.h"    // Necesario para tipos como uint32_t
#include <stdbool.h> // Necesario para bool, true, false

// MÁQUINA DE ESTADOS
typedef enum {
    ESTADO_OFF = 0,          // Apagado / Standby
    ESTADO_INICIO,           // Bienvenida
    ESTADO_LOGIN,            // Meter ID de usuario
    ESTADO_SELECCION_DIF,    // Elegir Fácil/Medio/Difícil
    ESTADO_CUENTA_ATRAS,     // 3, 2, 1...
    ESTADO_JUEGO_ACTIVO,     // ¡Bomba activada!
    ESTADO_FIN_VICTORIA,     // Ganaste
    ESTADO_FIN_GAMEOVER,     // Explotó
    ESTADO_RANKING           // Hall of Fame
} EstadoJuego_t;

// NIVELES DE DIFICULTAD
typedef enum {
    DIFICULTAD_FACIL = 0,
    DIFICULTAD_MEDIO,
    DIFICULTAD_DIFICIL
} Dificultad_t;

// SUPER-STRUCT
typedef struct {
    // Control de Flujo
    EstadoJuego_t estadoActual;
    uint32_t tiempoEnEstado;     // Cronómetro general (para menús, transiciones...)

    // Configuración de Partida
    Dificultad_t dificultad;
    int32_t tiempoRestante_ms;   // Cuenta atrás de la bomba (int32 por si baja de 0)

    // Datos del Juego (Lógica Candado)
    uint8_t codigoSecreto[4];    // La clave a adivinar (ej. {1,5,2,9})
    uint8_t intentoUsuario[4];   // Lo que el usuario tiene puesto ahora
    uint8_t indiceDigitoActual;  // ¿Qué número estamos intentando abrir? (0, 1, 2 o 3)
    bool digitoAcertado[4];      // Array de checks (true = abierto)

    // Datos del Jugador
    uint16_t usuarioID;          // El ID que meten al principio (ej. 1234)

} Juego_Handle_t;

// Hacemos la variable pública para que otros módulos (como el Main) puedan verla si hace falta
extern Juego_Handle_t Juego;

// FUNCIONES PÚBLICAS (API)
// Estas son las que llamarás desde el main.c

void Juego_Init(void);          // Configuración inicial al encender
void Juego_FSM_Update(void);    // La máquina de estados (poner en el while(1))
void Juego_Tick_Timer(void);    // El reloj (poner en la interrupción del Timer)

#endif /* INC_JUEGO_GABRIELA_H_ */
