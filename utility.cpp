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

	//WIP
	void generate_video(Magick::Image image, uint8_t fps)
	{
		image.colorSpace(Magick::ColorspaceType::RGBColorspace);

		Magick::Geometry size = image.size();
		Magick::PixelPacket* image_pixels = image.getPixels(0, 0, size.width(), size.height());

		ffmpeg::SwsContext* sws_context = ffmpeg::sws_getContext(size.width(), size.height(), ffmpeg::AV_PIX_FMT_RGB24, size.width(), size.height(), ffmpeg::AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

		uint8_t* input_pixels[4];
		int input_linesize[4];
		int input_byte_size = ffmpeg::av_image_alloc(input_pixels, input_linesize, size.width(), size.height(), ffmpeg::AV_PIX_FMT_RGB24, 1);

		uint32_t n = 0;
		for (uint32_t i = 0; i < input_byte_size; i += 3, n++)
		{
			input_pixels[0][i] = image_pixels[n].red;
			input_pixels[0][i + 1] = image_pixels[n].green;
			input_pixels[0][i + 2] = image_pixels[n].blue;
		}

		uint8_t* output_pixels[4];
		int output_linesize[4];
		int output_byte_size = ffmpeg::av_image_alloc(output_pixels, output_linesize, size.width(), size.height(), ffmpeg::AV_PIX_FMT_YUV420P, 1);

		ffmpeg::sws_scale(sws_context, input_pixels, input_linesize, 0, size.height(), output_pixels, output_linesize);

		/*====================================================================================================*/

		{
			FILE* dst_file = fopen(R"(C:\rawimage)", "wb");
			if (!dst_file) {
				fprintf(stderr, "Could not open destination file %s\n", R"(C:\rawimage)");
				exit(1);
			}

			fwrite(output_pixels[0], 1, output_byte_size, dst_file);

			fprintf(stderr, "Scaling succeeded. Play the output file with the command:\n"
				"ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",
				ffmpeg::av_get_pix_fmt_name(ffmpeg::AV_PIX_FMT_YUV420P), size.width(), size.height(), R"(C:\rawimage)");

			fclose(dst_file);
		}

		/*====================================================================================================*/

		ffmpeg::av_freep(&input_pixels[0]);
		ffmpeg::av_freep(&output_pixels[0]);

		ffmpeg::sws_freeContext(sws_context);
	}
}
