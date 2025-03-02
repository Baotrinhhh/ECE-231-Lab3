#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "pin_config_lib.h"

// Function prototype
void helper(int state, long *count, const char *gpio_pin_number, const char *pwmchip, 
            const char *channel, const char *period, const char *duty_cycle, const char *pin_number);

int main() {
    // GPIO and PWM pin configurations
    const char *gpio_pin_number_P8_8 = "P8_08";
    const char *gpio_pin_number_P8_9 = "P8_09";
    const char *pin_number = "P9_16";
    const char *pwmchip = "pwmchip4";
    const char *channel = "1";
    
    // PWM timing configurations
    const char *periodP8_8 = "10000000";  // 0.01 sec in nanoseconds
    const char *duty_cycle_P8_8 = "6000000";  // 0.006 sec in nanoseconds
    const char *periodP8_9 = "1000000";  // 0.001 sec in nanoseconds
    const char *duty_cycle_P8_9 = "700000";  // 0.0007 sec in nanoseconds

    // GPIO numbers
    int gpio_number_P8_8 = 67;
    int gpio_number_P8_9 = 69;

    // Configure pins as input
    configure_gpio_input(gpio_number_P8_8, gpio_pin_number_P8_8);
    configure_gpio_input(gpio_number_P8_9, gpio_pin_number_P8_9);

    // Configure PWM pin
    stop_pwm(pin_number, pwmchip, channel);

    // File paths for reading button status
    char valuePath_P8_8[40], valuePath_P8_9[40];
    sprintf(valuePath_P8_8, "/sys/class/gpio/gpio%d/value", gpio_number_P8_8);
    sprintf(valuePath_P8_9, "/sys/class/gpio/gpio%d/value", gpio_number_P8_9);

    // Wait before first reading to avoid faulty readings
    sleep(1);
    
    long count_P8_8 = 0, count_P8_9 = 0;
    int state_P8_8, state_P8_9;

    FILE *fp;

    // Infinite loop to monitor button events
    while (1) {
        // Read button states
        fp = fopen(valuePath_P8_8, "r");
        fscanf(fp, "%d", &state_P8_8);
        fclose(fp);

        fp = fopen(valuePath_P8_9, "r");
        fscanf(fp, "%d", &state_P8_9);
        fclose(fp);

        // Handle button press events
        helper(state_P8_8, &count_P8_8, gpio_pin_number_P8_8, pwmchip, channel, periodP8_8, duty_cycle_P8_8, pin_number);
        helper(state_P8_9, &count_P8_9, gpio_pin_number_P8_9, pwmchip, channel, periodP8_9, duty_cycle_P8_9, pin_number);
    }

    return 0;
}

// Handles button press events and starts/stops PWM accordingly
void helper(int state, long *count, const char *gpio_pin_number, const char *pwmchip, 
            const char *channel, const char *period, const char *duty_cycle, const char *pin_number) {
    if (state == 0) {
        (*count)++;
        printf("Pin %s Interrupted %lu\n", gpio_pin_number, *count);
        
        start_pwm(pin_number, pwmchip, channel, period, duty_cycle);
        sleep(5);
        stop_pwm(pin_number, pwmchip, channel);
    }
}
