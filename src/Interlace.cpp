#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include <opencv2/opencv.hpp>

#include <omp.h>

#include "FrameBuffer.h"
#include "FrameProducer.h"
#include "FrameStitcher.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    /* Parse arguments and ensure that at least videoA and videoB are provided
     */
    if (argc < 3) {
        cerr << "Format"
             << "  ::  "
             << "./Interlace videoA videoB width height" << endl;
        cerr << "(e.g.)"
             << "  ::  "
             << "./Interlace field_trees.avi dynamic_test.mp4" << endl;
        cerr << "(e.g.)"
             << "  ::  "
             << "./Interlace field_trees.avi dynamic_test.mp4 400 500" << endl;
        return 1;
    }

    /* If resolution is also given parse that */
    Size res;
    if (argc >= 5) {
        try {
            res = Size(stoi(argv[3]), stoi(argv[4]));
        } catch (const exception& e) {
            cerr << "Arguments 3 & 4 were not numbers." << endl;
            return 1;
        }
    } else
        res = Size(600, 400);

    /* Initialize two buffers, one for each video feed */
    FrameBuffer a_buf = FrameBuffer();
    FrameBuffer b_buf = FrameBuffer();

    /* Initialize two frame producers which read each video and fill up the
     * buffers */
    FrameProducer a = FrameProducer(argv[1], a_buf);
    FrameProducer b = FrameProducer(argv[2], b_buf);

    if (a.my_frame_count == 0) {
        cerr << argv[1] << " could not be decoded." << endl;
        return 1;
    }

    if (b.my_frame_count == 0) {
        cerr << argv[2] << " could not be decoded." << endl;
        return 1;
    }

    /* Initialize a frame stitcher which assembles the output based on the two
     * buffers */
    FrameStitcher s =
        FrameStitcher("output.avi", a.fourcc, 10.0, res, a_buf, b_buf);

    int output_frames = min(a.my_frame_count, b.my_frame_count) - 1;

    chrono::time_point<std::chrono::system_clock> start, end;

    cout << "Starting Conversion..."
         << "\t";
    start = chrono::system_clock::now();

    /* Now we start the three threads and wait for them to all join */
    thread a_thread(&FrameProducer::convertFrames, &a, output_frames, res);
    thread b_thread(&FrameProducer::convertFrames, &b, output_frames, res);
    thread s_thread(&FrameStitcher::stitchFrames, &s, output_frames * 2);

    /* Wait for all threads to rejoin */
    a_thread.join();
    b_thread.join();
    s_thread.join();

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "...Completed in " << elapsed_seconds.count() << " seconds."
         << endl;

    return 0;
}