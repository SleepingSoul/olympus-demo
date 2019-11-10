#include <stdafx.h>

int main(int argc, char** argv)
{
    std::puts(oly::String{"olympus"}.data());

    oly::Array arr{ 1, 2, 3, 4, 5 };
    oly::Vector vec{ 1, 2, 3, 4, 5 };

    oly::Logger logger("hello.txt");
    logger.info("Hello");
    logger.info("WHADDA FUCK IS THAT %f", 3.1415);
    logger.error("Fuck, error %d %s", 244, "lol");

    if (!estd::contains(arr, 2))
    {
        std::terminate();
    }

    return 0;
}
