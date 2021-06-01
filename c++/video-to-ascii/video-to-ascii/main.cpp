//
//  main.cpp
//  video-to-ascii
//
//  Created by Wang Zerui on 1/6/21.
//

#include "opencv2/opencv.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// Utilities
#include "colorUtil.hpp"

// Finding terminal size
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

using namespace std;
using namespace cv;
using namespace std::chrono;

// ========= //
// Constants //
const string DENSITY[] = {
    " ", " ", ".", ":", "!", "+", "*", "e", "$", "@", "8",
    ".", "*", "e", "s", "◍",
    "░", "▒", "▓", "█"
};

// ======= //
// Globals //
struct winsize termSize;
vector<string> buffer;

// Renderer function, running async
void decToAscii(VideoCapture cap) {
    while (1) {
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
        
        buffer.push_back("");
        buffer[buffer.size()-1] += "\u001b[" + to_string(termSize.ws_col) + "D\u001b[" + to_string(termSize.ws_row) + "A";
        for (int i = 0; i < frame.rows; i++) {
            for (int j = 0; j < frame.cols; j++) {
                bgrPixel.val[2] = pixelPtr[i * frame.cols * cn + j * cn + 0]; // B
                bgrPixel.val[1] = pixelPtr[i * frame.cols * cn + j * cn + 1]; // G
                bgrPixel.val[0] = pixelPtr[i * frame.cols * cn + j * cn + 2]; // R

                // do something with RGB values...
                const uint8_t intensity = (bgrPixel[0] + bgrPixel[1] + bgrPixel[2]) / 60;
                buffer[buffer.size()-1] += "\u001b[38;5;" + to_string(getColorId(bgrPixel[0], bgrPixel[1], bgrPixel[2])) + "m" + DENSITY[intensity];
            }
            buffer[buffer.size()-1] += "\n";
        }
    }
}

int main() {
    // Fast IO speed
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Get terminal size
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize); // This only works on Unix
    
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
    
    const auto targetDelay = 1000000 / cap.get(CAP_PROP_FPS);
    
    // int totalFrame = cap.get(CAP_PROP_FRAME_COUNT);
    
    thread decThread(decToAscii, cap); // Start renderer thread
    
    cout << "Performing initial buffering, please wait a second..." << endl;
    sleep(1);
    
    unsigned int i = 0;
    
    while (1) {
        const auto t1 = high_resolution_clock::now();
        cout << buffer[i];
        i++;
        if (i > buffer.size()) break;
        // Press ESC on keyboard to exit
        // char c = (char) waitKey(5);
        // if (c == 27) break;
        const auto t2 = high_resolution_clock::now();
        const auto dur = duration_cast<microseconds>(t2 - t1).count();
        if (dur >= 0) usleep(targetDelay - dur);
    }
    
    cout << "Done" << endl;
    
    decThread.join(); // Make sure thread is finished
    
    // When everything is done, release the video capture object
    cap.release();
    
    return 0;
}
