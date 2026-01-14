/*
 * juego_Gabriela.c
 * Responsable: Gabriela
 *
 * DESCRIPCIÓN:
 * Cerebro del sistema.
 */

#include "juego_Gabriela.h"
#include "inputs_Jorge.h"
#include "audio_Jorge.h"
#include "display_Tudor.h"
#include "ranking_Tudor.h"
#include "bluetooth_Gabriela.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
extern volatile int tiempo_restante;
extern volatile uint8_t flag_sonar_tic;

// VARIABLES GLOBALES
Juego_Handle_t Juego;
char lcd_buffer[32]; // Buffer extendido para que no haya problemas



// Genera un código secreto aleatorio (0-9)
static void GenerarSecreto(void) {
    srand(HAL_GetTick()); //usa el tiempo transcurrido para mezclar diferente cada vez
    for(int i=0; i<4; i++) {
        Juego.codigoSecreto[i] = rand() % 10; // Número aleatorio del 0 al 9
        Juego.digitoAcertado[i] = false; // Reset
    }
}

// Lógica de "Ladrón de Caja Fuerte" (Proximidad)
static void ActualizarFeedbackVisual(void) {

    // Miramos SOLO el potenciómetro que toca
    int idx = Juego.indiceDigitoActual;
    uint8_t valorPuesto = Inputs_ReadPot(idx); // Leemos el pote actual
    uint8_t valorSecreto = Juego.codigoSecreto[idx];

    // Calculamos la distancia (Valor Absoluto)
    int distancia = abs(valorPuesto - valorSecreto);

    // Decidimos color según DIFICULTAD
    if (distancia == 0) {
        // LO TIENES!
        if (Juego.dificultad == DIFICULTAD_DIFICIL) {
            // En difícil NO hay verde, dejamos en amarillo
            Actualizar_Semaforo(LED_TEMPLADO_AMARILLO);
        } else {
            // En Fácil/Medio sí damos el gusto visual
            Actualizar_Semaforo(LED_CALIENTE_VERDE);
        }
    }
    else if (distancia <= 2) {
        // CALIENTE (Cerca, a +/- 1 o 2) --- Ej: Si es 5 y pones 3, 4, 6 o 7 -> Amarillo
        Actualizar_Semaforo(LED_TEMPLADO_AMARILLO);
    }
    else {
        // FRÍO (Lejos)
        Actualizar_Semaforo(LED_FRIO_ROJO);
    }
}

// FUNCIONES PÚBLICAS

void Juego_Init(void) {

    // Inicializamos valores por defecto
    Juego.estadoActual = ESTADO_INICIO;
    Juego.dificultad = DIFICULTAD_FACIL;
    Juego.tiempoEnEstado = 0;
    Juego.usuarioID = 0000;
    Juego.indiceDigitoActual = 0;

    // Inicializamos los hardwares
    // Seguridad
    Display_LCD_Limpiar();
    Actualizar_Semaforo(LED_APAGADO);
}

void Juego_Tick_Timer(void) {

	// ¿Estamos jugando?
    if (Juego.estadoActual == ESTADO_JUEGO_ACTIVO) {
    	// Cuenta Atrás, llama al TIM4 cada 10ms
    	if (Juego.tiempoRestante_ms > 0) {
            Juego.tiempoRestante_ms -= 10;
        } else {
            Juego.tiempoRestante_ms = 0; // Fin del tiempo
        }
    }

    Juego.tiempoEnEstado += 10; // Cronómetro general
}

// FUNCIÓN PRINCIPAL
void Juego_FSM_Update(void) {

    // LEER BOTONES
    ButtonState_t btnValidar = Inputs_ReadValidarBtn(); // CLICK CORTO: ok
    													// CLICK LARGO (> 3s): SOS
    ButtonState_t btnMenu = Inputs_ReadMenuBtn(); 		// CLICK CORTO: reset
														// CLICK LARGO (> 3s): standby

    // RESET DEL JUEGO PARA EL USUARIO
    if (btnMenu == BTN_SHORT_CLICK) {
    	tiempo_restante = 0;
        Juego.estadoActual = ESTADO_INICIO; // Reset rápido
        Audio_Play_Tic();
        return; // Seguridad
    }
    // APAGÓN
    if (btnMenu == BTN_LONG_CLICK) {
    	tiempo_restante = 0;
        Juego.estadoActual = ESTADO_OFF; // Apagar
        Display_LCD_Limpiar();
        Display_LCD_Escribir(0, 0, "    BYE BYE!    ");
        Actualizar_Semaforo(LED_APAGADO);
        HAL_Delay(2000);
        Display_LCD_Limpiar();
        return; // Seguridad
    }

    // MÁQUINA DE ESTADOS
    switch (Juego.estadoActual) {

        case ESTADO_OFF:
            // No se hace nada hasta que se toque reset
            if (btnMenu == BTN_SHORT_CLICK) Juego.estadoActual = ESTADO_INICIO;
            break;

        case ESTADO_INICIO:

            // Pantalla de Bienvenida
            if (Juego.tiempoEnEstado < 100) { // Solo la primera vez
                Display_LCD_Escribir(0, 0, " BIENVENIDO "); // ARRIBA IZQ
                Display_LCD_Escribir(1, 0, "PULSA OK (btn DER) "); // ABAJO IZQ
            }

            // Cambio de estado
            if (btnValidar == BTN_SHORT_CLICK) {
                Juego.estadoActual = ESTADO_LOGIN;
                Audio_Play_Validar();
                Display_LCD_Limpiar();
            }
            break;

        case ESTADO_LOGIN:
			// Elegir ID con los potenciómetros
			Juego.usuarioID = (Inputs_ReadPot(0)*1000) + (Inputs_ReadPot(1)*100) + (Inputs_ReadPot(2)*10) + Inputs_ReadPot(3);
			// Ejemplo: 1000 + 200 + 30 + 4 = 1234

			sprintf(lcd_buffer, "LOGIN ID: %04d", Juego.usuarioID);
			Display_LCD_Escribir(0, 0, lcd_buffer);
			Display_LCD_Escribir(1, 0, "PULSA OK AL TERMINAR");

			// Cambio de estado
			if (btnValidar == BTN_SHORT_CLICK) {
				Audio_Play_Validar();

				// (INSTRUCCIONES)
				Display_LCD_Limpiar();
				Display_LCD_Escribir(0, 0, "USA POTE 1 MENU"); // Avisamos qué tocar
				Display_LCD_Escribir(1, 0, "PULSA OK");
				HAL_Delay(3000); // Congelamos 3 segs para que lean
				Display_LCD_Limpiar();

				Juego.estadoActual = ESTADO_SELECCION_DIF;
			}
			break;

		case ESTADO_SELECCION_DIF:
			// Elegir dificultad con Pote 0
			uint8_t val = Inputs_ReadPot(0);

			if (val < 3)
			{
				Juego.dificultad = DIFICULTAD_FACIL;
				Display_LCD_Escribir(0, 0, "DIFICULTAD: FACIL");
				Display_LCD_Escribir(1, 0, "TIEMPO: 120s    ");
				Juego.tiempoRestante_ms = 120000; // 2 min
			} else if (val < 7)
			{
				Juego.dificultad = DIFICULTAD_MEDIO;
				Display_LCD_Escribir(0, 0, "DIFICULTAD: MEDIO");
				Display_LCD_Escribir(1, 0, "TIEMPO: 90s     ");
				Juego.tiempoRestante_ms = 90000; // 1.5 min
			} else
			{
				Juego.dificultad = DIFICULTAD_DIFICIL;
				Display_LCD_Escribir(0, 0, "DIFICULTAD: HARD ");
				Display_LCD_Escribir(1, 0, "TIEMPO: 60s     ");
				Juego.tiempoRestante_ms = 60000; // 1 min
			}

			// Cambio de estado
			if (btnValidar == BTN_SHORT_CLICK) {
				tiempo_restante = Juego.tiempoRestante_ms / 1000;
				GenerarSecreto(); // Creamos la clave
				Juego.indiceDigitoActual = 0;
				Juego.estadoActual = ESTADO_CUENTA_ATRAS;
				Juego.tiempoEnEstado = 0;
				Display_LCD_Limpiar();
			}
			break;

        case ESTADO_CUENTA_ATRAS:
            if (Juego.tiempoEnEstado < 1000) Display_LCD_Escribir(0, 6, " 3 ");
            else if (Juego.tiempoEnEstado < 2000) Display_LCD_Escribir(0, 6, " 2 ");
            else if (Juego.tiempoEnEstado < 3000) Display_LCD_Escribir(0, 6, " 1 ");
            else {
                Audio_Play_Tic(); // Sonidino de start
                Juego.estadoActual = ESTADO_JUEGO_ACTIVO;
                Display_LCD_Limpiar();
            }
            break;

        case ESTADO_JUEGO_ACTIVO:
			// ¿GAME OVER?
			if (Juego.tiempoRestante_ms == 0) {
				tiempo_restante = 0;
				Juego.estadoActual = ESTADO_FIN_GAMEOVER;
				Audio_Play_GameOver();
				break;
			}
			static uint32_t ultimo_refresco_lcd = 0;
			// ACTUALIZAR PANTALLA
			if (HAL_GetTick() - ultimo_refresco_lcd > 100) {
				ultimo_refresco_lcd = HAL_GetTick(); // Actualizamos reloj

			int segundos = Juego.tiempoRestante_ms / 1000;
			sprintf(lcd_buffer, "T:%03ds  	CLAVE:%d", segundos, Juego.indiceDigitoActual + 1);
			Display_LCD_Escribir(0, 0, lcd_buffer);

			char displayLine2[17];
			for(int i=0; i<4; i++) {
				if(i < Juego.indiceDigitoActual) {
					// Ya acertados (se muestran fijos)
					displayLine2[i*4] = Juego.codigoSecreto[i] + '0';
				} else if (i == Juego.indiceDigitoActual) {
					// El que estás moviendo ahora
					displayLine2[i*4] = Inputs_ReadPot(i) + '0';
				} else {
					// Los futuros (ocultos)
					displayLine2[i*4] = '*';
				}
				// Relleno estético para separar ( 1   * * * )
				displayLine2[i*4 + 1] = ' ';
				displayLine2[i*4 + 2] = ' ';
				displayLine2[i*4 + 3] = ' ';
			}
			displayLine2[13] = '\0'; // Cortamos string (ajuste visual rápido)
			Display_LCD_Escribir(1, 2, displayLine2); // Centrado aprox

			// SEMÁFORO
			ActualizarFeedbackVisual();
			}
			// LÓGICA DE BOTONES
			if (btnValidar == BTN_SHORT_CLICK) {
				// Chequeamos SOLO el dígito actual
				uint8_t val = Inputs_ReadPot(Juego.indiceDigitoActual);
				uint8_t target = Juego.codigoSecreto[Juego.indiceDigitoActual];

				if (val == target) {
					// ACIERTO
					Audio_Play_Validar();
					flag_sonar_tic = 0;
					HAL_Delay(600);
					Juego.indiceDigitoActual++; // Pasamos al siguiente candado

					// Si era el último (ya tenemos los 4)...
					if (Juego.indiceDigitoActual >= 4) {
						tiempo_restante = 0;
						Juego.estadoActual = ESTADO_FIN_VICTORIA;
						Audio_Play_Victoria();
					}
				} else {
					// FALLO
					// Penalización (-5s)
					if(Juego.tiempoRestante_ms > 5000) Juego.tiempoRestante_ms -= 5000;
					else Juego.tiempoRestante_ms = 0;
					Audio_Play_Error();
					HAL_Delay(600);
					flag_sonar_tic = 0;
				}
			}
			// PEDIR PISTA
			else if (btnValidar == BTN_LONG_CLICK) {
				if (Juego.dificultad == DIFICULTAD_DIFICIL) {
					Audio_Play_Error();
				} else {
					// Penalización (-15s)
					if(Juego.tiempoRestante_ms > 15000) Juego.tiempoRestante_ms -= 15000;
					else Juego.tiempoRestante_ms = 0;
					Audio_Play_Tic();
					// Enviamos solo el número que se complica
					Bluetooth_EnviarPista(Juego.codigoSecreto, Juego.dificultad);
					Display_LCD_Escribir(0, 0, "PISTA ENVIADA...");
					HAL_Delay(1000);
				}
			}
			break;

        case ESTADO_FIN_VICTORIA:
            Display_LCD_Escribir(0, 0, "  ¡¡ VICTORIA !! ");
            Actualizar_Semaforo(LED_VICTORIA); // Todos los LEDs on

            // A los 5 segundos, guardamos ranking
                        if (Juego.tiempoEnEstado > 5000) {

                        	// --- BORRA LA LÍNEA ANTIGUA Y PON ESTO ---

                        	// 1. Calculamos el tiempo total según dificultad
                            uint32_t tiempoTotal = 120000; // Facil
                            if(Juego.dificultad == DIFICULTAD_MEDIO) tiempoTotal = 90000;
                            if(Juego.dificultad == DIFICULTAD_DIFICIL) tiempoTotal = 60000;

                            // 2. Calculamos cuánto has tardado
                            uint32_t segundosTardados = (tiempoTotal - Juego.tiempoRestante_ms) / 1000;

                            // 3. Guardamos ESO en el ranking
                            Ranking_Actualizar(Juego.usuarioID, segundosTardados, Juego.dificultad);

                            // -----------------------------------------

                            Juego.estadoActual = ESTADO_RANKING;
                            Display_LCD_Limpiar();
                        }

            break;

        case ESTADO_FIN_GAMEOVER:
            Display_LCD_Escribir(0, 0, "  XX BOOOOM XX  ");
            Actualizar_Semaforo(LED_FRIO_ROJO);

            if (Juego.tiempoEnEstado > 5000) {
                Juego.estadoActual = ESTADO_RANKING;
                Display_LCD_Limpiar();
            }
            break;

        case ESTADO_RANKING:
                    static uint32_t timer_ranking = 0;
                    static int indice_mostrado = 0;
                    uint32_t ahora = HAL_GetTick();

                    if (ahora - timer_ranking > 5000) {
                        timer_ranking = ahora;
                        indice_mostrado = 0;
                    }

                    // TITULO DINÁMICO SEGÚN DIFICULTAD
                    if(Juego.dificultad == DIFICULTAD_FACIL)      Display_LCD_Escribir(0, 0, "TOP 10 - FACIL ");
                    else if(Juego.dificultad == DIFICULTAD_MEDIO) Display_LCD_Escribir(0, 0, "TOP 10 - MEDIO ");
                    else                                          Display_LCD_Escribir(0, 0, "TOP 10 - HARD  ");

                    // Carrusel
                    if (ahora - timer_ranking > 2000) {
                        timer_ranking = ahora;
                        indice_mostrado++;
                        if (indice_mostrado >= 10) indice_mostrado = 0;
                    }
                    // Usamos Juego.dificultad para saber qué lista sacar
                    Jugador p = Top10[Juego.dificultad][indice_mostrado];
                    // ------------------------------------------------------

                    if (p.puntuacion >= 99990) {
                       sprintf(lcd_buffer, "%d. --- VACIO ---", indice_mostrado + 1);
                    } else {
                       sprintf(lcd_buffer, "%d.ID:%04d T:%lds", indice_mostrado + 1, p.id_usuario, p.puntuacion);
                    }
                    Display_LCD_Escribir(1, 0, lcd_buffer);

                    if (btnValidar == BTN_SHORT_CLICK || btnMenu == BTN_SHORT_CLICK) {
                        Juego.estadoActual = ESTADO_INICIO;
                        Audio_Play_Validar();
                        indice_mostrado = 0;
                    }
                    break;


    }
}
