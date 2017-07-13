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

namespace {
  VALUE number_to_ruby(long number) {
    return LONG2NUM(number);
  }

  VALUE number_to_ruby(LONG_LONG number) {
    return LL2NUM(number);
  }

  VALUE py_object_to_ruby(PyObject *py_object) {
    using number_type =
      std::conditional<sizeof(long) >= sizeof(void *), long, LONG_LONG>::type;
    auto rb_pointer = number_to_ruby(reinterpret_cast<number_type>(py_object));
    auto rbFFI = rb_const_get(rb_cObject, rb_intern("FFI"));
    auto rbFFIPointer = rb_const_get(rbFFI, rb_intern("Pointer"));
    auto rb_ffi_pointer =
      rb_funcall(rbFFIPointer, rb_intern("new"), 1, rb_pointer);
    auto rbPyCall = rb_const_get(rb_cObject, rb_intern("PyCall"));
    auto rbPyCallLibPython =
      rb_const_get(rbPyCall, rb_intern("LibPython"));
    auto rbPyCallLibPythonPyObjectStruct =
      rb_const_get(rbPyCallLibPython, rb_intern("PyObjectStruct"));
    auto rb_py_object_struct =
      rb_funcall(rbPyCallLibPythonPyObjectStruct,
                 rb_intern("new"), 1, rb_ffi_pointer);
    return rb_funcall(rb_py_object_struct, rb_intern("to_ruby"), 0);
  }

  template <typename RETURN_TYPE>
  RETURN_TYPE ruby_to_number(VALUE rb_number);

  template <>
  long ruby_to_number(VALUE rb_number) {
    return NUM2LONG(rb_number);
  }

  template <>
  LONG_LONG ruby_to_number(VALUE rb_number) {
    return NUM2LL(rb_number);
  }

  void *ruby_to_pointer(VALUE rb_number) {
    using number_type =
      std::conditional<sizeof(long) >= sizeof(void *), long, LONG_LONG>::type;
    number_type number = ruby_to_number<number_type>(rb_number);
    return reinterpret_cast<void *>(number);
  }

  PyObject *pycall_object_to_py_object(VALUE rb_pycall_object) {
    auto rb_pyobj = rb_funcall(rb_pycall_object, rb_intern("__pyobj__"), 0);
    auto rb_pyobj_ffi_pointer = rb_funcall(rb_pyobj, rb_intern("pointer"), 0);
    auto rb_pyobj_pointer =
      rb_funcall(rb_pyobj_ffi_pointer, rb_intern("to_i"), 0);
    return static_cast<PyObject *>(ruby_to_pointer(rb_pyobj_pointer));
  }
}

extern "C" void Init_arrow_pycall(void);

static VALUE
rb_arrow_buffer_to_python(VALUE self)
{
  auto buffer = GARROW_BUFFER(RVAL2GOBJ(self));
  auto arrow_buffer = garrow_buffer_get_raw(buffer);
  auto py_buffer = arrow::py::wrap_buffer(arrow_buffer);
  return py_object_to_ruby(py_buffer);
}

static VALUE
rb_pycall_arrow_buffer_to_ruby(VALUE self)
{
  auto py_buffer = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Buffer> arrow_buffer;
  auto status = arrow::py::unwrap_buffer(py_buffer, &arrow_buffer);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][buffer][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_buffer = garrow_buffer_new_raw(&arrow_buffer);
  return GOBJ2RVAL(garrow_buffer);
}

static VALUE
rb_arrow_data_type_to_python(VALUE self)
{
  auto data_type = GARROW_DATA_TYPE(RVAL2GOBJ(self));
  auto arrow_data_type = garrow_data_type_get_raw(data_type);
  auto py_data_type = arrow::py::wrap_data_type(arrow_data_type);
  return py_object_to_ruby(py_data_type);
}

static VALUE
rb_pycall_arrow_data_type_to_ruby(VALUE self)
{
  auto py_data_type = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::DataType> arrow_data_type;
  auto status = arrow::py::unwrap_data_type(py_data_type, &arrow_data_type);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][data-type][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_data_type = garrow_data_type_new_raw(&arrow_data_type);
  return GOBJ2RVAL(garrow_data_type);
}

static VALUE
rb_arrow_field_to_python(VALUE self)
{
  auto field = GARROW_FIELD(RVAL2GOBJ(self));
  auto arrow_field = garrow_field_get_raw(field);
  auto py_field = arrow::py::wrap_field(arrow_field);
  return py_object_to_ruby(py_field);
}

static VALUE
rb_pycall_arrow_field_to_ruby(VALUE self)
{
  auto py_field = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Field> arrow_field;
  auto status = arrow::py::unwrap_field(py_field, &arrow_field);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][field][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_field = garrow_field_new_raw(&arrow_field);
  return GOBJ2RVAL(garrow_field);
}

static VALUE
rb_arrow_schema_to_python(VALUE self)
{
  auto schema = GARROW_SCHEMA(RVAL2GOBJ(self));
  auto arrow_schema = garrow_schema_get_raw(schema);
  auto py_schema = arrow::py::wrap_schema(arrow_schema);
  return py_object_to_ruby(py_schema);
}

static VALUE
rb_pycall_arrow_schema_to_ruby(VALUE self)
{
  auto py_schema = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Schema> arrow_schema;
  auto status = arrow::py::unwrap_schema(py_schema, &arrow_schema);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][schema][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_schema = garrow_schema_new_raw(&arrow_schema);
  return GOBJ2RVAL(garrow_schema);
}

static VALUE
rb_arrow_array_to_python(VALUE self)
{
  auto array = GARROW_ARRAY(RVAL2GOBJ(self));
  auto arrow_array = garrow_array_get_raw(array);
  auto py_array = arrow::py::wrap_array(arrow_array);
  return py_object_to_ruby(py_array);
}

static VALUE
rb_pycall_arrow_array_to_ruby(VALUE self)
{
  auto py_array = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Array> arrow_array;
  auto status = arrow::py::unwrap_array(py_array, &arrow_array);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][array][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_array = garrow_array_new_raw(&arrow_array);
  return GOBJ2RVAL(garrow_array);
}

static VALUE
rb_arrow_tensor_to_python(VALUE self)
{
  auto tensor = GARROW_TENSOR(RVAL2GOBJ(self));
  auto arrow_tensor = garrow_tensor_get_raw(tensor);
  auto py_tensor = arrow::py::wrap_tensor(arrow_tensor);
  return py_object_to_ruby(py_tensor);
}

static VALUE
rb_pycall_arrow_tensor_to_ruby(VALUE self)
{
  auto py_tensor = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Tensor> arrow_tensor;
  auto status = arrow::py::unwrap_tensor(py_tensor, &arrow_tensor);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][tensor][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_tensor = garrow_tensor_new_raw(&arrow_tensor);
  return GOBJ2RVAL(garrow_tensor);
}

static VALUE
rb_arrow_column_to_python(VALUE self)
{
  auto column = GARROW_COLUMN(RVAL2GOBJ(self));
  auto arrow_column = garrow_column_get_raw(column);
  auto py_column = arrow::py::wrap_column(arrow_column);
  return py_object_to_ruby(py_column);
}

static VALUE
rb_pycall_arrow_column_to_ruby(VALUE self)
{
  auto py_column = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Column> arrow_column;
  auto status = arrow::py::unwrap_column(py_column, &arrow_column);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][column][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_column = garrow_column_new_raw(&arrow_column);
  return GOBJ2RVAL(garrow_column);
}

static VALUE
rb_arrow_table_to_python(VALUE self)
{
  auto table = GARROW_TABLE(RVAL2GOBJ(self));
  auto arrow_table = garrow_table_get_raw(table);
  auto py_table = arrow::py::wrap_table(arrow_table);
  return py_object_to_ruby(py_table);
}

static VALUE
rb_pycall_arrow_table_to_ruby(VALUE self)
{
  auto py_table = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::Table> arrow_table;
  auto status = arrow::py::unwrap_table(py_table, &arrow_table);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][table][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_table = garrow_table_new_raw(&arrow_table);
  return GOBJ2RVAL(garrow_table);
}

static VALUE
rb_arrow_record_batch_to_python(VALUE self)
{
  auto record_batch = GARROW_RECORD_BATCH(RVAL2GOBJ(self));
  auto arrow_record_batch = garrow_record_batch_get_raw(record_batch);
  auto py_record_batch = arrow::py::wrap_record_batch(arrow_record_batch);
  return py_object_to_ruby(py_record_batch);
}

static VALUE
rb_pycall_arrow_record_batch_to_ruby(VALUE self)
{
  auto py_record_batch = pycall_object_to_py_object(self);
  std::shared_ptr<arrow::RecordBatch> arrow_record_batch;
  auto status =
    arrow::py::unwrap_record_batch(py_record_batch, &arrow_record_batch);
  GError *error = NULL;
  if (!garrow_error_check(&error, status, "[pyarrow][record-batch][to-ruby]"))
    RG_RAISE_ERROR(error);
  auto garrow_record_batch = garrow_record_batch_new_raw(&arrow_record_batch);
  return GOBJ2RVAL(garrow_record_batch);
}

extern "C" void
Init_arrow_pycall(void)
{
  arrow::py::import_pyarrow();

  auto rbArrow = rb_const_get(rb_cObject, rb_intern("Arrow"));
  auto rbPyArrow = rb_const_get(rb_cObject, rb_intern("PyArrow"));

  {
    auto rbArrowBuffer = rb_const_get(rbArrow, rb_intern("Buffer"));
    rb_define_method(rbArrowBuffer,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_buffer_to_python,
                     0);
  }
  {
    auto rbPyArrowBuffer = rb_const_get(rbPyArrow, rb_intern("Buffer"));
    rb_define_method(rbPyArrowBuffer,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_buffer_to_ruby,
                     0);
  }

  {
    auto rbArrowDataType = rb_const_get(rbArrow, rb_intern("DataType"));
    rb_define_method(rbArrowDataType,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_data_type_to_python,
                     0);
  }
  {
    auto rbPyArrowDataType = rb_const_get(rbPyArrow, rb_intern("DataType"));
    rb_define_method(rbPyArrowDataType,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_data_type_to_ruby,
                     0);
  }

  {
    auto rbArrowField = rb_const_get(rbArrow, rb_intern("Field"));
    rb_define_method(rbArrowField,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_field_to_python,
                     0);
  }
  {
    auto rbPyArrowField = rb_const_get(rbPyArrow, rb_intern("Field"));
    rb_define_method(rbPyArrowField,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_field_to_ruby,
                     0);
  }

  {
    auto rbArrowSchema = rb_const_get(rbArrow, rb_intern("Schema"));
    rb_define_method(rbArrowSchema,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_schema_to_python,
                     0);
  }
  {
    auto rbPyArrowSchema = rb_const_get(rbPyArrow, rb_intern("Schema"));
    rb_define_method(rbPyArrowSchema,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_schema_to_ruby,
                     0);
  }

  {
    auto rbArrowArray = rb_const_get(rbArrow, rb_intern("Array"));
    rb_define_method(rbArrowArray,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_array_to_python,
                     0);
  }
  {
    auto rbPyArrowArray = rb_const_get(rbPyArrow, rb_intern("Array"));
    rb_define_method(rbPyArrowArray,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_array_to_ruby,
                     0);
  }

  {
    auto rbArrowTensor = rb_const_get(rbArrow, rb_intern("Tensor"));
    rb_define_method(rbArrowTensor,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_tensor_to_python,
                     0);
  }
  {
    auto rbPyArrowTensor = rb_const_get(rbPyArrow, rb_intern("Tensor"));
    rb_define_method(rbPyArrowTensor,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_tensor_to_ruby,
                     0);
  }

  {
    auto rbArrowColumn = rb_const_get(rbArrow, rb_intern("Column"));
    rb_define_method(rbArrowColumn,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_column_to_python,
                     0);
  }
  {
    auto rbPyArrowColumn = rb_const_get(rbPyArrow, rb_intern("Column"));
    rb_define_method(rbPyArrowColumn,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_column_to_ruby,
                     0);
  }

  {
    auto rbArrowTable = rb_const_get(rbArrow, rb_intern("Table"));
    rb_define_method(rbArrowTable,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_table_to_python,
                     0);
  }
  {
    auto rbPyArrowTable = rb_const_get(rbPyArrow, rb_intern("Table"));
    rb_define_method(rbPyArrowTable,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_table_to_ruby,
                     0);
  }

  {
    auto rbArrowRecordBatch = rb_const_get(rbArrow, rb_intern("RecordBatch"));
    rb_define_method(rbArrowRecordBatch,
                     "to_python",
                     (VALUE(*)(ANYARGS))rb_arrow_record_batch_to_python,
                     0);
  }
  {
    auto rbPyArrowRecordBatch =
      rb_const_get(rbPyArrow, rb_intern("RecordBatch"));
    rb_define_method(rbPyArrowRecordBatch,
                     "to_ruby",
                     (VALUE(*)(ANYARGS))rb_pycall_arrow_record_batch_to_ruby,
                     0);
  }
}
