#pragma once

#include <Magick++.h>

namespace ffmpeg
{
	extern "C"
	{
		#include <libavcodec/avcodec.h>
		#include <libavutil/opt.h>
		#include <libavutil/imgutils.h>
		#include <libswscale/swscale.h>
		#include <libavformat/avformat.h>
		#include <libavutil/timestamp.h>
		#include <libavfilter/avfilter.h>
		#include <libavfilter/buffersink.h>
		#include <libavfilter/buffersrc.h>
		#include <libswresample/swresample.h>
		#include <libavutil/audio_fifo.h>
		#include <libavutil/parseutils.h>
		#include <libavutil/avassert.h>
		#include <libavutil/channel_layout.h>
		#include <libavutil/mathematics.h>
	}
}

namespace utility
{
	void jpegify(Magick::Image& image, uint8_t power = 92, uint8_t repetitions = 50);
	void apply_color_overlay(Magick::Image& image, Magick::Image overlay, bool grayscale = true, uint16_t opaque = MaxRGB / 2);
	void paste_image(Magick::Image& image, Magick::Image logo, Magick::Geometry place, double_t angle = 0);
	void change_color(Magick::Image& image, Magick::Color color);
	void colorize_with_image(Magick::Image& image, Magick::Image overlay, Magick::Coordinate position = Magick::Coordinate(0, 0));
	void ruin_resolution(Magick::Image& image, uint8_t power = 3);
	void generate_video(Magick::Image image, std::string filename, uint8_t fps = 8);
}
