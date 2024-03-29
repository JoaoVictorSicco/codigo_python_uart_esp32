#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"


#define BUTTON_PIN  GPIO_NUM_21  // Pin do botão (altere conforme necessário)
#define ESP_INTR_FLAG_DEFAULT 0

#define LEVEL_BUTTON (0)

static const char *SYSTEM_TAG = "SYSTEM";
static const char *TAG = "UART";

#define UART_NUM UART_NUM_0
#define BUF_SIZE (1024)



void apl_uart_init() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
}

// void uart_task(void *pvParameters) {
//     uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
//     while (1) {
//         uart_flush_input(UART_NUM); // Limpeza do buffer de entrada
//         int len = uart_read_bytes(UART_NUM, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
//         if(len > 0) 
//         {
//             // Convertendo os bytes para uma string
//             char str[len + 1];
//             memcpy(str, data, len);
//             str[len] = '\0';  // Adicionando o caractere nulo ao final da string
//             ESP_LOGI(TAG, "Received: %s", str);
//         }
//     }
//     free(data);
// }

void apl_cadastro_ID()
{
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        uart_flush_input(UART_NUM); // Limpeza do buffer de entrada
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if(len > 0) 
        {
            // Convertendo os bytes para uma string
            char str[len + 1];
            memcpy(str, data, len);
            str[len] = '\0';  // Adicionando o caractere nulo ao final da string
            ESP_LOGI(TAG, "Received: %s", str);
        }
        break;
    }
    free(data);
}

void apl_gpio_config()
{
    gpio_config_t io_conf;
    // Configurações do botão como entrada
    io_conf.intr_type = GPIO_INTR_POSEDGE;  
    io_conf.pin_bit_mask = (1ULL << BUTTON_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);

}

void button_task(void *pvParameter) 
{

    while(1)
    {
        if (gpio_get_level(BUTTON_PIN) == 0) // Verifica se o botão foi pressionado (nível baixo)
        { 
            // printf("Modo Fábrica\n");
            // apl_cadastro_ID(); 
            while(1)
            {
                printf("Modo Fábrica\n");
                apl_cadastro_ID();
                vTaskDelay(pdMS_TO_TICKS(500)); 
            }
            // vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay para evitar leituras múltiplas enquanto o botão estiver pressionado
            vTaskDelay(pdMS_TO_TICKS(1000));  // Em millis
        }
        printf("Modo normal\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
    
        
}

void app_main() 
{    
    apl_uart_init();
    apl_gpio_config();
    
    // Inicia a tarefa que verifica o botão
    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);

    // ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0));
    
    // xTaskCreate(uart_task, "uart_task", 2048, NULL, 10, NULL);
}