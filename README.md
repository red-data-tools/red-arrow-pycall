# README

## Name

Red Arrow PyCall

## Description

Red Arrow PyCall is a library that provides converters between Ruby objects for Apache Arrow and Python objects for Apache Arrow.

Red Arrow PyCall adds `#to_python` method to most Ruby objects for Apache Arrow such as `Arrow::Array`. Red Arrow PyCall adds `to_ruby` method to most Python objects for Apache Arrow such as `pyarrow.lib.Int8Array`.

## Install

```text
% gem install red-arrow-pycall
```

## Usage

```ruby
require "arrow-pycall"

arrow_int8_array.to_python # -> An object of PyCall::PyObject that wraps pyarrow.lib.Int8Array

py_arrow_int8_array.to_ruby # -> An object of Arrow::Int8Array
```

## Dependencies

* [Red Arrow](https://github.com/red-data-tools/red-arrow)

* [PyCall](https://github.com/mrkn/pycall)

## Authors

* Kouhei Sutou \<kou@clear-code.com\>

## License

Apache License 2.0. See doc/text/apache-2.0.txt for details.

(Kouhei Sutou has a right to change the license including contributed
patches.)
