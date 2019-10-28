FROM debian:stretch

ARG meson_version="0.49.2"
ARG sfml_version="2.5.1"

# setup as root
RUN apt-get update && apt-get install -y \
	gcc \
	g++ \
	wget \
	python3 \
	ninja-build \
	cmake \
	pkg-config

RUN apt-get install -y \
	libx11-dev \
	libxrandr-dev \
	libudev-dev \
	libgl1-mesa-dev \
	libfreetype6-dev \
	binutils-dev \
	libiberty-dev

RUN mkdir /src /build /meson

RUN wget https://github.com/mesonbuild/meson/releases/download/"$meson_version"/meson-"$meson_version".tar.gz && \
tar -xvf meson-"$meson_version".tar.gz -C /meson --strip-components 1

RUN wget https://github.com/SFML/SFML/archive/"$sfml_version".tar.gz && \
tar -xvf "$sfml_version".tar.gz && \
cd SFML-"$sfml_version" && mkdir build && cd build && \
cmake -G"Ninja" .. \
	-DBUILD_SHARED_LIBS=FALSE \
	-DSFML_BUILD_AUDIO=FALSE \
	&& \
ninja && ninja install

WORKDIR /src

RUN useradd appuser

# switch to user
USER appuser

CMD ["/bin/bash"]
