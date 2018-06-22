/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2010-2015 Piotr Esden-Tempski <piotr@esden.net>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#define STM32F4



#include <string>



extern "C"
{
#include <stdio.h>
#include <ctype.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>

#include "clock.h"
#include "sdram.h"
#include "lcd.h"
#include "gfx.h"
}

/* Set STM32 to 168 MHz. */
//static void clock_setup(void)
//{
//	rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
//}

static void gpio_setup(void)
{
	/* Enable GPIOG clock. */
	rcc_periph_clock_enable(RCC_GPIOG);

	/* Set GPIO13 (in GPIO port G) to 'output push-pull'. */
	gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO13);
}

static void button_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}

int main(void)
{
    using namespace std;
	int i;

	clock_setup();
	button_setup();
	gpio_setup();

        /* Enable the SDRAM attached to the board */
	sdram_init();
	/* Enable the LCD attached to the board */
        lcd_init();

	printf("System initialized.\n");
        
        
        gfx_init(lcd_draw_pixel, 240, 320);
	gfx_fillScreen(LCD_GREY);
	gfx_fillRoundRect(10, 10, 220, 220, 5, LCD_WHITE);
	gfx_drawRoundRect(10, 10, 220, 220, 5, LCD_RED);
	gfx_fillCircle(20, 250, 10, LCD_RED);
	gfx_fillCircle(120, 250, 10, LCD_GREEN);
	gfx_fillCircle(220, 250, 10, LCD_BLUE);
	gfx_setTextSize(2);
	gfx_setCursor(15, 25);
	gfx_puts(R"(STM32F4-DISCO)");
	gfx_setTextSize(1);
	gfx_setCursor(15, 49);
	gfx_puts(R"(Simple example to put some)");
	gfx_setCursor(15, 60);
	gfx_puts(R"(stuff on the LCD screen.)");
	lcd_show_frame();
        
        
        
	/* Blink the LED (PD13) on the board. */
	while (1) {
		gpio_toggle(GPIOG, GPIO13);

		/* Upon button press, blink more slowly. */
		if (gpio_get(GPIOA, GPIO0)) {
//			for (i = 0; i < 3000000; i++) {	/* Wait a bit. */
//				__asm__("nop");
//			}
                    msleep(1000);
		}

                
                msleep(500);
//		for (i = 0; i < 3000000; i++) {		/* Wait a bit. */
//			__asm__("nop");
//		}
	}

	return 0;
}
