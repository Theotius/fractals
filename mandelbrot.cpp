#include <iostream>
#include <complex>
#include <cmath>
#include "EasyBMP\EasyBMP.h"


using namespace std;

// Structures //
struct pixel {
    pixel(double real, double imag) : c(real, imag), z(0.0, 0.0), n(0) {}
    complex<double> c;
    complex<double> z;
    int n;
};


// Function Declarations //
void calc(pixel &, int);
void init_matrix();
void make_bmp(int, string);


// Global Variables //
const int WIDTH = 900;
const int HEIGHT = 600;

pixel *matrix[WIDTH][HEIGHT];


// Code //
int main() {
    init_matrix();
    make_bmp(42, "out.bmp");

    /*
    for (int i = 0; i < 80; i++) {
        string str = ".\\gif\\" + to_string(i) + ".bmp";
        make_bmp(i, str);
    }
    */
}

void init_matrix() {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            // Ugly calculation of pixel - coord value, replace with dynamic brot coords
            double px = (double(x)/WIDTH) * (-0.5 - -2) + -2;
            double py = (double(y)/HEIGHT) * (0.5 - -0.5) + -0.5;

            matrix[x][y] = new pixel(px, py);
        }
    }
}

void make_bmp(int max_iter=80, string out_filepath="out.bmp") {
    
    BMP out;
    out.SetSize(WIDTH, HEIGHT);
    out.SetBitDepth(8);

    double pi = 3.14159265358979323846;

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            calc(*matrix[x][y], max_iter);
            
            if (matrix[x][y]->n == max_iter) {
                out(x, y)->Red = 0;
                out(x, y)->Green = 0;
                out(x, y)->Blue = 0;
            } else {
                double colour_base = matrix[x][y]->n + 1 - log(log2(abs(matrix[x][y]->z)));
                out(x, y)->Red = cos(colour_base/2) *255;
                out(x, y)->Green = sin(colour_base/2) *255;
                out(x, y)->Blue = cos(colour_base/4) *255;
            }
        }
    }

    out.WriteToFile(out_filepath.c_str());
}

void calc(pixel &p, int max_iter=80) {
    while (abs(p.z) <= 2 and p.n < max_iter) {
        p.z = (p.z * p.z) + p.c;
        p.n++;
    }
}