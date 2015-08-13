Playing back recordings from our CaroloCup miniature car
========================================================

This application allows you to play back recordings that
we recorded using our experimental miniature car. The car
is competing in the international competition "CaroloCup"
(http://www.carolocup.de).

To playback a recording file that contains data captured
on our experimental car like the video stream and more data,
simply clone this repository and run make all.

To compile the source file, you need to install the
OpenDaVINCI libraries (cf. http://opendavinci.readthedocs.org)
and OpenCV.

Once you will have compiled the source file successfully,
you can run it as follows::

    $ ./CaroloCup-CameraPlayback myRecording.rec

If you have the corresponding ground truth data at hand,
just specify the .csv file as additional parameter::

    $ ./CaroloCup-CameraPlayback myRecording.rec myRecording.csv

Alternatively, you can also use our Docker image where
we provide a ready-to-use binary image. As our image
opens a window to play back the video data, you need to
allow it accessing your X server::

    $ xhost +

Then, you can simply run our image (Docker will pull it
for you automatically). The following command assumes that
you have a folder named ``data`` where the *.rec files
are located to be mapped into the Docker container on
its execution::

    $ docker run -ti -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v ${PWD}/data:/opt/data seresearch/carolocup-cameraplayback:latest /CaroloCup-CameraPlayback /opt/data/myRecording.rec /opt/data/myRecording.csv
