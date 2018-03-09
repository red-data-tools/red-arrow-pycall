FROM ruby:2.5-stretch

MAINTAINER Kouhei Sutou <kou@clear-code.com>

RUN \
  apt update --quiet && \
  apt install --quiet -y apt-transport-https && \
  echo "deb https://packages.red-data-tools.org/debian/ stretch main" > \
    /etc/apt/sources.list.d/red-data-tools.list && \
  apt update --quiet --allow-insecure-repositories && \
  apt install --quiet -y --allow-unauthenticated red-data-tools-keyring && \
  apt update --quiet && \
  apt install --quiet -y \
    cmake \
    libarrow-glib-dev \
    libarrow-python-dev \
    python3-pip \
    python3-setuptools

RUN \
  pip3 install \
    cython \
    pandas

RUN \
  git clone \
    --depth 1 \
    https://github.com/apache/arrow.git && \
  (cd arrow/python && \
     python3 setup.py sdist && \
     pip3 install dist/pyarrow-*.tar.gz)

RUN mkdir /app
WORKDIR /app
COPY . /app
RUN bundle install

ENV PYTHON=python3
