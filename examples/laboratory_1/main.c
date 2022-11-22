#include <stdio.h>
#include <stdlib.h>
#include "button.h"
#include "timer.h"
#include "led.h"
#include "arraylist.h"

typedef enum direction {
    DIRECT = 1, REVERSED = -1
} Direction;

typedef struct stream {
    ArrayList *arraylist;
    Direction direction;
    int current_id;
} Stream;

Stream *stream_create(ArrayList *arraylist) {
    Stream *stream = malloc(sizeof(Stream));
    if (stream == NULL)
        exit(1);
    stream->current_id = 0;
    stream->direction = DIRECT;
    stream->arraylist = arraylist;
    return stream;
}

void *stream_next(Stream *stream) {
    if (stream->direction == DIRECT) {
        if (arraylist_size(stream->arraylist) - 1 == stream->current_id)
            stream->current_id = 0;
        else
            stream->current_id++;
    } else if (stream->direction == REVERSED) {
        if (stream->current_id == 0)
            stream->current_id = arraylist_size(stream->arraylist) - 1;
        else
            stream->current_id--;
    }

    return arraylist_get(stream->arraylist, stream->current_id);
}

void *stream_get(Stream *stream) {
    return arraylist_get(stream->arraylist, stream->current_id);
}

void timer_light_handler(void *arg) {
    Stream *led_stream = (Stream *) arg;
    uint32_t led_current = *((uint32_t *) stream_get(led_stream));
    uint32_t led_next = *((uint32_t *) stream_next(led_stream));

    led_turn_off(led_current);
    led_turn_on(led_next);
}

void button_pressed_handler(void *arg) {
    Stream *led_stream = (Stream *) arg;
    led_stream->direction *= -1;
    if (DIRECT == led_stream->direction) {
        timer_set_time(TIMER_2, 500, 42000);
    } else if (REVERSED == led_stream->direction) {
        timer_set_time(TIMER_2, 1200, 42000);
    }
    led_turn_off(LED_ALL);
}

int main() {
    uint32_t led_1 = LED_3;
    uint32_t led_2 = LED_4;
    uint32_t led_3 = LED_5;
    uint32_t led_4 = LED_6;

    ArrayList *led_list = arraylist_create(4);
    Stream *led_stream = stream_create(led_list);

    Handler *timer_handler = handler_create(&timer_light_handler, led_stream);
    Handler *button_handler = handler_create(&button_pressed_handler, led_stream);

    GPIO_InitTypeDef *led_config = led_get_default_config();

    arraylist_add(led_list, (void *) &led_1);
    arraylist_add(led_list, (void *) &led_2);
    arraylist_add(led_list, (void *) &led_3);
    arraylist_add(led_list, (void *) &led_4);

    GPIO_Init(GPIOD, led_config);

    timer_add_handler(TIMER_2, timer_handler);
    timer_set_time(TIMER_2, 500, 42000);

    button_add_handler(button_handler);

    while (1);
}