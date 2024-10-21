#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

// Base class representing an image
class Image {
protected:
    string imageName;
    int width, height;
    bool isLoaded;

public:
    Image() : imageName(""), width(0), height(0), isLoaded(false) {}

    virtual void loadImage(const string& name) {
        imageName = name;
        width = 800;  // Default width for simplicity
        height = 600; // Default height
        isLoaded = true;
        cout << "Image " << imageName << " loaded with size: " << width << "x" << height << endl;
    }

    virtual void displayInfo() const {
        if (isLoaded) {
            cout << "Image: " << imageName << " (" << width << "x" << height << ")" << endl;
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
        cout << "Applying " << filterType << " filter to image: " << imageName << endl;
    }
};

// Derived class for resizing an image
class Resize : public Image {
public:
    virtual void resizeImage(int newWidth, int newHeight) {
        if (!isLoaded) {
            throw runtime_error("No image loaded to resize!");
        }
        width = newWidth;
        height = newHeight;
        cout << "Image resized to: " << width << "x" << height << endl;
    }
};

// Derived class for saving an image
class Save : public Image {
public:
    virtual void saveImage(const string& outputPath) {
        if (!isLoaded) {
            throw runtime_error("No image loaded to save!");
        }
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
                    cout << "Enter image name to load: ";
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
