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
    cout << date << " - ";
    
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
