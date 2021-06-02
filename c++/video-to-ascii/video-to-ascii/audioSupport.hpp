//
//  audioSupport.hpp
//  video-to-ascii
//
//  Created by Vincent Kwok on 2/6/21.
//

#ifndef audioSupport_hpp
#define audioSupport_hpp

#include <string>

extern "C" {
    #include "libavutil/avstring.h"
    #include "libavutil/eval.h"
    #include "libavutil/mathematics.h"
    #include "libavutil/pixdesc.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/dict.h"
    #include "libavutil/fifo.h"
    #include "libavutil/parseutils.h"
    #include "libavutil/samplefmt.h"
    #include "libavutil/avassert.h"
    #include "libavutil/time.h"
    #include "libavutil/bprint.h"
    #include "libavformat/avformat.h"
    #include "libavdevice/avdevice.h"
    #include "libswscale/swscale.h"
    #include "libavutil/opt.h"
    #include "libavcodec/avfft.h"
    #include "libswresample/swresample.h"
}

void audioPlay(std::string filePath);

#endif /* audioSupport_hpp */
