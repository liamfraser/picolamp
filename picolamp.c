#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

char linebuf[256];
unsigned w = 0;
unsigned y = 0;

#define WHITE_PWM_PIN 2
#define YELLOW_PWM_PIN 3

void print_help(void) {
    printf("Lamp Control available options:\n");
    printf("w 0-255: set white to value:\n");
    printf("y 0-255: set yellow to value\n");
    printf("p: print values\n");
}

void print_values(void) {
    printf("w: %d\n", w);
    printf("y: %d\n", y);
}

void handle_colour(char colour) {
    // Char 1 is a space to start at char 2
    int i = atoi(&linebuf[2]);

    if (i < 0 || i > 255) {
        printf("Ignoring bad colour %d\n", i);
        return;
    } else {
        printf("Setting %c to %d\n", colour, i);
    }

    // Store variable and set pwm level
    if (colour == 'w') {
        w = i;
        pwm_set_gpio_level(WHITE_PWM_PIN, i);
    } else if (colour == 'y') {
        y = i;
        pwm_set_gpio_level(YELLOW_PWM_PIN, i);
    }
}

void handle_line(unsigned len) {
    char c = linebuf[0];
    if (c == 'w' || c == 'y') {
        handle_colour(c);
    } else if (c == 'p') {
        print_values();
    } else {
        // Unknown
        print_help();
    }
}

void pwm_setup(void) {
    gpio_set_function(WHITE_PWM_PIN, GPIO_FUNC_PWM);
    gpio_set_function(YELLOW_PWM_PIN, GPIO_FUNC_PWM);

    // Should both be on same slice
    uint slice_num_white = pwm_gpio_to_slice_num(WHITE_PWM_PIN);
    uint slice_num_yellow = pwm_gpio_to_slice_num(YELLOW_PWM_PIN);
    assert(slice_num_white == slice_num_yellow);

    // Set up pwm in default mode
    pwm_config config = pwm_get_default_config();

    // Set max value to 255
    pwm_config_set_wrap(&config, 255);

    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num_white, &config, true);
}

int main() {
    stdio_init_all();

    pwm_setup();

    print_help();
    
    while (true) {
        unsigned i = 0;
        char c = 0;
        // Keep adding chars to linebuf until we see a newline
        do {
            // Get char from terminal
            c = getchar();
            // Put the char on the screen for the user
            putchar(c);
            // Store the char in the line buffer
            linebuf[i++] = c;
        } while (c != '\n' && c != '\r');

        // Handle the line
        handle_line(i);
    }

    return 0;
}
