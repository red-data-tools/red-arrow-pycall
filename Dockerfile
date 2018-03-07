FROM ruby:2.5-stretch

MAINTAINER Kouhei Sutou <kou@clear-code.com>

RUN \
  apt update && \
  apt install -y apt-transport-https && \
  echo "deb https://packages.red-data-tools.org/debian/ stretch main" > \
    /etc/apt/sources.list.d/red-data-tools.list && \
  apt update --allow-insecure-repositories && \
  apt install -y --allow-unauthenticated red-data-tools-keyring && \
  apt update && \
  apt install -y \
    cmake \
    cython3 \
    libarrow-glib-dev \
    libarrow-python-dev \
    python3-setuptools \
    python3-six

RUN \
  git clone --depth 1 https://github.com/apache/arrow.git && \
  (cd arrow/python && \
   python3 setup.py install) && \
  rm -rf arrow

RUN mkdir /app
WORKDIR /app
COPY . /app
RUN bundle install
