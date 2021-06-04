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

void writeHelp(FlagActions fActions[], uint16_t fArgs) {
    // MARK: Help header
    puts("╔═════════════════════════════════════════════════════════════════════╗");
    puts("║                     C++ ASCII Video Player Help                     ║");
    puts("╠═════════╦═══════════════════════════════════════════════════════════╣");
    puts("║ Command ║                        Description                        ║");
    puts("╠═════════╬═══════════════════════════════════════════════════════════╣");
    // MARK: Help content
    
    // MARK: Help footer
    puts("╚═════════╩═══════════════════════════════════════════════════════════╝");
}

vector<FlagOps> parseArgs(int argc, char** argv, FlagActions fActions[], uint16_t fArgs) {
    vector<FlagOps> flags;
    for (uint16_t i = 1; i < argc; i++) {
        // Check if this is a flag
        if (argv[i][0] != '-') {
            if (i != argc - 1) puts(("Unrecognised argument: '" + string(argv[i]) + "'").c_str());
            continue;
        }
        if (strcmp("-help", argv[i]) == 0 || strcmp("-h", argv[i]) == 0) {
            writeHelp(fActions, fArgs);
            exit(0);
        }
        bool valid = false;
        for (uint16_t j = 0; j < fArgs; j++) {
            // cout << (string("-") += fActions[j].flag).c_str();
            if (strcmp((string("-") += fActions[j].flag).c_str(), argv[i]) == 0) {
                if (fActions[j].isAct) {
                    fActions[j].fAct();
                    exit(0);
                }
                else {
                    valid = true;
                    const string fStr = string(argv[i]).substr(1);
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        flags.push_back({fStr, argv[i + 1]});
                        i++; // Skip the next argument
                    }
                    else flags.push_back({fStr, ""});
                }
            }
        }
        if (!valid) puts((string("Invalid flag '") += argv[i] + string("'")).c_str());
    }
    return flags;
}
