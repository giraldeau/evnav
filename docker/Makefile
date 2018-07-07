# Ref: https://stackoverflow.com/questions/18136918/how-to-get-current-relative-directory-of-your-makefile
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

# The working directory must be the directory containing the Dockerfile

include Makefile.mk

REGISTRY_HOST=registry.gitlab.com
USERNAME=giraldeauf
NAME=evnav

DOCKER_BUILD_ARGS=--build-arg DOCKER_TAG=release
