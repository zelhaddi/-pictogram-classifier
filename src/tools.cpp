#include <iostream>
using namespace std;

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;


#include "tools.hpp"

/** Show an image reduced by a factor
 * @param m : the image to show
 * @param name : the name of the window
 * @param factor : the factor of reduction
 */
void show_reduced_img(const Mat& m, const string& name, const int factor) {
    // Check if the input image is empty
    if (m.empty()) {
        cout << "Input image is empty." << endl;
        return;
    }

    // Check if the name is empty
    if (name.empty()) {
        cout << "Name is empty." << endl;
        return;
    }

    // Check if the factor is valid
    if (factor <= 0) {
        cout << "Invalid reduction factor." << endl;
        return;
    }

    Size reduced_size(m.cols / factor, m.rows / factor);
    Mat reduced_img;

    // Resize the image directly
    resize(m, reduced_img, reduced_size);

    // Show the image
    imshow(name, reduced_img);
    waitKey(0);
}