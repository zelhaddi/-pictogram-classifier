#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
using namespace std;


#include "opencv2/imgproc.hpp"
using namespace cv;


/** Show an image reduced by a factor
 * @param m : the image to show
 * @param name : the name of the window
 * @param factor : the factor of reduction
 */
void show_reduced_img(const Mat& m, const string& name, int factor);


#endif //TOOLS_H
