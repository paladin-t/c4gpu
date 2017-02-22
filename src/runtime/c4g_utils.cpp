/*
** The C4GPU (C for GPU) System.
**
** For the latest info, see https://github.com/c4gpu/c4gpu/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#include "c4g_utils.hpp"
#include <GL/glew.h>
#include <algorithm>

namespace c4g {

namespace gl {

Buffer::Buffer() {
	memset(_bytes, 0, sizeof(_bytes));
	_pointer = _bytes;
}

Buffer::Buffer(Buffer &&o) {
	_type = o._type;
	_allocated = o._allocated;
	_size = o._size;
	_sizePerElement = o._sizePerElement;
	memcpy(_bytes, o._bytes, sizeof(_bytes));
	if (o._pointer == o._bytes) _pointer = _bytes;
	else _pointer = o._pointer;
	_mapped = o._mapped;
	_name = o._name;
	_id = o._id;

	o._type = DT_INVALID;
	o._allocated = 0;
	o._size = 0;
	o._sizePerElement = 0;
	o._pointer = nullptr;
	o._mapped = false;
	o._name.clear();
	o._id = 0;
}

Buffer::~Buffer() {
	tidy();
}

Buffer &Buffer::operator = (Buffer &&o) {
	_type = o._type;
	_allocated = o._allocated;
	_size = o._size;
	_sizePerElement = o._sizePerElement;
	memcpy(_bytes, o._bytes, sizeof(_bytes));
	if (o._pointer == o._bytes) _pointer = _bytes;
	else _pointer = o._pointer;
	_mapped = o._mapped;
	_name = o._name;
	_id = o._id;

	o._type = DT_INVALID;
	o._allocated = 0;
	o._size = 0;
	o._sizePerElement = 0;
	o._pointer = nullptr;
	o._mapped = false;
	o._name.clear();
	o._id = 0;

	return *this;
}

size_t Buffer::map(const C4GRT_Data &in) {
	tidy();
	_type = in._type;
	_mapped = true;
	_pointer = in._bytes;
	_size = c4grt_data_size(&in);
	_sizePerElement = c4grt_data_size_per_element(&in);
	if (in._name) _name = in._name;
	else _name.clear();

	return _size;
}

size_t Buffer::fill(const Byte* const in, size_t is) {
	size_t ret = std::min(_size, is);
	memcpy(_pointer, in, ret);

	return ret;
}

void Buffer::resize(size_t s, size_t es) {
	if (_mapped)
		tidy();
	if (_pointer == _bytes) {
		if (s <= C4GRT_BUFFER_SIZE) {
			_size = s;
		} else {
			_pointer = (unsigned char*)malloc(s);
			_allocated = s;
			_size = s;
		}
	} else {
		if (s <= _allocated) {
			_size = s;
		} else {
			_pointer = (unsigned char*)realloc(_pointer, s);
			_allocated = s;
			_size = s;
		}
	}
	_sizePerElement = es;
}

size_t Buffer::count(void) const {
	return _size / _sizePerElement;
}

size_t Buffer::size(void) const {
	return _size;
}

size_t Buffer::sizePerElement(void) const {
	return _sizePerElement;
}

size_t Buffer::countOfGLfloat(void) const {
	return _size / sizeof(GLfloat);
}

size_t Buffer::countOfGLint(void) const {
	return _size / sizeof(GLint);
}

C4GRT_DataTypes Buffer::type(void) const {
	return _type;
}

unsigned char* Buffer::ptr(void) const {
	return _pointer;
}

const char* Buffer::name(void) const {
	if (_name.empty()) return nullptr;

	return _name.c_str();
}

const UInt &Buffer::id(void) const {
	return _id;
}

UInt &Buffer::id(void) {
	return _id;
}

void Buffer::tidy(void) {
	_type = DT_INVALID;
	if (!_mapped && _pointer != _bytes)
		free(_pointer);
	_allocated = _size = 0;
	_sizePerElement = 0;
	_pointer = _bytes;
	_mapped = false;
}

BufferList::BufferList() {
}

BufferList::~BufferList() {
}

BufferList &BufferList::push(void) {
	_list.push_back(Buffer());

	return *this;
}

BufferList &BufferList::clear(void) {
	_list.clear();

	return *this;
}

bool BufferList::empty(void) const {
	return _list.empty();
}

size_t BufferList::count(void) const {
	return _list.size();
}

size_t BufferList::totalSize(void) const {
	size_t result = 0;
	for (const Buffer &b : _list)
		result += b.size();

	return result;
}

size_t BufferList::sizePerElement(void) const {
	size_t result = 0;
	for (const Buffer &b : _list)
		result += b.sizePerElement();

	return result;
}

const Buffer &BufferList::front(void) const {
	return _list.front();
}

Buffer &BufferList::front(void) {
	return _list.front();
}

const Buffer &BufferList::back(void) const {
	return _list.back();
}

Buffer &BufferList::back(void) {
	return _list.back();
}

BufferList::List::iterator BufferList::begin(void) {
	return _list.begin();
}

BufferList::List::const_iterator BufferList::begin(void) const {
	return _list.begin();
}

BufferList::List::iterator BufferList::end(void) {
	return _list.end();
}

BufferList::List::const_iterator BufferList::end(void) const {
	return _list.end();
}

}

}
