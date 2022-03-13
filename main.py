import io
import subprocess
from PIL import Image, ImageEnhance


# Creates a video from a single image
def create_video(image_path, video_path, audio_path, duration):
    out, err = subprocess.Popen(
        ["ffmpeg", "-y", "-loop", "1", "-i", image_path, "-c:v", "libx264", "-t", str(duration), "-tune", "stillimage",
         "-pix_fmt", "yuv420p", "-c:a", "aac", "-b:a", "192k", "-vf", "scale=720:720", "-shortest", video_path, "-i",
         audio_path],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE).communicate()


def jpegify(image, power=92, repetitions=50):
    out = image.convert("RGB")
    for i in range(repetitions):
        buffer = io.BytesIO()
        out.save(buffer, "jpeg", quality=100 - power)
        buffer.seek(0)
        out = Image.open(buffer)
    return out


def apply_color(template, image, template_to_grayscale=True):
    template = template.convert("RGB")
    image = image

    if template_to_grayscale:
        template = template.convert("L").convert("RGB")

    image.putalpha(ImageEnhance.Brightness(image.split()[3]).enhance(0.5))
    return Image.composite(image, template, image)
