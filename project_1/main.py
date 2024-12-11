import cv2
import numpy as np
import argparse
import os

from utils import ocr, generate_image


def main():
    parser = argparse.ArgumentParser(description="OCR - optical character recognition.")
    parser.add_argument("--image_path", help="Image path.")
    parser.add_argument("--font_path", help="Font path.", default="/mnt/c/Windows/Fonts/arialbd.ttf")
    parser.add_argument("--font_size", help="Font size (default 50).", type=int, default=50)
    parser.add_argument("--characters", help="Characters used for pattern generation", default=generate_image.TEMPLATE_CHARACTERS)
    parser.add_argument("--output_path", help="Output image path.", default="output.png")
    args = parser.parse_args()

    template_image = generate_image.generate_template_image(font_path=args.font_path, font_size=args.font_size, output_path="template.png", characters=args.characters)
    template_features, template_image = ocr.extract_features(image_path="template.png", font_size=args.font_size)

    patterns = {}
    for count, character in enumerate(args.characters):
        patterns[character] = template_features[count]

        # if not os.path.exists('characters'):
        #     os.makedirs('characters')

        # if character.isupper():
        #     chatacter_output_file = f'characters/{character}_upper.png'
        # else:
        #     chatacter_output_file = f'characters/{character}.png'
        # cv2.imwrite(chatacter_output_file, patterns[character])

    image_features, image = ocr.extract_features(image_path=args.image_path, font_size=args.font_size)

    # _, binary, _ = ocr.preprocess_image(args.image_path)
    # cv2.imwrite('binary.png', binary)

    # for count, image_feature in enumerate(image_features):
    #     if not os.path.exists('image_features'):
    #         os.makedirs('image_features')

    #     image_feature_output_file = f'image_features/{count}.png'
    #     cv2.imwrite(image_feature_output_file, image_feature)

    recognized_text = ocr.recognize_text(image_features, patterns)
    print("Recognized text:", recognized_text)

    cv2.imwrite(args.output_path, image)
    print(f"Output image with contours saved to: {args.output_path}")

if __name__ == "__main__":
    main()
