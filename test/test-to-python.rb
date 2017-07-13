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

class ToPythonTest < Test::Unit::TestCase
  test("Buffer") do
    data = "hello"
    buffer = Arrow::Buffer.new(data)
    assert_equal(data, buffer.to_python.to_pybytes.())
  end

  test("DataType") do
    data_type = Arrow::BooleanDataType.new
    assert_equal(data_type.to_s,
                 data_type.to_python.__str__.())
  end

  test("Field") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    assert_equal("pyarrow.Field<#{field}>",
                 field.to_python.__str__.())
  end

  test("Schema") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    schema = Arrow::Schema.new([field])
    assert_equal(schema.to_s,
                 schema.to_python.__str__.())
  end

  test("Array") do
    elements = [1, -2, 4]
    array = Arrow::Int8Array.new(elements)
    assert_equal(elements,
                 array.to_python.to_pylist.())
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
    assert_equal(<<-TENSOR.chomp, tensor.to_python.to_numpy.().__str__.())
[[[ 1  2]
  [ 3  4]]

 [[ 5  6]
  [ 7  8]]

 [[ 9 10]
  [11 12]]]
    TENSOR
  end

  test("Column") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    array = Arrow::BooleanArray.new([true, false, true])
    column = Arrow::Column.new(field, array)
    assert_equal([true, false, true],
                 column.to_python.to_pylist.())
  end

  test("Table") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    schema = Arrow::Schema.new([field])
    array = Arrow::BooleanArray.new([true, false, true])
    column = Arrow::Column.new(field, array)
    table = Arrow::Table.new(schema, [column])
    assert_equal(<<-TABLE.chomp, table.to_python.to_pandas.().to_s)
   enabled
0     True
1    False
2     True
    TABLE
  end

  test("RecordBatch") do
    field = Arrow::Field.new("enabled", Arrow::BooleanDataType.new)
    schema = Arrow::Schema.new([field])
    array = Arrow::BooleanArray.new([true, false, true])
    record_batch = Arrow::RecordBatch.new(schema, 3, [array])
    assert_equal(<<-RECORD_BATCH.chomp, record_batch.to_python.to_pandas.().to_s)
   enabled
0     True
1    False
2     True
    RECORD_BATCH
  end
end
