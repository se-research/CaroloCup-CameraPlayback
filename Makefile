CXX=g++
RM=rm -f
INCLUDES=-I/usr/include/opendavinci
CPPFLAGS=-Wall $(INCLUDES)
LDLIBS=-lopendavinci -lpthread -lopencv_core -lopencv_highgui

BIN=CaroloCup-CameraPlayback
SRCS=CaroloCup-CameraPlayback.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

# Make variables to create the Docker image.
PRODUCT=carolocup-cameraplayback
VERSION=v2
REPOSITORY=seresearch
BUILDLOG=build.log

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) -o $(BIN) $(OBJS) $(LDLIBS) 

docker: $(BIN)
	docker build -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee $(BUILDLOG) || exit 1

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) $(BUILDLOG)
	$(RM) $(BIN)
	$(RM) *~ .depend

include .depend

