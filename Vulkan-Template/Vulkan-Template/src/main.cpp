#include "VK/MainVulkan.hpp"
#undef main

int main (int argc, char *argv[])
{
    MainVulkan mainVulkan{};
    mainVulkan.run();
    return 0;
}