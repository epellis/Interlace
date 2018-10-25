#ifndef FRAME_PRODUCER_H
#define FRAME_PRODUCER_H

#include <string>

#include <opencv2/opencv.hpp>

#include "FrameBuffer.h"

using std::string;
using namespace cv;

class FrameProducer
{
  public:
    FrameProducer(const string fname, FrameBuffer& fbuf);
    void convertFrames(const int frame_count, Size resolution);

    int my_frame_count;
    int fourcc;

  private:
    VideoCapture capture;
    FrameBuffer& buf;
};

#endif // FRAME_PRODUCER_H