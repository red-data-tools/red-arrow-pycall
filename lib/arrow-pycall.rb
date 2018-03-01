# Copyright 2017 Kouhei Sutou <kou@clear-code.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require "arrow"
require "pycall"

require "arrow-pycall/version"

module PyArrow
  class << self
    def __pyptr__
      @pyarrow ||= PyCall.import_module("pyarrow")
    end
  end

  Buffer = __pyptr__.Buffer
  class Buffer
    register_python_type_mapping
  end

  DataType = __pyptr__.lib.DataType
  class DataType
    register_python_type_mapping
  end

  Field = __pyptr__.Field
  class Field
    register_python_type_mapping
  end

  Schema = __pyptr__.Schema
  class Schema
    register_python_type_mapping
  end

  Tensor = __pyptr__.Tensor
  class Tensor
    register_python_type_mapping
  end

  Column = __pyptr__.Column
  class Column
    register_python_type_mapping
  end

  Table = __pyptr__.Table
  class Table
    register_python_type_mapping
  end

  RecordBatch = __pyptr__.RecordBatch
  class RecordBatch
    register_python_type_mapping
  end
end

begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/arrow_pycall.so"
rescue LoadError
  require "arrow_pycall.so"
end

module PyArrow
  Arrow.constants.each do |name|
    case name.to_s
    when "ChunkedArray"
    when /Array\z/
      py_arrow_array = __pyptr__.lib.__dict__[name.to_s]
      next if py_arrow_array.nil?
      py_arrow_array.__send__(:register_python_type_mapping)
      py_arrow_array.__send__(:include, ArrayConvertable)
    end
  end
end
