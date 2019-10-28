#!/bin/bash

if [ ! -d "/build" ]; then
	exit -1
fi

if [ -z "$(ls -A /build)" ]; then
	/meson/meson.py /build
fi

cd /build && ninja
