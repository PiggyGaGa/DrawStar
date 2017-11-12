#include <iostream>
#include "Star.hpp"
#include <iostream>
#include <thread>
int main()
{
    
    int radius = 200;
    cv::Size imgsize(640, 480);
    cv::Point center(320, 240);
    Star star(center, imgsize, radius);
    std::thread task0(&Star::ImgProducer, star);
    std::thread task1(&Star::ShowPicture, star);
    task0.join();
    task1.join();
    
    star.release();
    return 0;
}