FROM rubylang/ruby

MAINTAINER Kouhei Sutou <kou@clear-code.com>

RUN \
  apt update --quiet && \
  apt install --quiet -y apt-transport-https curl gnupg2 lsb-release wget && \
  wget -O /usr/share/keyrings/apache-arrow-keyring.gpg \
    https://dl.bintray.com/apache/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-keyring.gpg && \
  echo "deb [arch=amd64 signed-by=/usr/share/keyrings/apache-arrow-keyring.gpg] https://dl.bintray.com/apache/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/ $(lsb_release --codename --short) main" > \
    /etc/apt/sources.list.d/apache-arrow.list && \
  curl https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
  echo "deb http://apt.llvm.org/$(lsb_release --codename --short)/ llvm-toolchain-$(lsb_release --codename --short)-7 main" > \
    /etc/apt/sources.list.d/llvm.list && \
  apt update --quiet && \
  apt install --quiet -y \
    cmake \
    libarrow-glib-dev \
    libarrow-python-dev \
    libparquet-dev \
    libparquet-glib-dev \
    libgirepository1.0-dev \
    python3-pip \
    python3-setuptools && \
  rm -rf /var/lib/apt/lists/*

RUN pip3 install --no-binary :all: Cython && \
    pip3 install --no-binary :all: pyarrow && \
    pip3 install pandas

RUN mkdir /app
WORKDIR /app
COPY . /app
RUN bundle install

ENV PYTHON=python3
