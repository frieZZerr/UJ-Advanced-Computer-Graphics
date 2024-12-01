import cv2
import numpy as np
from PIL import Image, ImageDraw, ImageFont
import argparse


"""
Generates image with specified characters
Args:
    font_path (str): Path to the font
    font_size (str): Size of the font
    output_path (str): Output image path
    characters (str): Characters printed on the output image
Returns:
    binary_image: cv2.threshold array
Raises:
    OSError: If there was an error when loading font
"""
def generate_characters_image(font_path, font_size, output_path, characters="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"):
    character_spacing = font_size // 2
    image_width = (len(characters) * font_size) + ((len(characters) - 1) * character_spacing)
    image_height = font_size * 2  # Bonus margin

    image = Image.new("L", (image_width, image_height), color="white")
    draw = ImageDraw.Draw(image)

    try:
        font = ImageFont.truetype(font_path, font_size)
    except OSError:
        print(f"Error loading font: {font_path}")
        return

    x_offset = 0
    for character in characters:
        draw.text((x_offset, font_size // 4), character, fill=0, font=font)
        x_offset += font_size + character_spacing

    binary_image = np.array(image)
    _, binary_image = cv2.threshold(binary_image, 128, 255, cv2.THRESH_BINARY)

    cv2.imwrite(output_path, binary_image)
    print(f"Saved output file to {output_path}")

    return binary_image


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generate character template with specified font.")
    parser.add_argument("--font_path", help="Font path.", default="/mnt/c/Windows/Fonts/arialbd.ttf")
    parser.add_argument("--font_size", help="Font size (default 50).", type=int, default=50)
    parser.add_argument("--characters", help="Characters used for pattern generation", default="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
    parser.add_argument("--output_path", help="Output image path.", default="characters.png")

    args = parser.parse_args()

    generate_characters_image(font_path=args.font_path, font_size=args.font_size, characters=args.characters, output_path=args.output_path)

