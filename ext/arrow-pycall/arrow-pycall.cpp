/*
 * Copyright 2017 Kouhei Sutou <kou@clear-code.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <arrow-glib/arrow-glib.hpp>

#include <rbgobject.h>

#include <arrow/python/pyarrow.h>

template<bool long_size_is_greater_than_pointer_size>
struct pointer2value;

template<>
struct pointer2value<true> {
  VALUE operator()(void* ptr)
  {
    return LONG2NUM(reinterpret_cast<long>(ptr));
  }
};

template<>
struct pointer2value<false> {
  VALUE operator()(void* ptr)
  {
    return LL2NUM(reinterpret_cast<LONG_LONG>(ptr));
  }
};

static inline VALUE
PTR2NUM(void* ptr)
{
  typedef pointer2value<(sizeof(long) >= sizeof(void*))> converter_type;
  return converter_type()(ptr);
}

extern "C" void Init_arrow_pycall(void);

static VALUE
rb_arrow_buffer_to_python_object_pointer(VALUE self)
{
  auto buffer = GARROW_BUFFER(RVAL2GOBJ(self));
  auto arrow_buffer = garrow_buffer_get_raw(buffer);
  auto py_buffer = arrow::py::wrap_buffer(arrow_buffer);
  return PTR2NUM(py_buffer);
}

static VALUE
rb_arrow_data_type_to_python_object_pointer(VALUE self)
{
  auto data_type = GARROW_DATA_TYPE(RVAL2GOBJ(self));
  auto arrow_data_type = garrow_data_type_get_raw(data_type);
  auto py_data_type = arrow::py::wrap_data_type(arrow_data_type);
  return PTR2NUM(py_data_type);
}

static VALUE
rb_arrow_field_to_python_object_pointer(VALUE self)
{
  auto field = GARROW_FIELD(RVAL2GOBJ(self));
  auto arrow_field = garrow_field_get_raw(field);
  auto py_field = arrow::py::wrap_field(arrow_field);
  return PTR2NUM(py_field);
}

static VALUE
rb_arrow_schema_to_python_object_pointer(VALUE self)
{
  auto schema = GARROW_SCHEMA(RVAL2GOBJ(self));
  auto arrow_schema = garrow_schema_get_raw(schema);
  auto py_schema = arrow::py::wrap_schema(arrow_schema);
  return PTR2NUM(py_schema);
}

static VALUE
rb_arrow_array_to_python_object_pointer(VALUE self)
{
  auto array = GARROW_ARRAY(RVAL2GOBJ(self));
  auto arrow_array = garrow_array_get_raw(array);
  auto py_array = arrow::py::wrap_array(arrow_array);
  return PTR2NUM(py_array);
}

static VALUE
rb_arrow_tensor_to_python_object_pointer(VALUE self)
{
  auto tensor = GARROW_TENSOR(RVAL2GOBJ(self));
  auto arrow_tensor = garrow_tensor_get_raw(tensor);
  auto py_tensor = arrow::py::wrap_tensor(arrow_tensor);
  return PTR2NUM(py_tensor);
}

static VALUE
rb_arrow_column_to_python_object_pointer(VALUE self)
{
  auto column = GARROW_COLUMN(RVAL2GOBJ(self));
  auto arrow_column = garrow_column_get_raw(column);
  auto py_column = arrow::py::wrap_column(arrow_column);
  return PTR2NUM(py_column);
}

static VALUE
rb_arrow_table_to_python_object_pointer(VALUE self)
{
  auto table = GARROW_TABLE(RVAL2GOBJ(self));
  auto arrow_table = garrow_table_get_raw(table);
  auto py_table = arrow::py::wrap_table(arrow_table);
  return PTR2NUM(py_table);
}

static VALUE
rb_arrow_record_batch_to_python_object_pointer(VALUE self)
{
  auto record_batch = GARROW_RECORD_BATCH(RVAL2GOBJ(self));
  auto arrow_record_batch = garrow_record_batch_get_raw(record_batch);
  auto py_record_batch = arrow::py::wrap_record_batch(arrow_record_batch);
  return PTR2NUM(py_record_batch);
}

extern "C" void
Init_arrow_pycall(void)
{
  arrow::py::import_pyarrow();

  VALUE rbArrow = rb_const_get(rb_cObject, rb_intern("Arrow"));

  {
    VALUE rbArrowBuffer = rb_const_get(rbArrow, rb_intern("Buffer"));
    rb_define_method(rbArrowBuffer,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_buffer_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowDataType = rb_const_get(rbArrow, rb_intern("DataType"));
    rb_define_method(rbArrowDataType,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_data_type_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowField = rb_const_get(rbArrow, rb_intern("Field"));
    rb_define_method(rbArrowField,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_field_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowSchema = rb_const_get(rbArrow, rb_intern("Schema"));
    rb_define_method(rbArrowSchema,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_schema_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowArray = rb_const_get(rbArrow, rb_intern("Array"));
    rb_define_method(rbArrowArray,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_array_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowTensor = rb_const_get(rbArrow, rb_intern("Tensor"));
    rb_define_method(rbArrowTensor,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_tensor_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowColumn = rb_const_get(rbArrow, rb_intern("Column"));
    rb_define_method(rbArrowColumn,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_column_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowTable = rb_const_get(rbArrow, rb_intern("Table"));
    rb_define_method(rbArrowTable,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_table_to_python_object_pointer,
                     0);
  }

  {
    VALUE rbArrowRecordBatch = rb_const_get(rbArrow, rb_intern("RecordBatch"));
    rb_define_method(rbArrowRecordBatch,
                     "to_python_object_pointer",
                     (VALUE(*)(ANYARGS))rb_arrow_record_batch_to_python_object_pointer,
                     0);
  }
}
