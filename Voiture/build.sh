#!/bin/bash

dir=$(pwd)

if [ ! -d "build" ]; then
	mkdir build
fi

sudo docker run --rm -it -v "$dir"/build:/build -v "$dir"/src:/src:ro sfml_env ./build_cmd.sh
