#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "ws2812.pio.h"  // Biblioteca para controlar LEDs WS2812

// Definições dos pinos
#define LED_RGB_RED_PIN 13 //Red=13, Blue=12, Green=11
#define LED_RGB_GREEN_PIN 11
#define LED_RGB_BLUE_PIN 12
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define WS2812_PIN 7
#define NUM_LEDS 25  // Matriz 5x5 de LEDs WS2812

// Variáveis globais
volatile int current_number = 0;  // Número atual exibido na matriz de LEDs
volatile uint32_t last_press_time = 0;  // Último tempo de pressionamento do botão
const uint32_t debounce_time = 200;  // Tempo de debouncing em milissegundos

// Padrões para os números de 0 a 9 (matriz 5x5)
const uint32_t patterns[10][25] = {
    // 0
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 1
    {0, 0, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 1, 0},

    // 2
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1},

    // 3
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 4
    {1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    // 5
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 6
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 7
    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 1, 0,
     0, 0, 1, 0, 0,
     0, 1, 0, 0, 0},

    // 8
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    // 9
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1}
};

// Função para piscar o LED vermelho
void blink_red_led() {
    static bool led_state = false;
    led_state = !led_state;
    gpio_put(LED_RGB_RED_PIN, led_state);
}

// Função para exibir números na matriz de LEDs WS2812
void display_number(int number) {
    // Verifica se o número está no intervalo válido (0-9)
    if (number < 0 || number > 9) return;

    // Define as cores dos LEDs com base no padrão
    for (int i = 0; i < NUM_LEDS; i++) {
        uint32_t color = (patterns[number][i] == 1) ? 0xFF0000 : 0x000000;  // Vermelho ou apagado
        pio_sm_put_blocking(pio0, 0, color << 8u);  // Envia o valor para o WS2812
    }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
    static absolute_time_t ultimo_tempo_a = 0;
    static absolute_time_t ultimo_tempo_b = 0;

    if (gpio == BUTTON_A_PIN) {
        // Botão A
        if (to_ms_since_boot(get_absolute_time()) - last_press_time > debounce_time) {
        current_number = (current_number + 1) % 10;  // Incrementa o número (0-9)
        display_number(current_number);
        last_press_time = to_ms_since_boot(get_absolute_time());
        }
    }else if (gpio == BUTTON_B_PIN) {
        // Botão B
        if (to_ms_since_boot(get_absolute_time()) - last_press_time > debounce_time) {
        current_number = (current_number - 1 + 10) % 10;  // Decrementa o número (0-9)
        display_number(current_number);
        last_press_time = to_ms_since_boot(get_absolute_time());
        }
    }
}
    

// Função principal
int main() {
    // Inicialização do SDK
    stdio_init_all();

    // Configuração dos pinos do LED RGB
    gpio_init(LED_RGB_RED_PIN);
    gpio_init(LED_RGB_GREEN_PIN);
    gpio_init(LED_RGB_BLUE_PIN);
    gpio_set_dir(LED_RGB_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_RGB_GREEN_PIN, GPIO_OUT);
    gpio_set_dir(LED_RGB_BLUE_PIN, GPIO_OUT);

    // Configuração dos botões com pull-up interno
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);


    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);

    // Inicialização do WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    // Timer para piscar o LED vermelho
    struct repeating_timer timer;
    add_repeating_timer_ms(200, (repeating_timer_callback_t)blink_red_led, NULL, &timer);

    // Exibe o número inicial na matriz de LEDs
    display_number(current_number);

    // Loop principal
    while (true) {
        // Configuração das interrupções dos botões
        tight_loop_contents();  // Mantém o programa rodando de forma eficiente
    }

    return 0;
}