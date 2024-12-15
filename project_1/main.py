import cv2
import numpy as np
import argparse
import os

from utils import ocr, generate_image, custom_logger

LOG_LEVELS = custom_logger.LOG_LEVELS
LOGGER = None

def save_image_features(image_path, image_features):
    for count, image_feature in enumerate(image_features):
        if not os.path.exists('image_features'):
            os.makedirs('image_features')

        image_feature_output_file = f'image_features/{count}.png'
        cv2.imwrite(image_feature_output_file, image_feature)

def create_patterns_dict(characters, template_features, log_level):
    patterns = {}
    for count, character in enumerate(characters):
        patterns[character] = template_features[count]

        # Save template characters for preview
        if LOG_LEVELS.get(log_level.upper()) == LOG_LEVELS["DEBUG"]:
            if not os.path.exists('characters'):
                os.makedirs('characters')

            if character.isupper():
                chatacter_output_file = f'characters/{character}_upper.png'
            else:
                chatacter_output_file = f'characters/{character}.png'
            cv2.imwrite(chatacter_output_file, patterns[character])
    
    return patterns

def main():
    parser = argparse.ArgumentParser(description="OCR - optical character recognition.")
    parser.add_argument("--image_path", help="Image path.")
    parser.add_argument("--font_path", help="Font path.", default="/mnt/c/Windows/Fonts/arialbd.ttf")
    parser.add_argument("--font_size", help="Font size (default 50).", type=int, default=50)
    parser.add_argument("--characters", help="Characters used for pattern generation", default=generate_image.TEMPLATE_CHARACTERS)
    parser.add_argument("--output_path", help="Output image path.", default="output.png")
    parser.add_argument('--log_level', help='Main logger level ("info", "debug", "warning", "error", "critical")', type=str, default='info')
    args = parser.parse_args()

    LOGGER = custom_logger.logger_setup(log_level=args.log_level, logger_name="ocr_logger")

    template_image = generate_image.generate_template_image(font_path=args.font_path, font_size=args.font_size, output_path="template.png", characters=args.characters)
    template_features, template_image = ocr.extract_features(image_path="template.png", font_size=args.font_size)

    patterns = create_patterns_dict(characters=args.characters, template_features=template_features, log_level=args.log_level)

    image_features, image = ocr.extract_features(image_path=args.image_path, font_size=args.font_size)

    # Save binary input image for preview
    if LOG_LEVELS.get(args.log_level.upper()) == LOG_LEVELS["DEBUG"]:
        save_image_features(image_path=args.image_path, image_features=image_features)

    recognized_text = ocr.recognize_text(image_features, patterns)
    LOGGER.info(f"Recognized text: {recognized_text}")

    cv2.imwrite(args.output_path, image)
    LOGGER.debug(f"Output image with contours saved to: {args.output_path}")

if __name__ == "__main__":
    main()
