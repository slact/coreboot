/* SPDX-License-Identifier: GPL-2.0-only */

#include <southbridge/intel/common/gpio.h>

static const struct pch_gpio_set1 pch_gpio_set1_mode = {
	.gpio6 = GPIO_MODE_GPIO,
	.gpio7 = GPIO_MODE_GPIO,
	.gpio8 = GPIO_MODE_GPIO,
	.gpio9 = GPIO_MODE_GPIO,
	.gpio10 = GPIO_MODE_GPIO,
	.gpio12 = GPIO_MODE_GPIO,
	.gpio13 = GPIO_MODE_GPIO,
	.gpio14 = GPIO_MODE_GPIO,
	.gpio15 = GPIO_MODE_GPIO,
	.gpio16 = GPIO_MODE_GPIO,
	.gpio17 = GPIO_MODE_GPIO,
	.gpio18 = GPIO_MODE_GPIO,
	.gpio19 = GPIO_MODE_GPIO,
	.gpio20 = GPIO_MODE_GPIO,
	.gpio24 = GPIO_MODE_GPIO,
	.gpio25 = GPIO_MODE_GPIO,
	.gpio26 = GPIO_MODE_GPIO,
	.gpio27 = GPIO_MODE_GPIO,
	.gpio28 = GPIO_MODE_GPIO,
};

static const struct pch_gpio_set1 pch_gpio_set1_direction = {
	.gpio6 = GPIO_DIR_INPUT,
	.gpio7 = GPIO_DIR_INPUT,
	.gpio8 = GPIO_DIR_INPUT,
	.gpio9 = GPIO_DIR_INPUT,
	.gpio10 = GPIO_DIR_INPUT,
	.gpio12 = GPIO_DIR_OUTPUT,
	.gpio13 = GPIO_DIR_INPUT,
	.gpio14 = GPIO_DIR_INPUT,
	.gpio15 = GPIO_DIR_INPUT,
	.gpio16 = GPIO_DIR_OUTPUT,
	.gpio17 = GPIO_DIR_OUTPUT,
	.gpio18 = GPIO_DIR_OUTPUT,
	.gpio19 = GPIO_DIR_INPUT,
	.gpio20 = GPIO_DIR_OUTPUT,
	.gpio24 = GPIO_DIR_OUTPUT,
	.gpio25 = GPIO_DIR_OUTPUT,
	.gpio26 = GPIO_DIR_OUTPUT,
	.gpio27 = GPIO_DIR_OUTPUT,
	.gpio28 = GPIO_DIR_OUTPUT,
};

static const struct pch_gpio_set1 pch_gpio_set1_level = {
	.gpio12 = GPIO_LEVEL_LOW,
	.gpio16 = GPIO_LEVEL_HIGH,
	.gpio17 = GPIO_LEVEL_HIGH,
	.gpio18 = GPIO_LEVEL_HIGH,
	.gpio20 = GPIO_LEVEL_HIGH,
	.gpio24 = GPIO_LEVEL_HIGH,
	.gpio25 = GPIO_LEVEL_HIGH,
	.gpio26 = GPIO_LEVEL_LOW,
	.gpio27 = GPIO_LEVEL_HIGH,
	.gpio28 = GPIO_LEVEL_LOW,
};

static const struct pch_gpio_set1 pch_gpio_set1_invert = {
	.gpio7 = GPIO_INVERT,
	.gpio8 = GPIO_INVERT,
	.gpio13 = GPIO_INVERT,
};

static const struct pch_gpio_set1 pch_gpio_set1_blink = {
};

static const struct pch_gpio_set2 pch_gpio_set2_mode = {
	.gpio32 = GPIO_MODE_GPIO,
	.gpio33 = GPIO_MODE_GPIO,
	.gpio34 = GPIO_MODE_GPIO,
	.gpio35 = GPIO_MODE_GPIO,
	.gpio36 = GPIO_MODE_GPIO,
	.gpio37 = GPIO_MODE_GPIO,
	.gpio38 = GPIO_MODE_GPIO,
	.gpio39 = GPIO_MODE_GPIO,
	.gpio48 = GPIO_MODE_GPIO,
};

static const struct pch_gpio_set2 pch_gpio_set2_direction = {
	.gpio32 = GPIO_DIR_OUTPUT,
	.gpio33 = GPIO_DIR_OUTPUT,
	.gpio34 = GPIO_DIR_OUTPUT,
	.gpio35 = GPIO_DIR_OUTPUT,
	.gpio36 = GPIO_DIR_OUTPUT,
	.gpio37 = GPIO_DIR_INPUT,
	.gpio38 = GPIO_DIR_OUTPUT,
	.gpio39 = GPIO_DIR_OUTPUT,
	.gpio48 = GPIO_DIR_OUTPUT,
};

static const struct pch_gpio_set2 pch_gpio_set2_level = {
	.gpio32 = GPIO_LEVEL_HIGH,
	.gpio33 = GPIO_LEVEL_LOW,
	.gpio34 = GPIO_LEVEL_HIGH,
	.gpio35 = GPIO_LEVEL_LOW,
	.gpio36 = GPIO_LEVEL_HIGH,
	.gpio38 = GPIO_LEVEL_LOW,
	.gpio39 = GPIO_LEVEL_LOW,
	.gpio48 = GPIO_LEVEL_HIGH,
};

const struct pch_gpio_map mainboard_gpio_map = {
	.set1 = {
		.mode		= &pch_gpio_set1_mode,
		.direction	= &pch_gpio_set1_direction,
		.level		= &pch_gpio_set1_level,
		.blink		= &pch_gpio_set1_blink,
		.invert		= &pch_gpio_set1_invert,
	},
	.set2 = {
		.mode		= &pch_gpio_set2_mode,
		.direction	= &pch_gpio_set2_direction,
		.level		= &pch_gpio_set2_level,
	},
};
