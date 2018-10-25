#ifndef FRAME_STITCHER_H
#define FRAME_STITCHER_H

#include <string>

#include <opencv2/opencv.hpp>

#include "FrameBuffer.h"

using std::string;
using namespace cv;

class FrameStitcher
{
  public:
    FrameStitcher(const string fname, int fourcc, double fps, Size frameSize, FrameBuffer &afbuf, FrameBuffer &bfbuf);
    void stitchFrames(const int framecount);

  private:
    VideoWriter output;
    FrameBuffer &abuf;
    FrameBuffer &bbuf;
};

#endif // FRAME_STITCHER_H