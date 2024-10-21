#include <iostream>
#include <string>
#include <opencv2/opencv.hpp> // Include OpenCV

using namespace std;
using namespace cv; // OpenCV namespace

// Base class representing an image
class Image {
protected:
    string imageName;
    Mat img;  // OpenCV matrix to store the image
    bool isLoaded;

public:
    Image() : imageName(""), isLoaded(false) {}

    // Method to load the image using OpenCV
    virtual void loadImage(const string& name) {
        imageName = name;
        img = imread(imageName);  // Load the image using OpenCV
        
        if (!img.empty()) {
            isLoaded = true;
            cout << "Image " << imageName << " loaded with size: " 
                 << img.cols << "x" << img.rows << endl;
        } else {
            throw runtime_error("Error: Could not load the image.");
        }
    }

    virtual void displayInfo() const {
        if (isLoaded) {
            cout << "Image: " << imageName << " (" << img.cols << "x" << img.rows << ")" << endl;
        } else {
            cout << "No image loaded." << endl;
        }
    }

    virtual ~Image() {}
};

// Derived class for applying filters to an image
class Filter : public Image {
public:
    virtual void applyFilter(const string& filterType) {
        if (!isLoaded) {
            throw runtime_error("No image loaded to apply the filter!");
        }
        if (filterType == "grayscale") {
            cvtColor(img, img, COLOR_BGR2GRAY); // Convert to grayscale
            cout << "Grayscale filter applied to " << imageName << endl;
        }
    }
};

// Derived class for resizing an image
class Resize : public Image {
public:
    virtual void resizeImage(int newWidth, int newHeight) {
        if (!isLoaded) {
            throw runtime_error("No image loaded to resize!");
        }
        resize(img, img, Size(newWidth, newHeight));
        cout << "Image resized to: " << newWidth << "x" << newHeight << endl;
    }
};

// Derived class for saving an image
class Save : public Image {
public:
    virtual void saveImage(const string& outputPath) {
        if (!isLoaded) {
            throw runtime_error("No image loaded to save!");
        }
        imwrite(outputPath, img);  // Save the image using OpenCV
        cout << "Image saved to: " << outputPath << endl;
    }
};

// Polymorphic function to demonstrate inheritance and polymorphism
void processImage(Image* img, const string& operation) {
    if (operation == "filter") {
        Filter* f = dynamic_cast<Filter*>(img);
        if (f) {
            f->applyFilter("grayscale");
        }
    } else if (operation == "resize") {
        Resize* r = dynamic_cast<Resize*>(img);
        if (r) {
            r->resizeImage(1024, 768); // Resize to 1024x768 for demo
        }
    } else if (operation == "save") {
        Save* s = dynamic_cast<Save*>(img);
        if (s) {
            s->saveImage("output_image.jpg");
        }
    }
}

void showMenu() {
    cout << "\nMenu:" << endl;
    cout << "1. Load Image" << endl;
    cout << "2. Apply Filter" << endl;
    cout << "3. Resize Image" << endl;
    cout << "4. Save Image" << endl;
    cout << "5. Exit" << endl;
    cout << "Select an option: ";
}

int main() {
    Image* image = nullptr;
    int choice;
    string imageName, outputPath;
    
    try {
        while (true) {
            showMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    // Load Image
                    image = new Image();
                    cout << "Enter image name to load (with path): ";
                    cin >> imageName;
                    image->loadImage(imageName);
                    break;

                case 2:
                    // Apply Filter
                    if (!image || !dynamic_cast<Filter*>(image)) {
                        image = new Filter();
                    }
                    processImage(image, "filter");
                    break;

                case 3:
                    // Resize Image
                    if (!image || !dynamic_cast<Resize*>(image)) {
                        image = new Resize();
                    }
                    processImage(image, "resize");
                    break;

                case 4:
                    // Save Image
                    if (!image || !dynamic_cast<Save*>(image)) {
                        image = new Save();
                    }
                    cout << "Enter output path to save image: ";
                    cin >> outputPath;
                    processImage(image, "save");
                    break;

                case 5:
                    // Exit
                    cout << "Exiting the application." << endl;
                    return 0;

                default:
                    cout << "Invalid choice, please try again." << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    delete image;
    return 0;
}
