#pragma once

#include <Magick++.h>

namespace utility
{
	void jpegify(Magick::Image& image, int power = 92, int repetitions = 50);
	void apply_color_overlay(Magick::Image& image, bool grayscale = true, float alpha = 0.5f);
	void paste_image(Magick::Image& image, Magick::Image logo, double x, double y, double w, double h, double angle = 0);
	void change_color(Magick::Image& image, Magick::Color color);
	void colorize_with_image(Magick::Image& image, Magick::Image overlay);
	void ruin_resolution(Magick::Image& image, int power = 3);
}