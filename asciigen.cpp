#include <iostream>
#include <string>
#include <stdint.h>
#include <Magick++.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

void getTermSize(int &cols, int &rows)
{
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    cols = w.ws_col;
    rows = w.ws_row;
}

/* TODO - remove resized image after running (during cleanup)
 *      - research if theres a possiblility to make scaling dynamic based on image resolution
*/
int main(int argc, char **argv)
{
    // Taking path from argument vector
    std::string path {};

    if (argc == 2) {
        path = argv[1];
    } else {
        std::cerr << "Check if you entered path to image!" << std::endl;
        return 1;
    }

    // Variables
    std::string ascii {"`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"};
    const int asciiNum = ascii.size();

    unsigned int width {0}, height {0};

    // Importing an image
    Magick::InitializeMagick(*argv);
    Magick::Image image;

    image.read(path);

    width = image.columns();
    height = image.rows();

    // Resizing based on terminal & image
    int termWidth = 0, termHeight = 0;
    getTermSize(termWidth, termHeight);

    size_t asciiWidth = termWidth - 10;
    size_t asciiHeight = termHeight - 5;

    double widthScale = static_cast<double>(image.columns() / asciiWidth);
    double heightScale = static_cast<double>(image.rows() / asciiHeight);
    double scalingFactor = std::min(widthScale, heightScale);

    size_t newWidth = static_cast<size_t>(image.columns() / scalingFactor);
    size_t newHeight = static_cast<size_t>(image.rows() / scalingFactor);

    image.resize(Magick::Geometry(newWidth, newHeight));
    image.write("./resized.jpg");

    Magick::Image resizedImage;
    resizedImage.read("./resized.jpg");

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
            // prints a char 3 times to improve clarity,
            // but probably at cost of performance (easier to append)
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

    try {
        std::remove("./resized.jpg") == 0;
    } catch (...) {
        std::perror("Error removing resized image file.\n");
    }

    return 0;
}
