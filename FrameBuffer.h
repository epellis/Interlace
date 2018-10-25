#include <queue>
#include <mutex>
#include <condition_variable>

#include <opencv2/opencv.hpp>

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

using namespace cv;
using std::queue;
using std::mutex;
using std::condition_variable;

class FrameBuffer
{
  public:
    FrameBuffer();
    FrameBuffer(const FrameBuffer& copy_from);
    FrameBuffer& operator=(const FrameBuffer& copy_from);
    ~FrameBuffer();

    void addFrame(Mat& frame);
    Mat getFrame();

  private:
    queue<Mat> buf;
    mutex mu;
    condition_variable new_frames_cv;
};


#endif // FRAME_BUFFER_H