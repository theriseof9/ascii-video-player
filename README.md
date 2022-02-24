# C++ ASCII Video Player

## About

Plays a video file as ASCII text on your terminal in realtime. High performance C++ code allows realtime smooth 60fps playback (depending on terminal size) with audio playback and A/V sync.

## Support

Operating system compatibility:

|       OS      |            Status            |
|---------------|------------------------------|
| macOS >= 11.0 | ✅　Verified: Builds and runs |
| POSIX-compliant OS | ❓　Unknown: **Should** work in theory but untested |
| Windows       | ✅　Works on windows |

## Building

Currently, this project is only tested on macOS, but should work on any POSIX-compliant OS. Windows support is WIP.

1. Clone this repository: `git clone https://github.com/Wang-Zerui/ascii-video-player.git`
2. Install the following dependancies: `ffplay` and `opencv`:
    - With Homebrew:
        1. `brew install ffmpeg` (ffplay)
        2. `brew install opencv`
    - Self installation:
        1. Ensure OpenCV headers and libraries are present in `/usr/local/Cellar/opencv/<opencv_version>/include/opencv4` and `/usr/local/Cellar/opencv/<opencv_version>/lib` respectively
        2. Ensure the ffplay binary is present in `/usr/local/bin/ffplay`
4. Open the Xcode project at `./c++/video-to-ascii/video-to-ascii.xcodeproj`
5. Edit the header/library search paths in Targets > video-to-ascii > Build Settings > Search Paths > Header/Library Search Paths if neccessary
6. Build the project (the executable cannot be run in Xcode as the built-in terminal reports a size of 0x0)
7. Run the executable with any terminal from the Xcode build folder

## Contributing

We welcome issues/pull requests that help improve this project. Please ensure your code compiles and has better performance or fixes bugs present in the code.
Before submiting a pull request, please take note of the following:
1. One PR per issue
2. Describe what bug(s) fixes or optimisations your code contains that improves the project
3. Must build with no errors on all supported platforms
4. Does not require additional dependancies unless there is a very good reason to do so (approved on case-by-case basis)
5. All "oops" commits are cleaned up

## Credits

This application is programmed in C++ by Vincent Kwok ([@CryptoAlgorithm](https://github.com/CryptoAlgorithm)) and Wang Zerui ([@Wang-Zerui](https://github.com/Wang-Zerui))
