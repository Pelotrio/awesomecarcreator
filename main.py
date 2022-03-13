from PIL import Image, ImageEnhance
import io


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
