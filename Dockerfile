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
    libarrow-python-dev \
    libparquet-dev \
    python3-pip \
    python3-setuptools

RUN \
  pip3 install \
    pyarrow

RUN mkdir /app
WORKDIR /app
COPY . /app
RUN bundle install

ENV PYTHON=python3
