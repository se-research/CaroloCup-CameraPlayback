# Make variables to create the Docker image.
PRODUCT=carolocup-cameraplayback
VERSION=v3
REPOSITORY=seresearch
BUILDLOG=build.log

docker:
	docker build -t $(REPOSITORY)/$(PRODUCT):$(VERSION) . | tee $(BUILDLOG) || exit 1

