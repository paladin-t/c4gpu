/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#ifndef __C4G_UTILS_H__
#define __C4G_UTILS_H__

#include "c4g_runtime.h"
#include <list>
#include <string>

#ifndef C4G_COUNTOF
#	define C4G_COUNTOF(__a) (sizeof(__a) / sizeof(*(__a)))
#endif /* C4G_COUNTOF */

#ifndef C4GRT_BUFFER_SIZE
#	define C4GRT_BUFFER_SIZE (1024 * 64)
#endif /* C4GRT_BUFFER_SIZE */

namespace c4g {

namespace gl {

typedef unsigned char Byte;

typedef unsigned int UInt;

class C4G_RUNTIME_IMPL Buffer final {

public:
	Buffer();
	Buffer(Buffer &&o);
	~Buffer();

	Buffer &operator = (Buffer &&o);

	size_t map(const C4GRT_Data &in);
	size_t fill(const Byte* const in, size_t is);

	void resize(size_t s, size_t es);
	size_t count(void) const;
	size_t size(void) const;
	size_t sizePerElement(void) const;
	size_t countOfGLfloat(void) const;
	size_t countOfGLint(void) const;

	C4GRT_DataTypes type(void) const;

	unsigned char* ptr(void) const;
	template<typename T> T* ptr(void) const {
		return static_cast<T*>(ptr());
	}

	const char* name(void) const;

	const UInt &id(void) const;
	UInt &id(void);

private:
	void tidy(void);

private:
	C4GRT_DataTypes _type = DT_INVALID;
	size_t _allocated = 0;
	size_t _size = 0;
	size_t _sizePerElement = 0;
	unsigned char _bytes[C4GRT_BUFFER_SIZE];
	unsigned char* _pointer = nullptr;
	bool _mapped = false;
	std::string _name;
	UInt _id = 0;

};

class C4G_RUNTIME_IMPL BufferList final {

public:
	typedef std::list<Buffer> List;

public:
	BufferList();
	~BufferList();

	BufferList &push(void);
	BufferList &clear(void);
	bool empty(void) const;
	size_t count(void) const;
	size_t totalSize(void) const;
	size_t sizePerElement(void) const;

	const Buffer &front(void) const;
	Buffer &front(void);
	const Buffer &back(void) const;
	Buffer &back(void);

	List::iterator begin(void);
	List::const_iterator begin(void) const;
	List::iterator end(void);
	List::const_iterator end(void) const;

private:
	List _list;

};

}

}

#endif /* __C4G_UTILS_H__ */
