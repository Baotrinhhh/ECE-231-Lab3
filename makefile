CC = gcc
CFLAGS = -I.
DEPS = pin_config_lib.h

bbb_poll: configure_gpio.c configure_pwm.c polling_capture.c $(DEPS)
	$(CC) -o bbb_poll configure_gpio.c configure_pwm.c polling_capture.c $(CFLAGS)

.PHONY: clean
clean:
	 rm -f bbb_poll

