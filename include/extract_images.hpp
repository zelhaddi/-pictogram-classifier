#ifndef EXTRACT_IMAGES_H
#define EXTRACT_IMAGES_H

#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
using namespace cv;

/**
 * Extract the 35 vignettes from the image
 * @param image : the image
 * @param gray_image : the image in grayscale
 * @param show_image : if true, show the image with the rectangles around each vignette
 * @return a 5 * 7 matrix of images
 */
vector<vector<Mat>> extract_images(Mat& image, Mat& gray_image, bool show_image);

#endif /*EXTRACT_IMAGES_H*/
