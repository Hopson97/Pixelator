#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <mutex>
#include <algorithm>
#include <random>

namespace {
    std::mutex mutex;

    bool isValidSize(unsigned size, unsigned pixelSize, const char* desc) {
        if (size % pixelSize != 0) {
            std::cout << "ERROR: Image " << desc << " is not divisible by pixel size.\n";
            std::cout << desc << " is " << size << '\n';
            std::cout << "Division result: " << size % pixelSize << "\n";
            return false;
        }
        return true;
    }

    void visualise(const sf::Image& ogImage, const sf::Image& newImage) {
        mutex.lock();
        float w = (float)ogImage.getSize().x;
        float h = (float)ogImage.getSize().y;

        sf::RenderWindow window({(unsigned)w, (unsigned)h}, "Pixelator");
        window.setFramerateLimit(60);
    

        sf::RectangleShape ogImageRect({w, h});
        sf::RectangleShape newImageRect({w, h});

        sf::Texture ogTex;
        ogTex.loadFromImage(ogImage);
        ogImageRect.setTexture(&ogTex);

        sf::Texture newTex;
        newTex.loadFromImage(newImage);
        newImageRect.setTexture(&newTex);
        mutex.unlock();

        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed) window.close();
            }
            window.clear();

            mutex.lock();
            newTex.loadFromImage(newImage);
            mutex.unlock();

            window.draw(ogImageRect);
            window.draw(newImageRect);

            window.display();
        }
    }
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
    newImage.create(width, height, sf::Color::Transparent);

    std::thread visualiseThread([&]() {
        visualise(image, newImage);
    });

    std::vector<sf::Vector2u> pxlPositions;
    for (unsigned y = 0; y < height / pixelSize; y++) {
        for (unsigned x = 0; x < width / pixelSize; x++) {
            pxlPositions.emplace_back(x, y);
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(pxlPositions.begin(), pxlPositions.end(), rng);

    std::vector<sf::Color> colours;
    for (auto& pos : pxlPositions) {
        unsigned x = pos.x;
        unsigned y = pos.y;
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
                mutex.lock();
                newImage.setPixel(localX, localY, avgColour);
                mutex.unlock();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "Saving image...\n";
    newImage.saveToFile("out.jpg");
    std::cout << "Complete!\n"; 
    visualiseThread.join();
}