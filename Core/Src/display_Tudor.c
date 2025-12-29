#include "display_Tudor.h"
#include "adc.h"
#include "gpio.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

#define LCD_ADDR (0x27 << 1)

void LCD_Send_Cmd(char cmd) {
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (cmd & 0xf0);
  data_l = ((cmd << 4) & 0xf0);
  data_t[0] = data_u | 0x0C;  // en=1, rs=0
  data_t[1] = data_u | 0x08;  // en=0, rs=0
  data_t[2] = data_l | 0x0C;  // en=1, rs=0
  data_t[3] = data_l | 0x08;  // en=0, rs=0
  HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void LCD_Send_Data(char data) {
  char data_u, data_l;
  uint8_t data_t[4];
  data_u = (data & 0xf0);
  data_l = ((data << 4) & 0xf0);
  data_t[0] = data_u | 0x0D;  // en=1, rs=1
  data_t[1] = data_u | 0x09;  // en=0, rs=1
  data_t[2] = data_l | 0x0D;  // en=1, rs=1
  data_t[3] = data_l | 0x09;  // en=0, rs=1
  HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}


void Display_Init(void) {
    // 1. Inicializar LEDs (Apagados)
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

    // 2. Inicializar LCD (Secuencia de arranque)
    HAL_Delay(50);
    LCD_Send_Cmd(0x30); HAL_Delay(5);
    LCD_Send_Cmd(0x30); HAL_Delay(1);
    LCD_Send_Cmd(0x30); HAL_Delay(10);
    LCD_Send_Cmd(0x20); HAL_Delay(10); // Modo 4 bits

    LCD_Send_Cmd(0x28); HAL_Delay(1); // Function set
    LCD_Send_Cmd(0x08); HAL_Delay(1); // Display off
    LCD_Send_Cmd(0x01); HAL_Delay(1); // Clear
    LCD_Send_Cmd(0x06); HAL_Delay(1); // Entry mode
    LCD_Send_Cmd(0x0C); HAL_Delay(1); // Display on, cursor off

    // Mensaje de Bienvenida
    Display_LCD_Escribir(0, 0, "INIT SYSTEM...");
}

void Display_LCD_Limpiar(void) {
    LCD_Send_Cmd(0x01);
    HAL_Delay(2);
}

void Display_LCD_Escribir(uint8_t fila, uint8_t col, char *texto) {
    // Fila 0 = 0x80, Fila 1 = 0xC0
    uint8_t pos = (fila == 0) ? 0x80 : 0xC0;
    LCD_Send_Cmd(pos | col);
    while (*texto) LCD_Send_Data(*texto++);
}

void Actualizar_Semaforo(EstadoSemaforo estado) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
    switch (estado) {
        case LED_FRIO_ROJO:      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); break;
        case LED_TEMPLADO_AMARILLO: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET); break;
        case LED_CALIENTE_VERDE: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); break;
        case LED_VICTORIA:       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_SET); break;
        default: break;
    }
}

void Leer_Potenciometros(ADC_HandleTypeDef *hadc, EntradasUsuario *misInputs) {
    for (int i = 0; i < 4; i++) {
        HAL_ADC_Start(hadc);
        if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
            uint32_t lecturaRaw = HAL_ADC_GetValue(hadc);
            // Mapeo 0-9
            uint8_t valorMapeado = (lecturaRaw * 10) / 4090;
            if (valorMapeado > 9) valorMapeado = 9;
            misInputs->digito[i] = valorMapeado;
        }
    }
    HAL_ADC_Stop(hadc);
}
