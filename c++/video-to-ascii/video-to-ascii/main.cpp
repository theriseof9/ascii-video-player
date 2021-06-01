//
//  main.cpp
//  video-to-ascii
//
//  Created by Wang Zerui on 1/6/21.
//

#include "opencv2/opencv.hpp"
#include <iostream>
#include <thread>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

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

void printProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

vector<string> buffer;

void decToAscii(VideoCapture cap) {
    while (1) {
        // printProgress(float(fmCnt)/float(totalFrame));
        // fmCnt += 1;
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
        
        for (int i = 0; i < frame.rows; i++) {
            buffer.push_back("");
            for (int j = 0; j < frame.cols; j++) {
                bgrPixel.val[2] = pixelPtr[i * frame.cols * cn + j * cn + 0]; // B
                bgrPixel.val[1] = pixelPtr[i * frame.cols * cn + j * cn + 1]; // G
                bgrPixel.val[0] = pixelPtr[i * frame.cols * cn + j * cn + 2]; // R

                // do something with RGB values...
                const uint8_t intensity = (bgrPixel[0] + bgrPixel[1] + bgrPixel[2]) / 60;
                buffer[buffer.size()-1] += "\u001b[38;5;"+to_string(getColorId(bgrPixel[0], bgrPixel[1], bgrPixel[2]))+"m"+DENSITY[intensity];
            }
            buffer[buffer.size()-1] += "\n";
        }
        // Press ESC on keyboard to exit
        // char c = (char) waitKey(1);
        // if (c == 27) break;
    }
}

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
    
    // Check if camera was opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    
    // int fmCnt = 0;
    // int totalFrame = cap.get(CAP_PROP_FRAME_COUNT);
    
    thread decThread(decToAscii, cap);
    
    cout << "Performing initial buffering" << endl;
    sleep(2);
    
    unsigned int i = 0;
    while (1) {
        cout << buffer[i] << flush;
        i++;
        if (i > buffer.size()) break;
    }
    
    cout << "Done";
    
    /*for (int i = 0; i < buffer.size(); i ++) {
        cout << buffer[i] << flush;
    }*/
    
    decThread.join();
    
    // When everything is done, release the video capture object
    cap.release();
    
    return 0;
}
