#include <stdio.h>
#include <stdlib.h>
#include "button.h"
#include "timer.h"
#include "led.h"
#include "arraylist.h"


//створюється структура що визначає напрям
typedef enum direction {
    DIRECT = 1, REVERSED = -1
} Direction;

//створення структури стрім
typedef struct stream {
    ArrayList *arraylist;
    Direction direction;
    int current_id;
} Stream;

//функція створення
Stream *stream_create(ArrayList *arraylist) {
    Stream *stream = malloc(sizeof(Stream)); //виділення пам'яті під стрім(потік)
    if (stream == NULL) //якщо пам'ять не виділилась - вихід із програми
        exit(1);
    stream->current_id = 0; //присвоєння id
    stream->direction = DIRECT; //присвоєння напряму
    stream->arraylist = arraylist; //иніціалізація масиву лед
    return stream;
}

//взяти наступний елемент і перемістити індект поточного елементу на 1 вперед
void *stream_next(Stream *stream) {
    if (stream->direction == DIRECT) { //якщо напрям потіку прямий
        if (arraylist_size(stream->arraylist) - 1 == stream->current_id) //оновлення id після включення світла останнього світлодіоду
            stream->current_id = 0;
        else
            stream->current_id++; //наступний свілодіод
    } else if (stream->direction == REVERSED) { //якщо напрям потоку зворотній
        if (stream->current_id == 0)
            stream->current_id = arraylist_size(stream->arraylist) - 1; //якщо current_id=0 то встіє на останню позицію
        else
            stream->current_id--; //попередній світлодіод
    }

    return arraylist_get(stream->arraylist, stream->current_id); //повертає наступний потік
}
//повернути поточний елемент у стрімі
void *stream_get(Stream *stream) {
    return arraylist_get(stream->arraylist, stream->current_id); //повертає поточний потік
}
//обробник переривання через таймер
void timer_light_handler(void *arg) { //обробник таймера хендлера
    Stream *led_stream = (Stream *) arg; // Отримуємо аргумент (в даному випадку стрім)
    uint32_t led_current = *((uint32_t *) stream_get(led_stream)); // Беремо поточний світлодіод
    uint32_t led_next = *((uint32_t *) stream_next(led_stream)); // Беремо наступний світлодіод

    led_turn_off(led_current); // Вимикаємо поточний світлодіод
    led_turn_on(led_next); // Вмикаємо наступний світлодіод
}
//обробний переривання через кнопку
void button_pressed_handler(void *arg) { //обробний натискання кнопки для зміни напрямку руху і вимкнення
    Stream *led_stream = (Stream *) arg; // Отримуємо аргумент (в даному випадку стрім)
    led_stream->direction *= -1; // Змінюємо напрям
    if (DIRECT == led_stream->direction) {
        timer_set_time(TIMER_2, 500, 42000);                  // В залежнсоті від напрямку
    } else if (REVERSED == led_stream->direction) {            // налаштовуємо час на таймері
        timer_set_time(TIMER_2, 1200, 42000);
    }
    led_turn_off(LED_ALL); // Вимикаємо усі світлодіоди
}
//
int main() {
    uint32_t led_1 = LED_3; //створення змінних що зберігають значення світодіодів
    uint32_t led_2 = LED_4;
    uint32_t led_3 = LED_5;
    uint32_t led_4 = LED_6;

    ArrayList *led_list = arraylist_create(4); //ініціалізація масиву
    Stream *led_stream = stream_create(led_list); //ініціалізація потоку

    Handler *timer_handler = handler_create(&timer_light_handler, led_stream); //стоврення хендленра
    Handler *button_handler = handler_create(&button_pressed_handler, led_stream); //створення обробника кнопки

    GPIO_InitTypeDef *led_config = led_get_default_config(); //дефолтний конфіг для світодіодів

    arraylist_add(led_list, (void *) &led_1); //додаємо наш стрім світодіоду
    arraylist_add(led_list, (void *) &led_2);
    arraylist_add(led_list, (void *) &led_3);
    arraylist_add(led_list, (void *) &led_4);

    GPIO_Init(GPIOD, led_config); //ініціалізуємо gpio для ледів

    timer_add_handler(TIMER_2, timer_handler); //додаємо хендлер для таймеру
    timer_set_time(TIMER_2, 500, 42000); //засікаємо час на таймері
    
    button_add_handler(button_handler); //додаємо обробник кнопки

    while (1);
}



