#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "pin_config_lib.h"

void helper(int state, int count, char gpio_pin_number, char pwmchip, char channel, char period, char duty_cycle, char pin_number)

int main(){
    // configure pin P8_8 as input with internal pull-up enabled[Button 0]
    char gpio_pin_number_P8_8[32] = "P8_08";
    char gpio_pin_number_P8_9[32] = "P8_09";
    char pin_number[32] = "P9_16";
    char pwmchip[32] = "pwmchip4";
    char channel[32] = "1";
    char periodP8_8[32] = "10000000"; // 0.01 second in nanosecs
    char duty_cycle_P8_8[32] = "6000000"; // 0.006 second in nanosecs
    char periodP8_9[32] = "1000000"; // 0.001 second in nanosecs
    char duty_cycle_P8_9[32] = "700000"; // 0.0007 second in nanosecs


    int gpio_number_P8_8 = 67;
    int gpio_number_P8_9 = 69;

    // config 2 pins P8_8 and P8_9 as GPIO inputs pin
    configure_gpio_input(gpio_number_P8_8, gpio_pin_number_P8_8);
    configure_gpio_input(gpio_number_P8_9, gpio_pin_number_P8_9);

    // config pin P9_16 in PWM mode
    stop_pwm(pin_number, pwmchip, channel);

    // file path to read button status
    char valuePath_P8_8[40];
    char valuePath_P8_9[40];

    sprintf(valuePath_P8_8, "/sys/class/gpio/gpio%d/value", gpio_number_P8_8);
    sprintf(valuePath_P8_9, "/sys/class/gpio/gpio%d/value", gpio_number_P8_9);

    // wait before first readings to avoid faulty readings
    sleep(1);
    long count_P8_8 = 0;
    long count_P8_9 = 0;

    int state_P8_8;
    int state_P8_9;

    FILE *fp;


    // loop to monitor events
    while(1){
    // P8_8
    fp = fopen(valuePath_P8_8, "r");
    // default state is 1 since buttons are configured with
    // internal pull-up resistors. So, reading 0 means button
    // is pressed
    fscanf(fp, "%d", &state_P8_8);
    fclose(fp);

    //P8_9
    fp = fopen(valuePath_P8_9, "r");
    // default state is 1 since buttons are configured with
    // internal pull-up resistors. So, reading 0 means button
    // is pressed
    fscanf(fp, "%d", &state_P8_9);
    fclose(fp);


    // event detected
    // P8_8
    helper(state_P8_8, count_P8_8, gpio_pin_number_P8_8, pwmchip, channel, periodP8_8, duty_cycle_P8_8, pin_number);

    // P8_9
    helper(state_P8_9, count_P8_9, gpio_pin_number_P8_9, pwmchip, channel, periodP8_9, duty_cycle_P8_9, pin_number);


    }
    return 0;
}

void helper(int state, int count, char gpio_pin_number, char pwmchip, char channel, char period, char duty_cycle, char pin_number){
    if( state == 0 ){
        count++;
        printf("Pin %s Interrupted %lu\n", gpio_pin_number, count);
        start_pwm(pin_number, pwmchip, channel, period, duty_cycle);
        sleep(5);
        stop_pwm(pin_number, pwmchip, channel);
        }
}
