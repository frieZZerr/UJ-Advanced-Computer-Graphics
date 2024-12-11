import cv2
import numpy as np
import argparse


"""
Check if two rectangles are close within threshold
Args:
    rect1 (arr): Rectangle position
    rect2 (arr): Rectangle position
    threshold (int): Maximum distance between rectangles for merge
Returns:
    rectangles_are_close: bool
"""
def rectangles_are_close(rect1, rect2, threshold):
    x1, y1, x2, y2 = rect1
    x3, y3, x4, y4 = rect2
    rectangles_are_close = (x1 - threshold < x4 and x3 - threshold < x2) and (y1 - threshold < y4 and y3 - threshold < y2)

    return rectangles_are_close


"""
Load and preprocess the image: convert to grayscale and apply binary threshold
Args:
    image_path (str): Path to the input image
Returns:
    image: array
    binary: array
Raises:
    FileNotFoundError: If the image was not found
"""
def preprocess_image(image_path, min_width=1200, min_height=720):
    image = cv2.imread(image_path)
    if image is None:
        raise FileNotFoundError(f"File not found: {image_path}")

    height, width = image.shape[:2]
    scale_factor = 1
    if width < min_width and height < min_height:
        scale_width = min_width / width
        scale_height = min_height / height
        scale_factor = max(scale_width, scale_height)

        new_width = int(width * scale_factor)
        new_height = int(height * scale_factor)

        image = cv2.resize(image, (new_width, new_height), interpolation=cv2.INTER_NEAREST)

    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    _, binary = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

    return image, binary, scale_factor


"""
Find and merge nearby contours based on the merge_threshold
Args:
    binary (arr): Array of the binary image
    merge_threshold (int): Threshold for merging close countours
Returns:
    merged_rectangles: array
"""
def find_and_merge_contours(binary, merge_threshold):
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
    binary = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)
    contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours_sorted = sorted(contours, key=lambda cnt: cv2.boundingRect(cnt)[0])
    rectangles = [cv2.boundingRect(cnt) for cnt in contours_sorted]

    merged_rectangles = []
    while rectangles:
        rect1 = rectangles.pop(0)
        merged = False
        for i, rect2 in enumerate(merged_rectangles):
            if rectangles_are_close(
                (rect1[0], rect1[1], rect1[0] + rect1[2], rect1[1] + rect1[3]),
                (rect2[0], rect2[1], rect2[2], rect2[3]),
                merge_threshold
            ):
                x1 = min(rect1[0], rect2[0])
                y1 = min(rect1[1], rect2[1])
                x2 = max(rect1[0] + rect1[2], rect2[2])
                y2 = max(rect1[1] + rect1[3], rect2[3])
                merged_rectangles[i] = (x1, y1, x2, y2)
                merged = True
                break
        if not merged:
            merged_rectangles.append(
                (rect1[0], rect1[1], rect1[0] + rect1[2], rect1[1] + rect1[3])
            )

    return merged_rectangles


"""
Extract feature for every contour and scale them to font_size
Args:
    image_path (str): Path to the image
    font_size (int): Scale image to specified size
Returns:
    features: array
"""
def extract_features(image_path, font_size=50):
    # Preprocess the image
    image, binary, scale_factor = preprocess_image(image_path)

    merge_threshold = int(font_size * scale_factor / 5)
    padding_size = int(font_size * scale_factor / 5)

    # Find and merge contours
    merged_rectangles = find_and_merge_contours(binary, merge_threshold)

    # Extract features
    features = []
    for x1, y1, x2, y2 in merged_rectangles:
        char = binary[y1:y2, x1:x2]
        char_resized = cv2.resize(char, (font_size, font_size), interpolation=cv2.INTER_NEAREST)

        padded_size = font_size + 2 * padding_size
        char_resized_with_padding = np.full((padded_size, padded_size), 0, dtype=char_resized.dtype)

        char_resized_with_padding[padding_size:padding_size + font_size, padding_size:padding_size + font_size] = char_resized

        features.append(char_resized_with_padding)
        cv2.rectangle(image, (x1, y1), (x2, y2), (0, 255, 0), 2)

    return features, image


"""
Recognize text by comparing pattern features
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
        best_score = -1
        for char, pattern in patterns.items():
            score = np.max(cv2.matchTemplate(feature, pattern, cv2.TM_CCOEFF_NORMED))
            # print(f'Char: {char} | score: {score}')
            if score > best_score:
                best_match = char
                best_score = score
        # print(f'Best char: {best_match} | best score: {best_score}')
        recognized_text += best_match

    return recognized_text
