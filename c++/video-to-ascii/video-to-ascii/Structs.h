//
//  Structs.h
//  video-to-ascii
//
//  Created by Vincent Kwok on 3/6/21.
//

#ifndef Structs_h
#define Structs_h

#include <string>

using namespace std;

struct FlagActions {
    string flag;
    string description;
    void (*fAct)();
};

#endif /* Structs_h */
