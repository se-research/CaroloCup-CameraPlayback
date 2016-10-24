# Dockerfile to create a Docker image wrapping the simple
# application realized with OpenDaVINCI to playback
# recordings captured from our CaroloCup miniature cars.
#
# Copyright (C) 2015 Hugo Andrade, Christian Berger, Federico Giaimo.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

# Author: Christian Berger
# Date: 2015-08-13

FROM seresearch/opendavinci-ubuntu-amd64:latest
MAINTAINER Christian Berger "christian.berger@gu.se"

# Set the env variable DEBIAN_FRONTEND to noninteractive
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y

RUN apt-get install -y libopencv-dev

RUN apt-get upgrade -y

RUN apt-get clean
RUN apt-get autoremove

# Set locale (fix the locale warnings)
RUN localedef -v -c -i en_US -f UTF-8 en_US.UTF-8 || :

ADD build/CaroloCup-CameraPlayback /CaroloCup-CameraPlayback

