FROM debian:stretch-slim
MAINTAINER Emmanuele Bassi <ebassi@gmail.com>

RUN apt-get update -qq \
    && apt-get install --no-install-recommends -qq -y \
        clang \
        gcc \
        gobject-introspection \
        gir1.2-glib-2.0 \
        git-core \
        libgirepository1.0-dev \
        libglib2.0-dev \
        locales \
        ninja-build \
        pkg-config \
        python3 \
        python3-pip \
        python3-setuptools \
        python3-wheel \
        python3-yaml \
    && rm -rf /usr/share/doc/* /usr/share/man/*

RUN locale-gen C.UTF-8 && /usr/sbin/update-locale LANG=C.UTF-8

ENV LANG=C.UTF-8 LANGUAGE=C.UTF-8 LC_ALL=C.UTF-8

RUN pip3 install meson==0.50.1

RUN pip3 install cpp-coveralls

COPY graphene-run-tests.sh /root
RUN chmod +x /root/graphene-run-tests.sh
