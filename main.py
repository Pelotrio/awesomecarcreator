from PIL import Image, ImageEnhance
import subprocess
import colorsys
import numpy
import json
import io
import os



def generate_video(image, audio_filename, output_filename, duration = 10, seek_to = "00:00:00", audio_rate = 16000, video_bitrate = "500k", max_filesize = "8M", framerate = 7):
    image_buffer = io.BytesIO()
    image.save(image_buffer, "png")
    image_buffer.seek(0)

    #ffmpeg
    command  = f"ffmpeg -y -hide_banner -loglevel error" + " "

    #image
    command += f"-r {framerate} -loop 1 -i cache:pipe:" + " "

    #audio
    command += f"-ss {seek_to} -i {audio_filename} -ar {audio_rate}" + " "

    #out file stuff
    command += f"-r {framerate} -vf \"scale=720:720\" -b:v {video_bitrate} -fs {max_filesize} -t {duration} -movflags +faststart {output_filename}"

    _, err = subprocess.Popen(command, stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE).communicate(input = image_buffer.read())

    if err != b"":
        raise Exception(f"Unknown error, stderr: {err}")

def jpegify(image, power = 92, repetitions = 50):
    out = image.convert("RGB")
    for i in range(repetitions):
        buffer = io.BytesIO()
        out.save(buffer, "jpeg", quality = 100 - power)
        buffer.seek(0)
        out = Image.open(buffer)
    return out

def apply_color_overlay(image, overlay, grayscale = True, alpha = 0.5):
    overlay = overlay.convert("RGBA")
    
    if grayscale:
        image = image.convert("L")

    image = image.convert("RGB")
    
    overlay.putalpha(ImageEnhance.Brightness(overlay.split()[3]).enhance(alpha))
    return Image.composite(overlay, image, overlay)

def put_watermark(image, watermark, pos):
    image.paste(watermark, pos, watermark)
    return image

def put_logo(image, logo, pos, size, angle = 0):
    logo = logo.resize(size)
    logo = logo.rotate(-angle, expand = True)
    #TODO: more magic
    image.paste(logo, pos, logo)
    return image

def change_color(image, color):
    image = image.convert("RGB")
    image_array = numpy.array(numpy.asarray(image).astype('float'))

    image_rgb = numpy.rollaxis(image_array, axis = -1)
    image_h, image_s, image_v = numpy.vectorize(colorsys.rgb_to_hsv)(*image_rgb)

    color_h, _, _ = colorsys.rgb_to_hsv(color[0], color[1], color[2])

    image_h = color_h
    image_rgb = numpy.vectorize(colorsys.hsv_to_rgb)(image_h, image_s, image_v)

    out_array = numpy.dstack(image_rgb)

    return Image.fromarray(out_array.astype("uint8"), "RGB")

def colorize_with_image(image, overlay):
    image = image.convert("RGB")
    overlay = overlay.convert("RGB")

    image_array = numpy.array(numpy.asarray(image).astype('float'))
    overlay_array = numpy.array(numpy.asarray(overlay).astype('float'))

    image_rgb = numpy.rollaxis(image_array, axis = -1)
    image_h, image_s, image_v = numpy.vectorize(colorsys.rgb_to_hsv)(*image_rgb)

    overlay_rgb = numpy.rollaxis(overlay_array, axis = -1)
    overlay_h, overlay_s, _ = numpy.vectorize(colorsys.rgb_to_hsv)(*overlay_rgb)

    image_h = overlay_h
    image_s = overlay_s * image_s

    image_rgb = numpy.vectorize(colorsys.hsv_to_rgb)(image_h, image_s, image_v)

    out_array = numpy.dstack(image_rgb)

    return Image.fromarray(out_array.astype("uint8"), "RGB")

def ruin_resolution(image, power = 3):
    return image.resize((int(image.size[0] / 2**power), int(image.size[1] / 2**power))).resize((image.size[0], image.size[1]))



template_name = "template"

with open(f"resources{os.sep}{template_name}.json") as file:
    template_data = json.load(file)

template = Image.open(f"resources{os.sep}{template_data['image']}")

template = change_color(template, (255, 0, 127))

template = jpegify(template)
template = ruin_resolution(template)

template.show()