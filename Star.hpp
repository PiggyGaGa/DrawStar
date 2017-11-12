#ifndef Star_H
#define Star_H

#include <opencv2/core.hpp>
#include <vector>
#include <iostream>
#define ROTATEDANGLE 0.4 * 3.1416


using std::cout; using std::cin; using std::endl; 
using std::vector;

class Star
{
public:
    Star(const cv::Point &center, const cv::Size &imgsize, int radius);
    Star();
    void release();
    vector<cv::Point> GenerateFivePoint(const cv::Point &center, int radius);
    void DrawStar(cv::Mat &bg, const vector<cv::Point> &fiveStars);
    void FillColorForStar(cv::Mat &imgStar);
    void ImgProducer();
    void ShowPicture();
private:
    cv::Point RotatedToGeneratePoint(const cv::Point &point1, const cv::Point &center);

private:
    cv::Point center;
    int radius;
    vector<cv::Point> fivepoints;
    cv::Mat background;
    cv::Mat showpic;


};

#endif