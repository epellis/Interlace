#include "FrameProducer.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <vector>

#include <omp.h>

#define CV_CAP_PROP_FOURCC          6
#define CV_CAP_PROP_FRAME_COUNT     7
#define CV_BGR2GRAY                 6

using namespace cv;
using namespace std;

/*
 * Constructs a FrameProducer Class
 */
FrameProducer::FrameProducer(const string fname, FrameBuffer& fbuf)
: buf(fbuf)
{
    capture = VideoCapture(fname);
    my_frame_count = capture.get(CV_CAP_PROP_FRAME_COUNT);
    fourcc = capture.get(CV_CAP_PROP_FOURCC);
}


/*
 * Generates the next frame if available, else returns -1
 */
void FrameProducer::convertFrames(const int frame_count, Size resolution)
{
    Mat original, grayscale, resized;
    for (int i = 0; i < frame_count; i++) {
        capture.read(original);

        /* Check that original is not empty */
        if (original.empty()) {
            break;
        }

        /* Convert to grayscale */
        cvtColor(original, grayscale, CV_BGR2GRAY);

        /* Resize the image */
        resize(grayscale, resized, resolution);

        /*
         * To efficiently invert the image, the underlying array
         * gets a bitwise NOT which effectively inverts the image.
         * Since this is a vectoried operation without any side
         * effects it can be parallelized using OpenMP's SIMD Pragmas.
         */
        vector<uchar> array;
        array.assign(resized.datastart, resized.dataend);
        for (int i = 0; i < resized.rows; i++) {

            uchar* g_i = resized.ptr<uchar>(i);

            #pragma omp simd
            for (int j = 0; j < resized.cols; j++) {
                g_i[j] = g_i[j] ^ 0xFF;
            }

        }


        // resize(grayscale, resized, resolution);
        buf.addFrame(resized);
    }
    capture.release();
}