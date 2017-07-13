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

class ToRubyTest < Test::Unit::TestCase
  test("Buffer") do
    data = "hello"
    buffer = Arrow::Buffer.new(data)
    py_buffer = buffer.to_python
    assert_equal(data, py_buffer.to_ruby.data.to_s)
  end

  test("DataType") do
    data_type = Arrow::BooleanDataType.new
    py_data_type = data_type.to_python
    assert_equal(data_type.to_s, py_data_type.to_ruby.to_s)
  end

  test("Field") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    assert_equal(field.to_s,
                 field.to_python.to_ruby.to_s)
  end

  test("Schema") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    schema = Arrow::Schema.new([field])
    assert_equal(schema.to_s,
                 schema.to_python.to_ruby.to_s)
  end

  test("Array") do
    elements = [1, -2, 4]
    array = Arrow::Int8Array.new(elements)
    assert_equal(elements,
                 array.to_python.to_ruby.to_a)
  end

  test("Tensor") do
    raw_data = [
      1, 2,
      3, 4,

      5, 6,
      7, 8,

      9, 10,
      11, 12,
    ]
    data = Arrow::Buffer.new(raw_data.pack("c*"))
    shape = [3, 2, 2]
    strides = []
    names = ["a", "b", "c"]
    tensor = Arrow::Tensor.new(Arrow::Int8DataType.new,
                               data,
                               shape,
                               strides,
                               names)
    assert_equal(tensor.buffer.data.to_s,
                 tensor.to_python.to_ruby.buffer.data.to_s)
  end

  test("Column") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    array = Arrow::BooleanArray.new([true, false, true])
    column = Arrow::Column.new(field, array)
    assert_equal([true, false, true],
                 column.to_python.to_ruby.to_a)
  end

  test("Table") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    schema = Arrow::Schema.new([field])
    array = Arrow::BooleanArray.new([true, false, true])
    column = Arrow::Column.new(field, array)
    table = Arrow::Table.new(schema, [column])
    assert_equal(table.each_column.collect(&:to_a),
                 table.to_python.to_ruby.each_column.collect(&:to_a))
  end

  test("RecordBatch") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    schema = Arrow::Schema.new([field])
    array = Arrow::BooleanArray.new([true, false, true])
    record_batch = Arrow::RecordBatch.new(schema, 3, [array])
    assert_equal(record_batch.to_s,
                 record_batch.to_python.to_ruby.to_s)
  end
end
