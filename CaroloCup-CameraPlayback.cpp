/**
 * Simple application realized with OpenDaVINCI to playback
 * recordings captured from our CaroloCup miniature cars.
 *
 * Copyright (C) 2015 Hugo Andrade, Christian Berger, Federico Giaimo.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <core/SharedPointer.h>
#include <core/data/Container.h>
#include <core/data/image/SharedImage.h>
#include <core/io/URL.h>
#include <core/wrapper/SharedMemory.h>
#include <core/wrapper/SharedMemoryFactory.h>
#include <tools/player/Player.h>

using namespace std;

// We add some of OpenDaVINCI's namespaces for the sake of readability.
using namespace core;
using namespace core::data;
using namespace core::data::image;
using namespace core::io;
using namespace core::wrapper;
using namespace tools::player;

int32_t main(int32_t argc, char **argv) {
    // Location of the recording file.
    URL url("file://example.rec");

    // Do we want to rewind the stream on EOF?
    const bool AUTO_REWIND = false;

    // Size of the memory buffer that should fit at least the size of one frame.
    const uint32_t MEMORY_SEGMENT_SIZE = 1024 * 768;

    // Number of memory segments (one is enough as we are running sychronously).
    const uint32_t NUMBER_OF_SEGMENTS = 1;

    // Run player in synchronous mode without data caching in background.
    const bool THREADING = false;

    // Construct the player.
    Player player(url, AUTO_REWIND, MEMORY_SEGMENT_SIZE, NUMBER_OF_SEGMENTS, THREADING);

    // The next container from the recording.
    Container nextContainer;

    // Using OpenCV's IplImage data structure to simply playback the data.
    IplImage *image = NULL;

    // Create the OpenCV playback window.
    cvNamedWindow("CaroloCup-CameraPlayback", CV_WINDOW_AUTOSIZE);

    // This flag indicates whether we have attached already to the shared
    // memory containing the sequence of captured images.
    bool hasAttachedToSharedImageMemory = false;

    // Using this variable, we will access the captured images while
    // also having convenient automated system resource management.
    SharedPointer<SharedMemory> sharedImageMemory;

    // Main data processing loop.
    while (player.hasMoreData()) {
        // Read next entry from recording.
        nextContainer = player.getNextContainerToBeSent();

        // Data type SHARED_IMAGE contains a SharedImage data structure that
        // provides meta-information about the captured image.
        if (nextContainer.getDataType() == Container::SHARED_IMAGE) {
            // Read the data structure to retrieve information about the image.
            SharedImage si = nextContainer.getData<SharedImage>();

            // Check if we have already attached to the shared memory.
            if (!hasAttachedToSharedImageMemory) {
                sharedImageMemory = SharedMemoryFactory::attachToSharedMemory(si.getName());

                // Toggle the flag as we have now attached to the shared memory.
                hasAttachedToSharedImageMemory = true;
            }

            // Check if we could successfully attach to the shared memory.
            if (sharedImageMemory->isValid()) {
                // Lock the memory region to get exclusive access.
                sharedImageMemory->lock();
                {
                    if (image == NULL) {
                        // Create the IplImage header data and access the shared memory for the actual image data. 
                        image = cvCreateImageHeader(cvSize(si.getWidth(), si.getHeight()), IPL_DEPTH_8U, si.getBytesPerPixel());

                        // Let the IplImage point to the shared memory containing the captured image.
                        image->imageData = static_cast<char*>(sharedImageMemory->getSharedMemory());
                    }

                    // Show the image using OpenCV.
                    cvShowImage("CaroloCup-CameraPlayback", image);

                    // Let the image render before proceeding to the next image.
                    cvWaitKey(10);
                }
                // Release the memory region so that the player can provide the next raw image data.
                sharedImageMemory->unlock();
            }
        }
    }

    // Release IplImage data structure.
    cvReleaseImage(&image);

    // Close playback window.
    cvDestroyWindow("CaroloCup-CameraPlayback");

    // The shared memory will be automatically released.
}
