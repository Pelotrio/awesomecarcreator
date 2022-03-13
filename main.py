import io
import os
import subprocess
from PIL import Image, ImageEnhance


def generate_video(image, audio_filename, output_filename, duration = 10, seek_to = "00:00:00", audio_rate = 16000, video_bitrate = "200k", max_filesize = "8M"):
    image_buffer = io.BytesIO()
    image.save(image_buffer, "png")
    image_buffer.seek(0)

    #ffmpeg
    command  = f"ffmpeg -y -hide_banner -loglevel error" + " "

    #image
    command += f"-r 15 -loop 1 -i -" + " "

    #audio
    command += f"-ss {seek_to} -i {audio_filename} -ar {audio_rate}" + " "

    #out file stuff
    command += f"-vf \"scale=720:720\" -b:v {video_bitrate} -t {duration} -fs {max_filesize} -r 15 {output_filename}"

    out, err = subprocess.Popen(command, stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE).communicate(input = image_buffer.read())

    #print(err)
    #TODO: maybe handle errors


def jpegify(image, power=92, repetitions=50):
    out = image.convert("RGB")
    for i in range(repetitions):
        buffer = io.BytesIO()
        out.save(buffer, "jpeg", quality=100 - power)
        buffer.seek(0)
        out = Image.open(buffer)
    return out


def apply_color_overlay(image, overlay):
    image = image.convert("L").convert("RGB")
    
    overlay.putalpha(ImageEnhance.Brightness(overlay.split()[3]).enhance(0.5))
    return Image.composite(overlay, image, overlay)

def apply_watermark(image, watermark):
    image.paste(watermark, (0, 0), watermark)
    return image


template = Image.open(f"resources{os.sep}template.png")
overlay = Image.open("overlay.png")
watermark = Image.open("watermark.png")

template = apply_color_overlay(template, overlay)
template = apply_watermark(template, watermark)
template = jpegify(template)

generate_video(template, "audio.mp3", "out.mp4")