### Image Opening and Conversion to Grayscale
- **CvtColor:** Used to convert the image from BGR to Gray format by averaging the R, G, and B values for each pixel of the image.

### Page Number Detection
- Utilizes squares in the top right corner, encoding the page number in binary to determine the current page being processed.
1. Apply a threshold to convert the image to black and white, simplifying the image, highlighting contours, and reducing computational costs for the next step.
    - **Threshold:** Pixels above the threshold become white, below become black.
2. Search for all contours in the image.
    - **FindContours:** Detects contours on the image with parameters:
        - `RETR_TREE`: Detects all contours.
        - `CHAIN_APPROX_SIMPLE`: Retains only important points.
3. Transform contours into rectangles.
    - **BoundingRect:** Takes a contour as input and returns the coordinates and size of the bounding rectangle.
4. Sort rectangles based on their coordinates, size, and relative positions to determine the page number code.

### Pictogram Detection
- Identifies which pictogram corresponds to each line by detecting pictograms on the left. If none are found, it indicates a page with a large text area, moving to the next image.
1. In a folder, various images of each pictogram are saved. Attempt to match each one in the image.
    - **MatchTemplate:** Searches for occurrences of an image within another image using a template correlation process. It compares the model with sub-regions of the source image and returns a similarity map indicating the correspondence between the model and different positions in the source image. When this similarity is above a set threshold, the image is retained.
2. Detect the relative position of each found pictogram to determine which pictogram corresponds to which line.

### Size Detection
- Detects the sizes of each pictogram (small, medium, and large). If not detected, it implies they are absent. The same method used for pictogram detection is employed here.

### Image Extraction
- Cuts out drawings within 35 black squares to save them.
1. Pre-process the image to highlight contours.
    - **Canny:** Method to convert an image to binary while retaining contours.
    - **Dilation:** Increases the size of light areas in an image using a structuring element to compare surrounding pixels, aiding in identifying the edge of the squares.
2. Retrieve contours using `findContours` with the `RETR_EXTERNAL` parameter (retains only external contours).

### Saving
- Saves each previously extracted drawing and creates a file listing the various information in the correct format.

### Results:
- From the initial set of 806 pages:
    - Number of correctly identified page numbers: 790 (98%)
    - Number of pages with incorrectly retrieved sizes: 4 (0.5%)
    - Number of images mistakenly identified as empty: 76 (9.4%)
    - Number of images showing black borders: 71 (8.8%)
    - Overall success rate: 82%

- From the test set of 12 pages:
    - Number of correctly identified page numbers: 12 (100%)
    - Overall success rate: 0%
In this set, the pictograms were not detected at all by match template.
