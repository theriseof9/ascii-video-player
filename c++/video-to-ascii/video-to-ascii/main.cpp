//
//  main.cpp
//  video-to-ascii
//
//  Created by Wang Zerui on 1/6/21.
//

#include "opencv2/opencv.hpp"
#include <iostream>

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
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    // Find home dir (on unix only)
    string HOME(getenv("HOME"));
    string vidPath("/video.mp4");
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

        // Display the resulting frame
        imshow("Output", frame);

        // Press  ESC on keyboard to exit
        char c = (char) waitKey(25);
        if (c == 27) break;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}
