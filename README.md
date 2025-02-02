# Projeto: Interrupções e Controle de LEDs na Placa BitDogLab

Este projeto tem como objetivo explorar o uso de interrupções no microcontrolador RP2040, utilizando a placa de desenvolvimento BitDogLab. O projeto envolve o controle de LEDs comuns e LEDs endereçáveis WS2812, além da manipulação de botões com debouncing via software.

## 🎯 Objetivos
- Compreender o funcionamento e a aplicação de interrupções em microcontroladores.
- Implementar um sistema de debouncing para botões.
- Controlar LEDs RGB e LEDs endereçáveis WS2812.
- Utilizar resistores de pull-up internos para botões.
- Desenvolver um projeto funcional que integre hardware e software.

## 🎥 Entrega e Demonstração
**Vídeo de demonstração:**
  - [Ver Video]()

## 🛠️ Componentes Utilizados
| Componente  | Pino GPIO |
|------------|----------|
| Matriz 5x5 de LEDs WS2812 | 7 |
| LED RGB (Vermelho, Azul e Verde) | 11, 12, 13 |
| Botão A  | 5 |
| Botão B  | 6 |

Os botões utilizam `pull-up` interno para evitar leituras flutuantes.

## 📌 Funcionalidades Implementadas
1. O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo.
2. O botão A incrementa o número exibido na matriz de LEDs quando pressionado.
3. O botão B decrementa o número exibido na matriz de LEDs quando pressionado.
4. A matriz WS2812 exibe números de 0 a 9 utilizando um formato fixo ou criativo.

## 📜 Requisitos do Projeto
- **Uso de interrupções:** As funcionalidades dos botões devem ser implementadas com rotinas de interrupção (IRQ).
- **Debouncing:** O projeto deve tratar bouncing nos botões via software.
- **Controle de LEDs:** Deve incluir o uso de LEDs comuns e LEDs WS2812.
- **Código organizado e comentado:** Para facilitar o entendimento e manutenção.

## 🚀 Como Compilar e Executar
1. Instale o **Raspberry Pi Pico SDK**.
2. Clone este repositório:
   ```sh
   ```
3. Compile o código usando CMake e GCC para ARM.
4. Copie o arquivo `.uf2` gerado para a placa BitDogLab.

## 📩 Contato
Criado por **Everton Espedito**. Para dúvidas ou sugestões, entre em contato!
