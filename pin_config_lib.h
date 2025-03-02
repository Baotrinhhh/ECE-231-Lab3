#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/epoll.h>
#include <time.h>
//Sets up gpio pin as input
void configure_gpio_input(int gpio_number, char* pin_number);

// Sets up a pin to a given mode (e.g. PWM)
void config_pin(char* pin_number, char* mode);

// catches CTRL+C interrupts to turn off pwm on user exit, as the program otherwise runs forever
void sigintHandler(int sig_num); 

//set the duty cycle for the specified pwm_pin
void set_pwm_duty_cycle(char* pwmchip, char* channel, char* duty_cycle);

//set the period for the specified pwm_pin. Sets duty_cycle to half if the period is less than current duty_cycle
void set_pwm_period(char* pwmchip, char* channel, char* period);

// sets enable to 0 on specified pwm_pin (turns off)
void stop_pwm(char* pin_number, char* pwmchip, char* channel);

// sets enable to 1 on specified pwm_pin (turns on). Also sets up period and duty_cycle using correspondin functions
void start_pwm(char* pin_number, char* pwmchip, char* channel, char* period, char* duty_cycle);
