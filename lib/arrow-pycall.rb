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

begin
  major, minor, _ = RUBY_VERSION.split(/\./)
  require "#{major}.#{minor}/arrow_pycall.so"
rescue LoadError
  require "arrow_pycall.so"
end

require "arrow-pycall/convertable"

module Arrow
  class Buffer
    include ArrowPyCall::Convertable
  end

  class DataType
    include ArrowPyCall::Convertable
  end

  class Field
    include ArrowPyCall::Convertable
  end

  class Schema
    include ArrowPyCall::Convertable
  end

  class Array
    include ArrowPyCall::Convertable
  end

  class Tensor
    include ArrowPyCall::Convertable
  end

  class Column
    include ArrowPyCall::Convertable
  end

  class Table
    include ArrowPyCall::Convertable
  end

  class RecordBatch
    include ArrowPyCall::Convertable
  end
end
