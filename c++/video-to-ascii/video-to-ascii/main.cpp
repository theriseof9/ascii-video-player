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
#include <time.h>

// Utilities
#include "colorUtil.hpp"

// Finding terminal size
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

using namespace std;
using namespace cv;
using namespace std::chrono;

// MARK: Constants
const string DENSITY[] = {
    " ", " ", ".", ":", "!", "+", "*", "e", "$", "@", "8",
    ".", "*", "e", "s", "◍",
    "░", "▒", "▓", "█"
};

// MARK: Globals
struct winsize termSize;
vector<string> buffer;

// MARK:- Renderer function, running async

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
                const uint8_t intensity = (bgrPixel[0] + bgrPixel[1] + bgrPixel[2]) / 40.26;
                buffer[buffer.size()-1] += "\u001b[38;5;" + to_string(getColorId(bgrPixel[0], bgrPixel[1], bgrPixel[2])) + "m" + DENSITY[intensity];
            }
            if (i != frame.rows - 1) buffer[buffer.size()-1] += "\n";
        }
    }
}

// MARK:- Audio playback thread
// Its sole purpose is to start the ffplay program

void playAudio(string path) {
    system(("ffplay -vn -nodisp " + path + " 2>/dev/null").c_str());
}

// MARK:- Termination Handler

void sigCleanUp(int signum) {
    cout << "\u001b[0mThanks, and goodbye!" << endl;
    
    exit(signum);
}

// MARK:- Main

int main() {
    // Fast IO speed
    cout.tie(0);
    ios_base::sync_with_stdio(0);
    
    // Register SIGINT signal handler
    signal(SIGINT, sigCleanUp);
    
    // Get terminal size
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &termSize); // This only works on Unix
    
    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    // Find home dir (on Unix only)
    string HOME(getenv("HOME"));
    string vidPath("/video.mp4");
    // HOME + vidPath
    VideoCapture cap(HOME + vidPath);
    
    // Check if camera was opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    
    const auto targetDelay = 1000ms / cap.get(CAP_PROP_FPS);
    
    thread decThread(decToAscii, cap); // Start renderer thread
    
    cout << "Performing initial buffering, please wait a second..." << endl;
    sleep(1);
    
    thread audioThread(playAudio, HOME + vidPath); // Start audio thread
    
    unsigned int i = 0;
    
    while (1) {
        const auto t1 = high_resolution_clock::now();
        // printf("\033c");
        cout << buffer[i];
        i++;
        if (i > buffer.size()) break;
        // Press ESC on keyboard to exit
        // char c = (char) waitKey(5);
        // if (c == 27) break;
        const auto t2 = high_resolution_clock::now();
        const auto dur = duration_cast<microseconds>(t2 - t1).count();
        // struct timespec delayTime;
        // delayTime.tv_nsec = (targetDelay - dur) * 1000;
        
        if (dur >= 0) this_thread::sleep_for(targetDelay - (t2 - t1));
    }
    cout << "End of video, thanks for watching!" << endl;
    
    decThread.detach(); // If the thread is still running, detach it (at this point it should not be running)
    
    // Release the video capture object
    cap.release();
    
    sigCleanUp(2);
    
    return 0;
}
