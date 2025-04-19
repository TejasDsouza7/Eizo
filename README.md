# Eizo

**Eizo** is a lightweight cross-platform C++ tool for converting image files to Base64 or Hexadecimal format, and decoding them back to images. It supports most common image formats and works on Windows, Linux, Termux (Android), and macOS.

## Features

- Convert image files to Base64
- Convert image files to Hex
- Decode Base64 or Hex back into image files
- Cross-platform terminal interface
- Automatic output file naming

## Supported Image Formats

- `.jpg`
- `.jpeg`
- `.png`
- `.bmp`
- `.gif`
- `.webp`

## Usage

1. **Build the project** using any C++17+ compiler:
   ```bash
   g++ -std=c++17 -o Eizo Eizo.cpp
   ```

2. **Run the Tool**
   ```bash
   ./Eizo
   ```

## Example

### Encode an Image to Base64

1. Run the program and select option `1`:
2. Enter the image file name (e.g., `photo.jpg`).
3. The program creates a text file named: photo_base64.txt

### Decode Base64 Back to Image

1. Run the program again and select option `3`:
2. Enter the file name:photo_base64.txt
3. The program creates an output image:photo_from_base64.png

### Same Flow Applies to Hex

- Option `2`: Encode image to Hex → Output: `photo_hex.txt`
- Option `4`: Decode Hex to image → Output: `photo_from_hex.png`
