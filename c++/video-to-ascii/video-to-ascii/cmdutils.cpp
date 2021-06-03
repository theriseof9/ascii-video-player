//
//  cmdutils.cpp
//  video-to-ascii
//
//  Created by Vincent Kwok on 3/6/21.
//

#include "cmdutils.hpp"

void writeMsg(string msg, uint8_t level) {
    const auto now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    const auto date = ctime(&now);
    date[strlen(date) - 1] = '\0'; // Strip newline from date
    cout << "\u001b[0m" << date << " - ";
    
    switch (level) {
        case LOG_VERBOSE:
            cout << VERBOSE_COL << VERBOSE_STR;
            break;
        case LOG_DEBUG:
            cout << DEBUG_COL << DEBUG_STR;
            break;
        case LOG_INFO:
            cout << INFO_COL << INFO_STR;
            break;
        case LOG_WARN:
            cout << WARN_COL << WARN_STR;
            break;
        case LOG_ERROR:
            cout << ERROR_COL << ERROR_STR;
            break;
        case LOG_FATAL:
            cout << FATAL_COL << FATAL_STR;
            break;
        default:
            cout << INFO_COL << INFO_STR;
    }
    
    cout << "\u001b[0m" << ": " << msg << endl;
}

void writeBanner() {
    puts("╔══════════════════════════════════════════════════╗");
    puts("║              C++ ASCII Video Player              ║");
    //   "║ Version 0.1.0, compiled Fri May  5 00:29:26 2017 ║");
    puts(string(string("║ Version ") + VERSION_STR + ", compiled " + __TIMESTAMP__ + " ║").c_str());
    puts("║      Written by Vincent Kwok and Wang Zerui      ║");
    puts("║    Plays a video from a file on your terminal    ║");
    puts("╚══════════════════════════════════════════════════╝");
}

void writeHelp() {
    // MARK: Help header
    puts("╔═════════════════════════════════════════════════════════════════════╗");
    puts("║                     C++ ASCII Video Player Help                     ║");
    puts("╠═════════╦═══════════════════════════════════════════════════════════╣");
    puts("║ Command ║                        Description                        ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    // MARK: Help content
    
    // MARK: Help footer
    puts("╚═════════════════════════════════════════════════════════════════════╝");
}

void parseArgs(int argc, char** argv, FlagActions fActions[]) {
    
}
