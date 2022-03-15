#include "utility.hpp"

namespace utility
{
	void jpegify(Magick::Image& image, int power = 92, int repetitions = 50)
	{
		Magick::Blob temp;
		for (; repetitions > 0; repetitions--)
		{
			image.magick("JPEG");
			image.quality(100 - power);

			image.write(&temp);
			image = Magick::Image(temp);
		}
	}

	void apply_color_overlay(Magick::Image& image, bool grayscale = true, float alpha = 0.5f)
	{
		image.draw(Magick::DrawableText(0, 0, "WIP"));
	}

	void paste_image(Magick::Image& image, Magick::Image logo, double x, double y, double w, double h, double angle = 0)
	{
		image.draw(Magick::DrawableText(0, 0, "WIP"));
	}

	void change_color(Magick::Image& image, Magick::Color color)
	{
		image.draw(Magick::DrawableText(0, 0, "WIP"));
	}

	void colorize_with_image(Magick::Image& image, Magick::Image overlay)
	{
		image.draw(Magick::DrawableText(0, 0, "WIP"));
	}

	void ruin_resolution(Magick::Image& image, int power = 3)
	{
		image.draw(Magick::DrawableText(0, 0, "WIP"));
	}
}