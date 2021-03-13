#include <iostream>
#include <cstdint>
int main(int, char **)
{
    // C-style POD array.
    int32_t data[100];
    // Make a pointer to the head address of the array.
    int32_t * pdata = data;
    // Make another pointer to the 50-th element from the head of the array.
    int32_t * odata = pdata + 50;
    // Initialize the array.
    for (size_t it=0; it<100; ++it) { data[it] = it + 5000; }
    std::cout << "data[10]: " << data[10] << std::endl;
    std::cout << "pdata[10]: " << pdata[10] << std::endl;
    std::cout << "*(data+20): " << *(data+20) << std::endl;
    std::cout << "*(pdata+20): " << *(pdata+20) << std::endl;
    std::cout << "data[50]: " << data[50] << std::endl;
    std::cout << "odata[0]: " << odata[0] << std::endl;
    std::cout << "data[40]: " << data[40] << std::endl;
    std::cout << "odata[-10]: " << odata[-10] << std::endl;
    std::cout << "*(data+40): " << *(data+40) << std::endl;
    std::cout << "*(odata-10): " << *(odata-10) << std::endl;
    return 0;
}
// vim: set sw=4 ts=4 sts=4 et:
