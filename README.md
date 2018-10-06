# Pixelator

Simple program to take an image and pixelate it.

It works by taking the average colour around an area, and then filling that area with that colour

## Building

Requires SFML library

On Ubuntu:

```
sudo apt-get install libsfml-dev
```

Building:

```
git clone https://github.com/Hopson97/Pixelator.git
cd Pixelator
cmake .
make
```

## Running

```
./pixelator <image-file> <pixel-size>
```

The pixel size must be a number divisible exactly by both the image width and height

## Example

```
./pixelator monalisa.jpg 12
```

Output: 

![MonaLisaPixel](https://i.imgur.com/w2MMtSv.png).md
