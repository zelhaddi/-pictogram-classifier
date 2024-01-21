#ifndef ROTATE_HPP
#define ROTATE_HPP

#include "opencv2/opencv.hpp"
using namespace cv;


/**
 * Rotate the image according so that the lines are horizontal and vertical
 * @param gray_image : the image in grayscale
 * @param show_image : if true, show the image with the rectangles around each vignette
 * @return the rotated image
 */
Mat& rotate_image(Mat& gray_image, bool show_image);

#endif /*ROTATE_HPP*/


