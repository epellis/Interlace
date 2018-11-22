#include "FrameStitcher.h"

using namespace cv;
using std::cout;
using std::endl;
using std::string;

/*
 * Constructs a FrameStitcher Class
 */
FrameStitcher::FrameStitcher(const string fname, int fourcc, double fps,
                             Size frameSize, FrameBuffer &afbuf,
                             FrameBuffer &bfbuf)
    : abuf(afbuf), bbuf(bfbuf) {
    output = VideoWriter(fname, fourcc, fps, frameSize, false);
}

/*
 * Stitches frames from two outputs until both are exhausted.
 * This effectively interlaces video from two sources
 */
void FrameStitcher::stitchFrames(const int framecount) {
    Mat aframe, bframe;
    int current_frames = 0;

    while (true) {
        aframe = abuf.getFrame();
        bframe = bbuf.getFrame();

        if (!aframe.empty()) output.write(aframe);
        if (!bframe.empty()) output.write(bframe);

        current_frames += 2;

        if (current_frames >= framecount) {
            break;
        }
    }

    output.release();
}