#include "utility.hpp"

namespace utility
{
	void jpegify(Magick::Image& image, uint8_t power, uint8_t repetitions)
	{
		Magick::Blob temp;
		for (uint8_t i = repetitions; i > 0; i--)
		{
			image.magick("JPEG");
			image.quality(100 - power);

			image.write(&temp);
			image = Magick::Image(temp);
		}
	}

	void apply_color_overlay(Magick::Image& image, Magick::Image overlay, bool grayscale, uint16_t opaque)
	{
		Magick::ColorspaceType original_colorspace(image.colorSpace());
		if (grayscale)
			image.colorSpace(Magick::ColorspaceType::GRAYColorspace);

		Magick::Geometry size = image.size();
		overlay.resize(size);
		overlay.opacity(opaque);
		image.composite(overlay, Magick::GravityType::CenterGravity, Magick::OverCompositeOp);

		image.colorSpace(original_colorspace);
	}

	void paste_image(Magick::Image& image, Magick::Image logo, Magick::Geometry place, double_t angle)
	{
		logo.scale(place);
		logo.backgroundColor(Magick::Color(MaxRGB, MaxRGB, MaxRGB, MaxRGB));
		logo.rotate(angle);
		image.composite(logo, place, Magick::OverCompositeOp);
	}

	void change_color(Magick::Image& image, Magick::Color color)
	{
		Magick::ColorspaceType original_colorspace(image.colorSpace());
		image.colorSpace(Magick::ColorspaceType::HSLColorspace);

		Magick::Quantum hue = Magick::ColorHSL(color).hue() * MaxRGB;

		Magick::Geometry size = image.size();
		Magick::PixelPacket* pixels = image.getPixels(0, 0, size.width(), size.height());
		uint32_t pixel_amount = size.width() * size.height();

		for (uint32_t i = 0; i < pixel_amount; i++)
			pixels[i].red = hue;

		image.syncPixels();

		image.colorSpace(original_colorspace);
	}

	void colorize_with_image(Magick::Image& image, Magick::Image overlay, Magick::Coordinate position)
	{
		Magick::ColorspaceType original_colorspace(image.colorSpace());
		image.colorSpace(Magick::ColorspaceType::HSLColorspace);

		overlay.colorSpace(Magick::ColorspaceType::HSLColorspace);

		Magick::Geometry image_size = image.size();
		Magick::Geometry overlay_size = overlay.size();

		Magick::PixelPacket* image_pixels = image.getPixels(position.x(), position.y(), overlay_size.width(), overlay_size.height());
		Magick::PixelPacket* overlay_pixels = overlay.getPixels(0, 0, overlay_size.width(), overlay_size.height());

		uint32_t pixel_amount = overlay_size.width() * overlay_size.height();

		for (uint32_t i = 0; i < pixel_amount; i++)
		{
			image_pixels[i].red = overlay_pixels[i].red;
			image_pixels[i].green *= overlay_pixels[i].green;
		}
		image.syncPixels();

		image.colorSpace(original_colorspace);
	}

	void ruin_resolution(Magick::Image& image, uint8_t power)
	{
		Magick::Geometry original_size = image.size();
		Magick::Geometry size = original_size;

		size.width(size.width() / pow(2, power));
		size.height(size.height() / pow(2, power));

		image.resize(size);
		image.resize(original_size);
	}
}