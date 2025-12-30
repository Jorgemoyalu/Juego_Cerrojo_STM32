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
    	uint32_t suma = 0;
    	// Tomamos 10 muestras rápidas
    	for(int j=0; j<10; j++){
    	HAL_ADC_Start(hadc);
    	if (HAL_ADC_PollForConversion(hadc, 5) == HAL_OK)
    		suma += HAL_ADC_GetValue(hadc);
    	}
    	// Calculamos la media
    	uint32_t media = suma / 10;
    	// Mapeamos el valor medio (0-4095 -> 0-9)
        uint8_t valorMapeado = (media * 10) / 4095;
        if (valorMapeado > 9) valorMapeado = 9;

        misInputs->digito[i] = valorMapeado;
        }
    HAL_ADC_Stop(hadc);
}

void Display_BarraProgreso(uint8_t fila, uint8_t porcentaje) {
    // Aseguramos que el porcentaje no pase de 100
    if (porcentaje > 100) porcentaje = 100;

    // Calculamos cuántos bloques negros hay que pintar (de 16 columnas)
    uint8_t bloques = (porcentaje * 16) / 100;

    // Nos ponemos al principio de la fila
    Display_LCD_Escribir(fila, 0, "");
    // Comando para mover cursor: 0x80 para fila 0, 0xC0 para fila 1
    uint8_t pos = (fila == 0) ? 0x80 : 0xC0;
    LCD_Send_Cmd(pos);

    for (int i = 0; i < 16; i++) {
        if (i < bloques) {
            LCD_Send_Data(0xFF);
        } else {
            LCD_Send_Data(' ');
        }
    }
}
