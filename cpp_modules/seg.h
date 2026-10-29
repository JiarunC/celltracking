#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
using namespace cv;
using namespace std;

class seg {
public:
    static void apply_blur(Mat& src, const int distance = 11, const double sigma_color = 15, const double sigma_space = 15) {
        const Mat dst = src.clone();
        bilateralFilter(dst, src, distance, sigma_color, sigma_space);
    }

    static void apply_threshold(Mat& src) {
        auto th = threshold(src, src, 0, 255, THRESH_BINARY + THRESH_TRIANGLE + THRESH_DRYRUN);
        threshold(src, src, th * .8, 255, THRESH_BINARY);
    }

    static void apply_morph(Mat& src) {
        auto kernel_close = getStructuringElement(MORPH_ELLIPSE, Size(7,7));
        auto kernel_open = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
        morphologyEx(src, src, MORPH_OPEN, kernel_open, Point(-1,-1), 1);
        morphologyEx(src, src, MORPH_CLOSE, kernel_close, Point(-1,-1), 4);
        morphologyEx(src, src, MORPH_OPEN, kernel_open, Point(-1,-1), 1);
    }
};