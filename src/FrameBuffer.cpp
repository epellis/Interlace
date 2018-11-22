#include "FrameBuffer.h"

using namespace cv;
using std::cout;
using std::endl;
using std::lock_guard;
using std::mutex;
using std::string;
using std::unique_lock;

/*
 * Constructs and Destructs a FrameBuffer Class
 * Since nothing is allocated on the heap, nothing needs to be done
 * so this is an empty constructor
 */
FrameBuffer::FrameBuffer() = default;
FrameBuffer::~FrameBuffer() = default;

/*
 * Adds a frame to the end of the buffer. Should always work.
 */
void FrameBuffer::addFrame(Mat& frame) {
    /* Establish a critical section to write frame to the queue */
    lock_guard<mutex> lock(mu);

    buf.push(frame.clone());

    new_frames_cv.notify_all();
}

/*
 * Returns the first frame in the buffer
 */
Mat FrameBuffer::getFrame() {
    bool buffer_empty;
    /* Critical section - see if queue has any frames already */
    mu.lock();
    buffer_empty = buf.empty();
    mu.unlock();

    if (!buffer_empty) {
        lock_guard<mutex> lock(mu);

        Mat m = buf.front();
        buf.pop();
        return m;
    }

    else {
        /* Block on condition variable, will unblock when new Mat in buffer */
        unique_lock<mutex> lk(mu);
        new_frames_cv.wait(lk);

        Mat m = buf.front();
        buf.pop();
        return m;
    }
}
