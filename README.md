Playing back recordings from our CaroloCup miniature car
========================================================

This application allows you to play back recordings that
we recorded using our experimental miniature car. The car
is competing in the international competition "CaroloCup"
(http://www.carolocup.de).

To playback a recording file that contains data captured
on our experimental car like the video stream and more data,
simply clone this repository and run make all.

![Screenshot](https://github.com/se-research/CaroloCup-CameraPlayback/blob/master/screenshot.png)

To compile the source file, you need to install the
OpenDaVINCI libraries (cf. http://opendavinci.readthedocs.org)
and OpenCV.

To compile the sources on Ubuntu Linux, you would need to
install the following libraries from the main repository:

    $ sudo apt-get install build-essential libopencv-dev cmake

Next, you need to add the repository that provides OpenDaVINCI:

    $ wget -O - -q http://opendavinci.cse.chalmers.se/opendavinci.cse.chalmers.se.gpg.key | apt-key add -

    $ echo "deb http://opendavinci.cse.chalmers.se/ubuntu/ trusty main" >> /etc/apt/sources.list

Finally, you need to install the OpenDaVINCI library:

    $ sudo apt-get install opendavinci-odlib 

Now, you can compile the sources as follows:

    $ mkdir build && cd build

    $ cmake .. && make

Once you will have compiled the source file successfully,
you can run it as follows where recordingFile.rec is one
of the recordings that we provide:

    $ ./CaroloCup-CameraPlayback recordingFile.rec

If you have the corresponding ground truth data at hand,
just specify the .csv file as additional parameter:

    $ ./CaroloCup-CameraPlayback recordingFile.rec recordingFile.csv

You can find the data set here: https://se-div-c3s-1.ce.chalmers.se:7001/fbdownload/Ground_Truth?k=iKuAbfQF

Alternatively, you can also use our Docker image where
we provide a ready-to-use binary image. As our image
opens a window to play back the video data, you need to
allow it accessing your X server:

    $ xhost +

Then, you can simply run our image (Docker will pull it
for you automatically). The following command assumes that
you have a folder named ``data`` where the *.rec files
are located to be mapped into the Docker container on
its execution:

    $ docker run -ti -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v ${PWD}/data:/opt/data seresearch/carolocup-cameraplayback:latest /CaroloCup-CameraPlayback /opt/data/myRecording.rec /opt/data/myRecording.csv

