#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <cctype>

namespace fs = std::filesystem;

std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const std::vector<unsigned char>& data) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::vector<unsigned char> base64_decode(const std::string& in) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;
    std::vector<unsigned char> out;
    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) continue;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return out;
}

std::string to_hex(const std::vector<unsigned char>& data) {
    std::stringstream ss;
    ss << std::hex;
    for (unsigned char c : data)
        ss << std::setw(2) << std::setfill('0') << (int)c;
    return ss.str();
}

std::vector<unsigned char> from_hex(const std::string& hex) {
    std::vector<unsigned char> bytes;
    std::string cleaned;
    for (char c : hex) {
        if (std::isxdigit(c)) cleaned += c;
    }

    if (cleaned.length() % 2 != 0) cleaned.pop_back();

    for (size_t i = 0; i < cleaned.length(); i += 2) {
        std::string byteStr = cleaned.substr(i, 2);
        unsigned char byte = (unsigned char) strtol(byteStr.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

bool is_image_file(const std::string& filename) {
    std::string ext = fs::path(filename).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || ext == ".gif" || ext == ".webp";
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_menu() {
    clear_screen();
    std::cout << "Welcome to Eizo\n";
    std::cout << "1. Convert image to Base64\n";
    std::cout << "2. Convert image to Hex\n";
    std::cout << "3. Decode Base64 to Image\n";
    std::cout << "4. Decode Hex to Image\n";
    std::cout << "5. Exit\n";
    std::cout << "Select an option (1-5): ";
}

int main() {
    int choice;

    print_menu();
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 5) return 0;

    std::string fname;
    std::cout << "Enter the file name: ";
    std::getline(std::cin, fname);

    std::string stem = fs::path(fname).stem().string();

    if (choice == 1 || choice == 2) {
        if (!fs::exists(fname) || !is_image_file(fname)) {
            std::cout << "Invalid or missing image file.\n";
            return 1;
        }

        std::ifstream file(fname, std::ios::binary);
        std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), {});
        std::string result;

        if (choice == 1) {
            result = base64_encode(buffer);
            std::ofstream out(stem + "_base64.txt");
            out << result;
        } else {
            result = to_hex(buffer);
            std::ofstream out(stem + "_hex.txt");
            out << result;
        }

        std::cout << "Encoded and saved to file.\n";
    } else if (choice == 3 || choice == 4) {
        if (!fs::exists(fname)) {
            std::cout << "Text file not found.\n";
            return 1;
        }

        std::ifstream file(fname);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string data = buffer.str();

        std::vector<unsigned char> decoded;
        if (choice == 3) {
            decoded = base64_decode(data);
        } else {
            decoded = from_hex(data);
        }

        std::string output_name = stem + "_from_" + (choice == 3 ? "base64" : "hex") + ".png";
        std::ofstream out(output_name, std::ios::binary);
        out.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());

        std::cout << "Image decoded and saved as " << output_name << std::endl;
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
