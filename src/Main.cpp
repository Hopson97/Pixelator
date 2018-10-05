#include <iostream>
#include <SFML/Graphics.hpp>

bool isValidSize(unsigned size, unsigned pixelSize, const char* desc) {
    if (size % pixelSize != 0) {
        std::cout << "ERROR: Image " << desc << " is not divisible by pixel size.\n";
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    std::string fileName;
    int pixelSize = 0;
    if (argc > 2) {
        std::string fileName = argv[1];
        std::string pxString = argv[2];
        pixelSize = std::stoi(pxString); //TODO Error check
    }
    else {
        std::cout << "Usage: \n";
        std::cout << "pixelator <image-file-path> <pixel-size>";
        return 0;
    }

    std::cout << "Loading image...\n";
    sf::Image image;
    image.loadFromFile(fileName);

    std::cout << "Validating image size to pixel size ratio...\n";    
    if (!isValidSize(image.getSize().x, pixelSize, "width") || 
        !isValidSize(image.getSize().y, pixelSize, "height")) {
            return -1;
        }
}