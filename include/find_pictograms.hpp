#ifndef FIND_PICTOGRAMS_H
#define FIND_PICTOGRAMS_H

#include <iostream>
using namespace std;

#include <vector>
#include <string>


/**
 * Detect what is the size of the pictograms on each line
 * @param gray_image : the image in grayscale
 * @return a vector of string with the name of the pictograms on each line
 */
vector<string> detect_pictograms(Mat& gray_image);


#endif /*FIND_PICTOGRAMS_H*/
