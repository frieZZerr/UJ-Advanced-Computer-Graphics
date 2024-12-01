import cv2
import numpy as np
import argparse


"""
Loads, converts to gray scale and binarizes the image.
Args:
    image_path (str): Path to the image
Returns:
    image: cv2.imread array
    binary: cv2.threshold array
Raises:
    FileNotFoundError: If the image was not found
"""
def preprocess_image(image_path):
    image = cv2.imread(image_path)
    if image is None:
        raise FileNotFoundError(f"File not found: {image_path}")
    
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, binary = cv2.threshold(gray, 128, 255, cv2.THRESH_BINARY_INV)
    return image, binary

"""
Finds the contours and draws rectangles around detected characters
Args:
    image (array): cv2.imread array
    binary_image (array): cv2.threshold array
Returns:
    contours: cv2.findContours
"""
def find_and_draw_contours(image, binary_image):
    contours, _ = cv2.findContours(binary_image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)
    return contours

"""
Extracts features for every contour and scaling them to font_size
Args:
    contours (array): cv2.findContours array
    binary_image (array): cv2.threshold array
    font_size (int): Scales image to specified size
Returns:
    features: array
"""
def extract_features(contours, binary_image, font_size=50):
    features = []
    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        char = binary_image[y:y+h, x:x+w]
        char_resized = cv2.resize(char, (font_size, font_size))
        features.append(char_resized.flatten())
    return features

"""
Recognizes text by comparing pattern features
Args:
    features (array): Features array
    patterns (array): Patterns array
Returns:
    recognized_text: Recognized text
"""
def recognize_text(features, patterns):
    recognized_text = ""
    for feature in features:
        best_match = None
        best_score = float('inf')
        for char, pattern in patterns.items():
            score = np.linalg.norm(feature - pattern)
            if score < best_score:
                best_match = char
                best_score = score
        recognized_text += best_match
    return recognized_text

def main():
    parser = argparse.ArgumentParser(description="OCR - optical character recognition.")
    parser.add_argument("--image_path", help="Ścieżka do pliku obrazu.")
    parser.add_argument("--output_path", help="Ścieżka do pliku wynikowego obrazu.", default="output.png")
    args = parser.parse_args()

    image, binary = preprocess_image(args.image_path)

    contours = find_and_draw_contours(image, binary)

    features = extract_features(contours, binary)

    # Wzorce - przykładowo uzupełnione danymi
    # patterns = {
    #     "0": np.zeros(100),  # Przykladowe wektory cech
    #     "1": np.ones(100),
    #     # Dodaj resztę wzorców
    # }

    # # Rozpoznawanie tekstu
    # recognized_text = recognize_text(features, patterns)
    # print("Rozpoznany tekst:", recognized_text)

    # Zapis obrazu z zaznaczonymi konturami
    cv2.imwrite(args.output_path, image)
    print(f"Obraz z zaznaczonymi konturami zapisany do: {args.output_path}")

if __name__ == "__main__":
    main()
