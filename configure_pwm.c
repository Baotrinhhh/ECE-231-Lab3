#include "pin_config_lib.h"

//set the duty cycle for the specified pwm_pin
void set_pwm_duty_cycle(char* pwmchip, char* channel, char* duty_cycle){
    // export file path
    char PWMDutyCycle[60];
    sprintf(PWMDutyCycle, "/sys/class/pwm/%s/pwm-4:%s/duty_cycle", pwmchip, channel);
    // configure PWM device
    FILE* fp = fopen(PWMDutyCycle, "w");
    fwrite(duty_cycle, sizeof(char), strlen(duty_cycle), fp);
    fclose(fp);
}

//set the period for the specified pwm_pin. Sets duty_cycle to half if the period is less than current duty_cycle
void set_pwm_period(char* pwmchip, char* channel, char* period){
    long duty_cycle_int, period_int;
    // before setting up the period read old duty cycle
    char PWMDutyCycle[60], duty_cycle_str[20];
    sprintf(PWMDutyCycle, "/sys/class/pwm/%s/pwm-4:%s/duty_cycle", pwmchip, channel);
    FILE* fp = fopen(PWMDutyCycle, "r");
    fscanf(fp, "%ld", &duty_cycle_int);
    fclose(fp);

    period_int = atol(period);
    // If the old duty_cycle value is greater than the new period
    // update the dummy_duty_cycle first to avoid errors with setting up
    // the period
    if( duty_cycle_int >= period_int){
    duty_cycle_int = period_int/2;
    // converting long to char data type
    sprintf(duty_cycle_str, "%ld", duty_cycle_int);
    // setup dummy duty cycle
    set_pwm_duty_cycle(pwmchip, channel, duty_cycle_str);
    }

    // export file path
    char PWMPeriod[60];
    sprintf(PWMPeriod, "/sys/class/pwm/%s/pwm-4:%s/period", pwmchip,
    channel);
    fp = fopen(PWMPeriod, "w");
    fwrite(period, sizeof(char), strlen(period), fp);
    fclose(fp);
}

// sets enable to 0 on specified pwm_pin (turns off)
void stop_pwm(char* pin_number, char* pwmchip, char* channel){
    char PWMDisable[40];
    sprintf(PWMDisable, "/sys/class/pwm/%s/pwm-4:%s/enable",
    pwmchip, channel);
    // stop generating PWM
    FILE* fp = fopen(PWMDisable, "w");
    fwrite("0", sizeof(char), 1, fp);
    fclose(fp);
}

// sets enable to 1 on specified pwm_pin (turns on). Also sets up period and duty_cycle using corresponding functions
void start_pwm(char* pin_number, char* pwmchip, char* channel, char* period, char* duty_cycle){
    /* Input:
    pin_number: pin_number to generate PWM on
    pwmchip: the device folder to generate PWM
    channel: pwm device channel
    perod: pwm period
    duty_cycle: pwm duty cyle
    */
    // configure the pin in PWM mode
    config_pin(pin_number, "pwm");
    FILE* fp;
    // configure PWM Period
    set_pwm_period(pwmchip, channel, period);
    // configure PWM Duty Cycle
    set_pwm_duty_cycle(pwmchip, channel, duty_cycle);
    // enable PWM
    char PWMEnable[40];
    sprintf(PWMEnable, "/sys/class/pwm/%s/pwm-4:%s/enable",
    pwmchip, channel);
    // configure generating PWM
    fp = fopen(PWMEnable, "w");
    fwrite("1", sizeof(char), 1, fp);
    fclose(fp);
}
