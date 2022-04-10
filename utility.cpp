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

		//bug workaround, for some reason image is corrupted in .getPixels(...) after resizing, i guess it just points to different location because after calling .syncPixels() image turns black
		Magick::Blob temp;
		image.write(&temp, "RGB");
		image.read(temp, original_size, 0, "RGB");
	}

	ffmpeg::AVFrame* generate_image_frame(Magick::Image image, /*ffmpeg::AVPixelFormat*/ int pixel_format = ffmpeg::AV_PIX_FMT_YUV420P)
	{
		image.colorSpace(Magick::ColorspaceType::RGBColorspace);

		Magick::Geometry size = image.size();

		ffmpeg::AVFrame* input = ffmpeg::av_frame_alloc();
		input->format = ffmpeg::AV_PIX_FMT_GBRP;
		input->width = size.width();
		input->height = size.height();
		ffmpeg::av_frame_get_buffer(input, 1);

		ffmpeg::AVFrame* output = ffmpeg::av_frame_alloc();
		output->format = pixel_format;
		output->width = size.width();
		output->height = size.height();
		ffmpeg::av_frame_get_buffer(output, 0);

		Magick::PixelPacket* image_pixels = image.getPixels(0, 0, size.width(), size.height());

		uint32_t input_frame_pixels = size.width() * size.height();
		for (uint32_t i = 0; i < input_frame_pixels; i++)
		{
			input->data[0][i] = image_pixels[i].green;
			input->data[1][i] = image_pixels[i].blue;
			input->data[2][i] = image_pixels[i].red;
		}

		ffmpeg::SwsContext* sws_context = ffmpeg::sws_getContext(size.width(), size.height(), ffmpeg::AV_PIX_FMT_GBRP, size.width(), size.height(), (ffmpeg::AVPixelFormat)pixel_format, SWS_BICUBIC, NULL, NULL, NULL);
		ffmpeg::sws_scale(sws_context, input->data, input->linesize, 0, input->height, output->data, output->linesize);
		ffmpeg::sws_freeContext(sws_context);

		ffmpeg::av_frame_free(&input);

		return output;
	}

	ffmpeg::AVFrame* generate_audio_frame()
	{
		ffmpeg::AVFrame* output = ffmpeg::av_frame_alloc();

		output->format = ffmpeg::AVSampleFormat::AV_SAMPLE_FMT_DBLP;

		//WIP

		return output;
	}

	void generate_video(Magick::Image image, std::string filename, uint8_t fps)
	{
		//container
		ffmpeg::AVFormatContext* output_context;
		ffmpeg::avformat_alloc_output_context2(&output_context, NULL, "mp4", NULL);

		ffmpeg::AVDictionary* options = 0;
		ffmpeg::av_dict_set(&options, "tune", "stillimage", NULL);
		//ffmpeg::av_dict_set(&options, "", "", NULL);
		//ffmpeg::av_dict_set(&options, "", "", NULL);

		//video codec
		ffmpeg::AVCodec* video_codec = ffmpeg::avcodec_find_encoder(ffmpeg::AVCodecID::AV_CODEC_ID_H264);
		ffmpeg::AVStream* video_stream = ffmpeg::avformat_new_stream(output_context, video_codec);
		ffmpeg::AVCodecContext* video_codec_context = video_stream->codec;

		video_codec_context->pix_fmt = ffmpeg::AVPixelFormat::AV_PIX_FMT_YUV420P;
		video_codec_context->time_base = ffmpeg::AVRational{ 1, fps };
		video_codec_context->width = image.size().width();
		video_codec_context->height = image.size().height();
		video_codec_context->gop_size = 0;
		
		ffmpeg::avcodec_open2(video_codec_context, video_codec, &options);



		////audio codec
		//ffmpeg::AVCodec* audio_codec = ffmpeg::avcodec_find_encoder(ffmpeg::AVCodecID::AV_CODEC_ID_AAC);
		//ffmpeg::AVStream* audio_stream = ffmpeg::avformat_new_stream(output_context, audio_codec);
		//ffmpeg::AVCodecContext* audio_codec_context = audio_stream->codec;
		//
		//audio_codec_context->time_base = ffmpeg::AVRational{1, 48000};
		//audio_codec_context->sample_rate = 48000;
		//
		//ffmpeg::avcodec_open2(audio_codec_context, audio_codec, &options);



		ffmpeg::AVFrame* video_frame = generate_image_frame(image);
		//create AVFrame with audio here



		ffmpeg::avio_open(&output_context->pb, filename.c_str(), AVIO_FLAG_WRITE);
		ffmpeg::avformat_write_header(output_context, &options);

		ffmpeg::AVPacket* video_packet = ffmpeg::av_packet_alloc();

		int frames = 80 + 1;
		for (int i = 0; i < frames; i++)
		{
			//https://stackoverflow.com/questions/43333542/what-is-video-timescale-timebase-or-timestamp-in-ffmpeg //check comments
			video_frame->pts = i * (90000 / fps);

			ffmpeg::avcodec_send_frame(video_codec_context, video_frame);

			if (ffmpeg::avcodec_receive_packet(video_codec_context, video_packet) == -11)
				frames++;

			ffmpeg::av_interleaved_write_frame(output_context, video_packet);
		}

		ffmpeg::av_write_trailer(output_context);
		ffmpeg::avio_closep(&output_context->pb);

		ffmpeg::avformat_free_context(output_context);
	}
}
	