//
//  main.cpp
//  video-to-ascii
//
//  Created by Wang Zerui on 1/6/21.
//

#include "opencv2/opencv.hpp"
#include <iostream>

// Utilities
#include "colorUtil.hpp"

// Finding terminal size
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

using namespace std;
using namespace cv;

// ======= //
// Globals //
const string DENSITY[] = {
    " ", " ", ".", ":", "!", "+", "*", "e", "$", "@", "8",
    ".", "*", "e", "s", "◍",
    "░", "▒", "▓", "█"
};

// ======= //
// Structs //
struct winsize termSize;

int main() {
    // Fast IO speed
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    // Find home dir (on unix only)
    string HOME(getenv("HOME"));
    string vidPath("/video.mp4");
    // HOME + vidPath
    VideoCapture cap(HOME + vidPath);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while (1) {
        // Get terminal size
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize); // This only works on Unix
        
        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty()) break;
        
        // Resize frame to the size of the terminal
        resize(frame, frame, Size(termSize.ws_col, termSize.ws_row), 0, 0, INTER_AREA);
        
        uint8_t* pixelPtr = (uint8_t*)frame.data;
        int cn = frame.channels();
        Scalar_<uint8_t> bgrPixel;

        // First move the cursor all the way to the top left again
        cout << "\u001b[" << termSize.ws_col << "D";
        cout << "\u001b[" << termSize.ws_row + 1 << "A";
        
        for (int i = 0; i < frame.rows; i++) {
            for (int j = 0; j < frame.cols; j++) {
                bgrPixel.val[2] = pixelPtr[i * frame.cols * cn + j * cn + 0]; // B
                bgrPixel.val[1] = pixelPtr[i * frame.cols * cn + j * cn + 1]; // G
                bgrPixel.val[0] = pixelPtr[i * frame.cols * cn + j * cn + 2]; // R

                // do something with RGB values...
                const uint8_t intensity = (bgrPixel[0] + bgrPixel[1] + bgrPixel[2]) / 60;
                cout << "\u001b[38;5;" << getColorId(bgrPixel[0], bgrPixel[1], bgrPixel[2]) << "m" << DENSITY[intensity];
            }
            cout << "\n";
        }
        cout << flush;

        // Display the resulting frame
        imshow("Output", frame);

        // Press ESC on keyboard to exit
        // char c = (char) waitKey(1);
        // if (c == 27) break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
