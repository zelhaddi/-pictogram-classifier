#ifndef SIZE_PICTURE_HPP
#define SIZE_PICTURE_HPP

/**
 * Detect what is the size of the pictograms on each line
 * @param gray_image : the image in grayscale
 * @param show_image : if you want to print things on the image
 * @return a vector of string with the size of the pictograms on each line
 */
vector<string> detect_sizes(Mat& gray_image, bool show_image);

#endif /*SIZE_PICTURE_HPP*/
