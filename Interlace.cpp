#include <iostream>
#include <fstream>
#include <exception>
#include <mutex>
#include <thread>
#include <chrono>

#include <opencv2/opencv.hpp>

#include <omp.h>

#include "FrameBuffer.h"
#include "FrameProducer.h"
#include "FrameStitcher.h"

// #define UNITTEST

#ifdef UNITTEST
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#endif

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{

    #ifdef UNITTEST
    int result = Catch::Session().run(argc, argv);
    return result;
    #endif

    #ifndef UNITTEST
    /* Parse arguments and ensure that at least videoA and videoB are provided */
    if (argc < 3) {
        cerr << "Format" << "  ::  " << "./Interlace videoA videoB width height" << endl;
        cerr << "(e.g.)" << "  ::  " << "./Interlace field_trees.avi dynamic_test.mp4" << endl;
        cerr << "(e.g.)" << "  ::  " << "./Interlace field_trees.avi dynamic_test.mp4 400 500" << endl;
        return 1;
    }

    /* Test that the two files exist on OS */
    ifstream f;
    f = ifstream(argv[1]);
    if (!f.good()) {
        cerr << "File " << argv[1] << " could not be located." << endl;
        return 1;
    }

    f = ifstream(argv[2]);
    if (!f.good()) {
        cerr << "File " << argv[2] << " could not be located." << endl;
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
    }
    else
        res = Size(600, 400);

    /* Initialize two buffers, one for each video feed */
    FrameBuffer a_buf = FrameBuffer();
    FrameBuffer b_buf = FrameBuffer();

    /* Initialize two frame producers which read each video and fill up the buffers */
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


    /* Initialize a frame stitcher which assembles the output based on the two buffers */
    FrameStitcher s = FrameStitcher("output.avi", a.fourcc, 10.0, res, a_buf, b_buf);

    int output_frames = min(a.my_frame_count, b.my_frame_count) - 1;

    chrono::time_point<std::chrono::system_clock> start, end;

    cout << "Starting Conversion..." << "\t";
    start = chrono::system_clock::now();

    /* Now we start the three threads and wait for them to all join */
    thread a_thread(&FrameProducer::convertFrames, &a, output_frames, res);
    thread b_thread(&FrameProducer::convertFrames, &b, output_frames, res);
    thread s_thread(&FrameStitcher::stitchFrames, &s, output_frames*2);

    /* Wait for all threads to rejoin */
    a_thread.join();
    b_thread.join();
    s_thread.join();

    end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    cout << "...Completed in " << elapsed_seconds.count() << " seconds." << endl;

    return 0;
    #endif
}

#ifdef UNITTEST
TEST_CASE( "Test Bad File", "[main]" ) {
    ifstream f;
    f = ifstream("badfile.abc");
    REQUIRE( !f.good());
    f = ifstream("dynamic_test.mp4");
    REQUIRE( f.good());
    f = ifstream("field_trees.avi");
    REQUIRE( f.good());
}

TEST_CASE( "Test Bad Number", "[main]") {
    string numstring;
    int number;
    bool caught;

    numstring = "";
    try {
        number = stoi(numstring);
        caught = false;
    } catch (const exception& e) {
        caught = true;
    }
    REQUIRE(caught);

    numstring = "forty five";
    try {
        number = stoi(numstring);
        caught = false;
    } catch (const exception& e) {
        caught = true;
    }
    REQUIRE(caught);

    numstring = "43";
    try {
        number = stoi(numstring);
        caught = false;
    } catch (const exception& e) {
        caught = true;
    }
    REQUIRE(caught==false);
    REQUIRE(number==43);
}

TEST_CASE( "Test Frame Producer " "[main]") {
    bool caught;
    FrameBuffer b;
    FrameProducer p1 = FrameProducer("dynamic_test.mp4", b);
    REQUIRE(p1.my_frame_count != 0);
    FrameProducer p2 = FrameProducer("Interlace", b);
    REQUIRE(p2.my_frame_count == 0);
}

#endif