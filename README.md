# 🔐 Proyecto Microcontroladores: El Juego del Cerrojo



> **Repositorio oficial del Grupo 11.** Implementación de un sistema de juego electrónico basado en desbloqueo por código numérico mediante periféricos analógicos y digitales.

---

## 📋 Descripción del Proyecto

Este proyecto implementa el **"Juego del Cerrojo"**. El objetivo es adivinar una combinación secreta de 4 dígitos generada aleatoriamente por el microcontrolador.
El sistema integra lectura de sensores analógicos, feedback visual/sonoro y lógica de estados finitos.

### Funcionalidades Principales
* **Interfaz de Entrada:** 4 Potenciómetros para seleccionar dígitos (0-9) y botones de control.
* **Feedback:** Pantalla LCD para menús y LEDs/Buzzer para indicar aciertos (Caliente) o fallos (Frío).
* **Conectividad:** Módulo Bluetooth para monitorización remota (Modo Espía/Pistas).
* **Temporización:** Cuenta atrás y control de tiempos de juego.

---

## ⚠️ Normas de Trabajo (Workflow GIT)

Para mantener la integridad del código y evitar conflictos, **es obligatorio seguir estas reglas**:

1.  🛑 **PROHIBIDO trabajar en la rama `main`**. La rama `main` solo debe contener código funcional, probado y libre de errores.
2.  **Cada integrante debe trabajar en su propia rama**. Antes de empezar a programar, crea una rama con tu nombre o la funcionalidad:
    * `dev-moya` (Drivers Hardware)
    * `dev-gabri` (Lógica/Bluetooth)
    * `dev-tudor` (LCD/Timers)
3.  **Testear antes de fusionar**. No hagas un *Merge* o *Pull Request* a `main` hasta que hayas compilado y probado que tu parte funciona en la placa.
4.  **Commits Descriptivos**. Usa mensajes claros: `Feat: Añadido driver ADC`, `Fix: Corregido rebote botón`, etc.

---

## 🛠️ Estructura del Hardware & Responsables

| Componente | Conexión (Pinout) | Responsable | Estado |
| :--- | :--- | :--- | :--- |
| **Microcontrolador** | STM32F411RE / F407 | - | ✅ Configurado |
| **4x Potenciómetros** | Pines ADC (`PA0`, `PA1`, `PA4`, `PA5`) | **Moya** | ✅ Implementado |
| **Botones (Validar/Rst)** | GPIO con Interrupciones Externas | **Moya** | ✅ Implementado |
| **Buzzer (Pasivo)** | PWM (Timer) | **Moya** | ✅ Implementado |
| **Pantalla LCD** | I2C (`PB6`, `PB7`) | **Tudor** | ⏳ Pendiente |
| **Módulo Bluetooth** | UART (`PA9`, `PA10`) | **Gabri** | ⏳ Pendiente |
| **Lógica del Juego** | Máquina de Estados (FSM) | **Gabri** | ⏳ Pendiente |

---

## 🚀 Instalación y Setup

Sigue estos pasos para clonar el proyecto y trabajar en tu máquina local:

1.  **Clonar el repositorio:**
    Abre GitHub Desktop o terminal y ejecuta:
    ```bash
    git clone [https://github.com/TU_USUARIO/Juego_Cerrojo_STM32.git](https://github.com/TU_USUARIO/Juego_Cerrojo_STM32.git)
    ```

2.  **Importar en STM32CubeIDE:**
    * Abre el IDE.
    * Ve a `File` -> `Import` -> `General` -> `Existing Projects into Workspace`.
    * Selecciona la carpeta clonada.
    * **IMPORTANTE:** ❌ DESMARCA la casilla "Copy projects into workspace" (para que los cambios se guarden en la carpeta de Git).

3.  **Generar Código:**
    * Abre el archivo `.ioc`.
    * Revisa la configuración de pines.
    * Guarda (Ctrl+S) para generar el código base (`main.c`, etc.).

---

## 📂 Organización de Módulos

El código se ha modularizado para facilitar el desarrollo paralelo:

* `Core/Src/inputs.c`: Drivers de ADC, Botones y LEDs (Moya).
* `Core/Src/audio.c`: Control de PWM y melodías (Moya).
* `Core/Src/display.c`: Librería y gestión de la LCD (Tudor).
* `Core/Src/game_logic.c`: Máquina de estados del juego (Gabri).
* `Core/Src/bluetooth.c`: Gestión de comandos UART (Gabri).

---

## 👥 Autores - Grupo 11

* **Moya** - *Hardware Abstraction Layer (HAL) & Audio*
* **Gabri** - *Game Logic & Communications*
* **Tudor** - *Display Interface & Timing*

---
