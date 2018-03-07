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
  pip3 install cython

RUN \
  git clone \
    --depth 1 \
    --branch apache-arrow-0.8.0 \
    https://github.com/apache/arrow.git && \
  (cd arrow && \
    curl https://patch-diff.githubusercontent.com/raw/apache/arrow/pull/1721.diff | \
      patch -p1) && \
  (cd arrow/python && python3 setup.py install)

RUN mkdir /app
WORKDIR /app
COPY . /app
RUN bundle install

ENV PYTHON=python3
