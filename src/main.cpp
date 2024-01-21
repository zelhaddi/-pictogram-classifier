#include <iostream>
using namespace std;

#include "opencv2/highgui.hpp"
using namespace cv;

#include "dirent.h" // To read files in the directory
#include <sys/stat.h>  // To create a directory
#include <fstream>  // To create a file
#include <sstream> // To construct a string

#include "number_page.hpp" // To find the number page in the image
#include "size_picture.hpp" // To find the size of the picture in the image
#include "extract_images.hpp" // To extract all the images
#include "find_pictograms.hpp" // To find the names of pictograms in the image
#include "rotate.hpp"   // To rotate the image


extern string name_pictograms[14]; // Names of the pictograms


/**
 * Analyze one image
 * @param image_name: name of the image
 * @param save_image: false = Save the vignettes in separate folders, true = Save the vignettes like we have to do for the project
 */
void analyze_image(const string &image_name, bool save_image);


/**
 * Create the folders to save the images
 * @param save_image : false = Save the vignettes in separate folders, true = Save the vignettes like we have to do for the project
 */
void create_folders(bool save_image);


/**
 * Analyze all the images in a folder or one image
 * @param open_all_images : false = Open one image, true = Open all images in a folder
 * @param save_image : false = Save the vignettes in separate folders, true = Save the vignettes like we have to do for the project
 * @param path : the path of the image or of the folder
 */
void analyze_all_images(bool open_all_images, bool save_image, const string& path);


int main() {
    bool open_all_images = true; // false = Open one image, true = Open all images in a folder
    bool save_image = false; // false = Save the vignettes in separate folders, true = Save the vignettes like we have to do for the project

    // The name of the folder to open (if open_one_image = 0)
    string path = "../base_test/";  // The base test
    // string path = "../examples/";  // Only the examples
    // string path = "../../NicIcon/all-scans/";  // All the images


    // Prepare the folders to save the files
    create_folders(save_image);


    // Analyze images (or one image)
    analyze_all_images(open_all_images, save_image, path);


    return EXIT_SUCCESS;
}


void create_folders(bool save_image) {
    if(save_image) {
        DIR *dir_result;
        if ((dir_result = opendir("../results_final/")) == nullptr) {
            // If it doesn't exist, create it
            mkdir("../results_final/", 0777);
        }
        closedir(dir_result);
    }
    else {
        DIR *dir_result;
        if ((dir_result = opendir("../results_test/")) == nullptr) {
            // If it doesn't exist, create it
            mkdir("../results_test/", 0777);
        }
        closedir(dir_result);

        DIR *dir_nothing;
        if ((dir_nothing = opendir("../results_test/EMPTY/")) == nullptr) {
            // If it doesn't exist, create it
            mkdir("../results_test/EMPTY/", 0777);
        }
        closedir(dir_nothing);

        DIR *dir_o_sizes;
        if ((dir_o_sizes = opendir("../results_test/NO_SIZES/")) == nullptr) {
            // If it doesn't exist, create it
            mkdir("../results_test/NO_SIZES/", 0777);
        }
        closedir(dir_o_sizes);

        // Iterate over the different existing pictograms
        for (const string& pictogram : name_pictograms) {
            DIR *dir;
            if ((dir = opendir(("../results_test/" + pictogram).c_str())) == nullptr) {
                // If it doesn't exist, create it
                mkdir(("../results_test/" + pictogram).c_str(), 0777);
            }
            closedir(dir);
        }
    }
}


void analyze_all_images(bool open_all_images, bool save_image, const string &path) {
    if (!open_all_images) {
        // Open one image
        analyze_image(path, save_image);
    }
    else {
        // Count the number of images in the folder
        int nb_images = 0;
        DIR *dir_count;
        struct dirent *ent_count;
        if ((dir_count = opendir(path.c_str())) != nullptr) {
            while ((ent_count = readdir(dir_count)) != nullptr) {
                if (ent_count->d_type == DT_REG) { // Check if it's a regular file
                    nb_images++;
                }
            }
            closedir(dir_count);
        } else {
            std::cerr << "Error opening directory" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Open all images in a folder
        int actual_nb_image = 0;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                if (ent->d_type == DT_REG) { // Check if it's a regular file
                    string file_name = ent->d_name;
                    actual_nb_image++;
                    if (open_all_images) {
                        cout << "Image " << actual_nb_image << " / " << nb_images << endl;
                    }
                    analyze_image(path + file_name, save_image);
                }
            }
            closedir(dir);
        } else {
            std::cerr << "Error opening directory" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}


void analyze_image(const string& image_name, bool save_image) {
    // The string variables to construct the filename
    stringstream filenameStream;
    string filename;

    // Open the image
    Mat image = imread(image_name);
    if(image.data == nullptr){
        cout << "Image not found: " << image_name << endl;
        exit(EXIT_FAILURE);
    }

    // Convert the image to grayscale
    Mat gray_img;
    cvtColor(image, gray_img, COLOR_BGR2GRAY);

    Mat gray_img_rotated = rotate_image(gray_img, false);

    // Find the number page in the image
    int number = find_number_page(gray_img, false);

    int page_number = number % 100;
    int scripter_number = number / 100;

    // Transform the number page in a string (always two digits)
    string page_number_str = std::to_string(page_number);
    page_number_str = string(2 - page_number_str.length(), '0') + page_number_str;

    // Transform the scripter number in a string (always three digits)
    std::string scripter_number_str = std::to_string(scripter_number);
    scripter_number_str = string(3 - scripter_number_str.length(), '0') + scripter_number_str;

    cout << "Image: " << image_name << endl;
    cout << "Scripter number: " << scripter_number_str << endl;
    cout << "Number page: " << page_number_str << endl;


    // Find the name of the pictograms of each row
    vector<string> pictograms = detect_pictograms(gray_img);
    // If pictograms is empty, there is no pictograms
    if (pictograms.empty()) {
        cout << "No pictograms found" << endl;

        if (!save_image) {
            // Save the image in the folder EMPTY
            imwrite("../results_test/EMPTY/" + scripter_number_str + page_number_str + ".png", image);
        }

        cout << "*******************************" << endl;
        return;
    }
    else {
        cout << "Pictograms found: ";
        for (const string& i : pictograms) {
            cout << i << " ";
        }
        cout << endl;
    }


    // Detect the size of the picture in the image
    vector<string> result = detect_sizes(gray_img, false);
    // If result is empty, there is no sizes
    if (result.empty()) {
        if (!save_image) {
            // Save the image in the folder NO_SIZES
            imwrite("../results_test/NO_SIZES/" + scripter_number_str + page_number_str + ".png", gray_img);
        }
    }
    else {
        cout << "Sizes found: ";
        for (const string& i : result) {
            cout << i << " ";
        }
        cout << endl;
    }


    // Extract all the vignettes
    vector<vector<Mat>> vignettes = extract_images(image, gray_img, false);

    if (vignettes.empty()) {
        if (!save_image) {
            // Save the image in the folder EMPTY
            imwrite("../results_test/EMPTY/" + scripter_number_str + page_number_str + ".png", image);
        }
        cout << "*******************************" << endl;
        return;
    }

    // Save the images
    int id_column; // The id of the vignette
    int id_row = 1; // The id of the row (start at 1)
    for (const vector<Mat>& row : vignettes) {
        id_column = 1;  // The id of the column (start at 1)
        for (const Mat& vignette : row) {
            // cout << "Vignette " << id_row << " " << id_column << endl;
            // Save the image for the final result
            if (save_image) {

                // Files names:
                //    image file: iconeID_scripterNumber_pageNumber_row_column.png
                //    description file: iconeID_scripterNumber_pageNumber_row_column.txt
                //
                // Contents of description file:
                //    # free comment (group name, year…) (other comment lines allowed)
                //    label <labelName>
                //    form <formNumber=scripterNumberpageNumber>
                //    scripter <scripterNumber>
                //    page <pageNumber>
                //    row <rowNumber
                //    column <columnNumber>
                //    size <small/medium/large (or nothing if size not extracted)>

                // Construct the filename to save the image
                filenameStream.str(""); // Clear the string stream
                filenameStream << "../results_final/" << pictograms[id_row - 1] << "_" << scripter_number_str << "_"
                               << page_number_str << "_" << to_string(id_row) << "_" << to_string(id_column) << ".png";
                filename = filenameStream.str();

                // Save the image
                imwrite(filename, vignette);

                // Construct the filename to save the description file
                filenameStream.str(""); // Clear the string stream
                filenameStream << "../results_final/" << pictograms[id_row - 1] << "_" << scripter_number_str << "_"
                               << page_number_str << "_" << to_string(id_row) << "_" << to_string(id_column) << ".txt";
                filename = filenameStream.str();

                // Create the description file
                ofstream file(filename);

                // Write the description file
                file << "label " << pictograms[id_row - 1] << endl
                     << "form " << scripter_number_str << page_number_str << endl
                     << "scripter " << scripter_number_str << endl
                     << "page " << page_number_str << endl
                     << "row " << id_row << endl
                     << "column " << id_column << endl;

                if (!result.empty()) {
                    file << "size " << result[id_row - 1] << endl;
                }

            }
            // Save the image in the folder of the pictogram
            else {
                // Construct the filename
                filenameStream.str(""); // Clear the string stream
                filenameStream  << "../results_test/" << pictograms[id_row - 1] << "/" << scripter_number_str
                                << page_number_str << "_" << to_string(id_row) << "_" << to_string(id_column)
                                << ".png";
                filename = filenameStream.str();

                // Save the image
                imwrite(filename, vignette);
            }
            id_column++;
        }
        id_row ++;
    }

    cout << "*******************************" << endl;
}

