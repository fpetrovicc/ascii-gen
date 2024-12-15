#include <iostream>
#include <string>
#include <stdint.h>
#include <Magick++.h>

int** convertImgToBrightness(Magick::Image image, int height, int width)
{
    int** newImg = 0;
    newImg = new int*[height];

    for (size_t h = 0; h < height; h++)
    {
        newImg[h] = new int[width];

        for (size_t w = 0; w < width; w++)
        {
            Magick::Color color = image.pixelColor(w, h);

            int red = (color.redQuantum() / 257);
            int green = (color.greenQuantum() / 257);
            int blue = (color.blueQuantum() / 257);

            newImg[h][w] = (0.21 * red) + (0.72 * green) + (0.07 * blue);
        }
    }

    return newImg;
}

/* TODO - add usage of paths and argument vector
 *      - remove resized image after running (during cleanup)
 *      - research if theres a possiblility to make scaling dynamic based on image resolution
*/
int main(int argc, char **argv)
{
    std::string ascii {"`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"};
    const int asciiNum = ascii.size();

    unsigned int width {0}, height {0};
    const int resizeFactor = 3;

    Magick::InitializeMagick(*argv);

    // Importing an image
    Magick::Image image;

    image.read("../res/test.jpg");

    width = image.columns();
    height = image.rows();

    // Resizing the image
    unsigned int newWidth {width / resizeFactor};
    unsigned int newHeight {height / resizeFactor};

    image.resize(Magick::Geometry(newWidth, newHeight));
    image.write("../res/resized.jpg");

    Magick::Image resizedImage;
    resizedImage.read("../res/resized.jpg");

    height = newHeight;
    width = newWidth;

    // Converting image to a 2D array of luminosity values
    int** newImg = convertImgToBrightness(resizedImage, height, width);

    // Mapping luminosity values to ASCII
    auto mapToAscii = [&](int lum) -> char {
        int asciiIndex = static_cast<int>(lum / 255.0 * (asciiNum - 1));
        return ascii[asciiIndex];
    };

    // Printing the ASCII image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            std::cout << mapToAscii(newImg[h][w]);
            std::cout << mapToAscii(newImg[h][w]);
            std::cout << mapToAscii(newImg[h][w]);
        }

        std::cout << "\n";
    }

    // Memory cleanup
    for (int h = 0; h < height; h++)
    {
        delete [] newImg[h];
    }

    delete [] newImg;
    newImg = 0;

    return 0;
}
