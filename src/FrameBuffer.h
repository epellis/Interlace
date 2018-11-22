#include <condition_variable>
#include <mutex>
#include <queue>

#include <opencv2/opencv.hpp>

#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

using namespace cv;
using std::condition_variable;
using std::mutex;
using std::queue;

class FrameBuffer {
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

#endif  // FRAME_BUFFER_H