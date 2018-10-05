#include <iostream>
#include <SFML/Graphics.hpp>

bool isValidSize(unsigned size, unsigned pixelSize, const char* desc) {
    if (size % pixelSize != 0) {
        std::cout << "ERROR: Image " << desc << " is not divisible by pixel size.\n";
        std::cout << desc << " is " << size << '\n';
        std::cout << "Division result: " << size % pixelSize << "\n";
        return false;
    }
    return true;
}

int main(int argc, char** argv) {
    std::string fileName;
    unsigned pixelSize = 0;
    if (argc > 2) {
        fileName = argv[1];
        std::string pxString = argv[2];
        pixelSize = std::stoi(pxString); //TODO Error check
    }
    else {
        std::cout << "Too few arguments.\nUsage: \n";
        std::cout << "pixelator <image-file-path> <pixel-size>\n";
        //std::cout << "pixelator <image-file-path> <pixel-size> <pixelImage>\n";
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

    unsigned width  = image.getSize().x;
    unsigned height = image.getSize().y;

    std::cout << "Creating new image...\n";
    sf::Image newImage;
    newImage.create(width, height);

    std::vector<sf::Color> colours;
    for (unsigned y = 0; y < height / pixelSize; y++) {
        for (unsigned x = 0; x < width / pixelSize; x++) {
            colours.clear();
            for (unsigned oy = 0; oy < pixelSize; oy++) {
                for (unsigned ox = 0; ox < pixelSize; ox++) {
                    unsigned localX = ox + x * pixelSize;
                    unsigned localY = oy + y * pixelSize;
                    colours.push_back(image.getPixel(localX, localY));
                }
            }

            unsigned totalRed   = 0;
            unsigned totalGreen = 0;
            unsigned totalBlue  = 0;
            for (auto c : colours) {
                totalRed    += c.r;
                totalGreen  += c.g;
                totalBlue   += c.b;
            }
            auto total = colours.size();
            sf::Color avgColour{
                uint8_t(totalRed / total), 
                uint8_t(totalGreen / total), 
                uint8_t(totalBlue / total)};

            for (unsigned oy = 0; oy < pixelSize; oy++) {
                for (unsigned ox = 0; ox < pixelSize; ox++) {
                    unsigned localX = ox + x * pixelSize;
                    unsigned localY = oy + y * pixelSize;
                    newImage.setPixel(localX, localY, avgColour);
                }
            }
        }
    } 
    std::cout << "Saving image...\n";
    newImage.saveToFile("out.jpg");
    std::cout << "Complete!\n"; 
}