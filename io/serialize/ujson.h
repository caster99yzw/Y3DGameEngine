/*
 * Copyright (c) 2014 Anders Wang Kristensen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <cstdint>
#include <iosfwd>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined(NEOX_PS4)
#include <assert.h>
#endif

namespace ujson {

#ifdef _LIBCPP_VERSION
#define UJSON_SHORT_STRING_OPTIMIZATION // libc++
#ifdef __LP64__
enum { sso_max_length = 22 }; // 22 bytes on 64 bit
#else
enum { sso_max_length = 10 }; // 10 bytes on 32 bit
#endif
#elif defined _CPPLIB_VER
#define UJSON_SHORT_STRING_OPTIMIZATION // msvc + dinkumware stl
enum { sso_max_length = 15 }; // 15 bytes on both 32/64 bit
#elif defined __GLIBCXX__
#define UJSON_REF_COUNTED_STRING // libstdc++
#else
#error Unrecognized STL library.
#endif

enum class value_type { null, boolean, number, string, array, object };
enum class value_impl_type { null, boolean, number, short_string, long_string, string, array, object };

class value;
class string_view;

using string = std::string;
using array = std::vector<value>;
using name_value_pair = std::pair<std::string, value>;
using object = std::vector<name_value_pair>;

class value final {
public:
    // construct null value
    value() noexcept;

    // construct boolean value
    value(bool b) noexcept;

    // construct double value; null if not finite
    value(double d) noexcept;

    // construct 32 bit signed int value
    value(std::int32_t d) noexcept;

    // construct 32 bit unsigned int value
    value(std::uint32_t d) noexcept;

    // construct string value; null if invalid utf-8
    value(string const &str);
    value(string &&str);

    // construct string value; if len==0 ptr must be zero terminated
    value(const char *ptr, std::size_t len = 0);

    // construct array value
    value(array const &a);
    value(array &&a);

    // construct object value
    value(object const &o);
    value(object &&o);

    value(value const &) noexcept;
    value(value &&) noexcept;
    value(const void *) = delete;

    // construct from array<T> if T is convertible to value
    template <typename T>
    value(const std::vector<T> &a,
          const typename std::enable_if<
              std::is_convertible<T, value>::value>::type *p = nullptr);

    // construct from array<T> if suitable to_json(T) method exists
    template <typename T>
    value(const std::vector<T> &a,
          const typename std::enable_if<std::is_same<
              value, decltype(to_json(std::declval<T>()))>::value>::type *p =
              nullptr);

    // construct from map<string,T> if T is convertible to value
    template <typename T>
    value(const std::map<std::string, T> &o,
          const typename std::enable_if<
              std::is_convertible<T, value>::value>::type *p = nullptr);

    // construct from map<string,T> if suitable to_json(T) method exists
    template <typename T>
    value(const std::map<std::string, T> &o,
          const typename std::enable_if<std::is_same<
              value, decltype(to_json(std::declval<T>()))>::value>::type *p =
              nullptr);

    value &operator=(bool) noexcept;
    value &operator=(double) noexcept;
    value &operator=(std::int32_t) noexcept;
    value &operator=(std::uint32_t) noexcept;
    value &operator=(string const &) noexcept;
    value &operator=(string &&) noexcept;
    value &operator=(const char *) noexcept;
    value &operator=(array const &);
    value &operator=(array &&);
    value &operator=(object const &);
    value &operator=(object &&);
    value &operator=(value const &) noexcept;
    value &operator=(value &&) noexcept;
    value &operator=(const void *) = delete;

    ~value();

    bool is_null() const noexcept;
    bool is_boolean() const noexcept;
    bool is_number() const noexcept;
    bool is_string() const noexcept;
    bool is_array() const noexcept;
    bool is_object() const noexcept;

	bool to_boolean(bool fallback = 0) const noexcept;
	int32_t to_int32(int32_t fallback = 0) const noexcept;
	uint32_t to_uint32(uint32_t fallback=0) const noexcept;
	float to_float(float fallback = 0.0f) const noexcept;
	double to_double(double fallback = 0.0) const noexcept;
	string_view to_string(const char *fallback="") const noexcept;
	array *to_array() noexcept;
	object *to_object() noexcept;
	const array *to_array() const noexcept;
	const object *to_object() const noexcept;

    value_type type() const noexcept;

    void swap(value &other) noexcept;

private:
    friend bool operator==(const value &lhs, const value &rhs);
    friend bool operator!=(const value &lhs, const value &rhs);

    struct impl_t {
        virtual ~impl_t() = 0;
        virtual value_type type() const noexcept = 0;
		virtual value_impl_type impl_type() const noexcept = 0;
        virtual void clone(char *storage) const noexcept = 0;
        virtual bool equals(const impl_t *ptr) const noexcept = 0;
    };

    struct null_impl_t : impl_t {
        null_impl_t() noexcept;
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
    };

    struct boolean_impl_t : impl_t {
        boolean_impl_t(bool b) noexcept;
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        bool boolean;
    };

    struct number_impl_t : impl_t {
        number_impl_t(double n) noexcept;
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        double number;
    };

#ifdef UJSON_SHORT_STRING_OPTIMIZATION

    struct short_string_impl_t : impl_t {
        short_string_impl_t(const char *ptr, std::size_t len);
        short_string_impl_t(const string &s);
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        char buffer[sso_max_length + 1];
        std::uint8_t length;
    };

    struct long_string_impl_t : impl_t {
        long_string_impl_t(string s);
        long_string_impl_t(std::shared_ptr<string> const &p);
		value_impl_type impl_type() const noexcept override;
        value_type type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        std::shared_ptr<string> ptr;
    };

#elif defined UJSON_REF_COUNTED_STRING

    struct string_impl_t : impl_t {
        string_impl_t(string s);
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        string str;
    };

#endif

    struct array_impl_t : impl_t {
        array_impl_t(array a);
        array_impl_t(const std::shared_ptr<array> &p);
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        std::shared_ptr<array> ptr;
    };
    struct object_impl_t : impl_t {
        object_impl_t(object o);
        object_impl_t(const std::shared_ptr<object> &p);
        value_type type() const noexcept override;
		value_impl_type impl_type() const noexcept override;
        void clone(char *storage) const noexcept override;
        bool equals(const impl_t *ptr) const noexcept override;
        std::shared_ptr<object> ptr;
    };

    // cast m_storage to impl_t*
    const impl_t *impl() const noexcept;

    // destroy object in m_storage
    void destroy() noexcept;

    static bool is_valid_utf8(const char *start, const char *end) noexcept;

public:
#define UJSON_MAX(a, b) (((a) > (b)) ? (a) : (b))

#ifdef UJSON_SHORT_STRING_OPTIMIZATION
    static const std::size_t storage_size = UJSON_MAX(
        sizeof(null_impl_t),
        UJSON_MAX(
            sizeof(boolean_impl_t),
            UJSON_MAX(
                sizeof(number_impl_t),
                UJSON_MAX(
                    sizeof(array_impl_t),
                    UJSON_MAX(sizeof(object_impl_t),
                              UJSON_MAX(sizeof(short_string_impl_t),
                                        sizeof(long_string_impl_t)))))));
#elif defined UJSON_REF_COUNTED_STRING
    static const std::size_t storage_size = UJSON_MAX(
        sizeof(null_impl_t),
        UJSON_MAX(sizeof(boolean_impl_t),
                  UJSON_MAX(sizeof(number_impl_t),
                            UJSON_MAX(sizeof(array_impl_t),
                                      UJSON_MAX(sizeof(object_impl_t),
                                                sizeof(string_impl_t))))));
#endif
#undef UJSON_MAX
    char m_storage[storage_size];
};

void swap(value &lhs, value &rhs) noexcept;

inline bool operator<(name_value_pair const &lhs,
                      name_value_pair const &rhs) {
    return lhs.first < rhs.first;
}

extern const value null;

class string_view {
public:
    string_view(const char *ptr, std::size_t len);

    const char *c_str() const;
    std::size_t length() const;

    operator std::string() const;

    const char *begin() const;
    const char *cbegin() const;
    const char *end() const;
    const char *cend() const;

    friend bool operator==(string_view const &lhs, string_view const &rhs);

private:
    const char *m_ptr;
    std::size_t m_length;
};

enum class character_encoding { ascii, utf8 };

struct to_string_options {
    int indent_amount;           // 0 means no insigficant whitespace
    character_encoding encoding; // ascii with utf-8 escaped \uXXXX or utf-8
};

const to_string_options indented_utf8 = { 4, character_encoding::utf8 };
const to_string_options indented_ascii = { 4, character_encoding::ascii };
const to_string_options compact_utf8 = { 0, character_encoding::utf8 };
const to_string_options compact_ascii = { 0, character_encoding::ascii };

// convert value to string using specified string options
std::string to_string(value const &v,
                      const to_string_options &opts = indented_utf8);
std::ostream &operator<<(std::ostream &stream, value const &v);

enum class error_code {
	no_error,		 // no error
    bad_cast,        // value has wrong type for cast
    bad_number,      // number not finite (NaN/inf not supported by JSON)
    bad_string,      // invalid utf-8 string
    invalid_syntax,  // error parsing JSON
    integer_overflow // number is outside valid range for integer cast
};

struct parse_result {
	value value;
	error_code error;
	int line;

	std::string error_string() const;
};

// parse buffer into value; if len==0 buffer must be zero terminated
// throws if buffer is not valid JSON
parse_result parse(const char *buffer, std::size_t len = 0);

// find first value with given name; returns obj.end() if not found
object::const_iterator find(object const &obj, char const *name);
object::iterator find(object &obj, char const *name);

} // namespace ujson

// ---------------------------------------------------------------------------
// implementation

#include <cassert>
#include <cstring>

namespace ujson {

inline value::value() noexcept { new (m_storage) null_impl_t{}; }

inline value::value(bool b) noexcept { new (m_storage) boolean_impl_t{ b }; }

inline value::value(double d) noexcept {
	new (m_storage) number_impl_t{ d };
}

inline value::value(std::int32_t i) noexcept : value(double(i)) {}

inline value::value(std::uint32_t i) noexcept : value(double(i)) {}

inline value::value(string const &s) {

#ifdef UJSON_SHORT_STRING_OPTIMIZATION
	auto length = s.length();
	if (length <= sso_max_length)
		new (m_storage) short_string_impl_t{ s.c_str(), length };
	else
		new (m_storage) long_string_impl_t{ s };
#elif defined UJSON_REF_COUNTED_STRING
	new (m_storage) string_impl_t{ std::move(s) };
#endif
}

inline value::value(string &&s) {

#ifdef UJSON_SHORT_STRING_OPTIMIZATION
	auto length = s.length();
	if (length <= sso_max_length)
		new (m_storage)short_string_impl_t{ s.c_str(), length };
	else
		new (m_storage)long_string_impl_t{ std::move(s) };
#elif defined UJSON_REF_COUNTED_STRING
	new (m_storage)string_impl_t{ std::move(s) };
#endif
}
inline value::value(const char *ptr, std::size_t length) {

	if (!length)
		length = std::strlen(ptr);

#ifdef UJSON_SHORT_STRING_OPTIMIZATION
	if (length <= sso_max_length)
		new (m_storage) short_string_impl_t{ ptr, length };
	else
		new (m_storage) long_string_impl_t{ std::string(ptr, ptr + length) };
#elif defined UJSON_REF_COUNTED_STRING
	new (m_storage) string_impl_t{ std::string(ptr, ptr + length) };
#endif
}

inline value::value(array const &a) { new (m_storage) array_impl_t{ a }; }

inline value::value(array &&a) { new (m_storage)array_impl_t{ std::move(a) }; }

inline value::value(object const &o) {
	new (m_storage) object_impl_t{ o };
}

inline value::value(object &&o) {
	new (m_storage)object_impl_t{ std::move(o) };
}

inline value::value(value const &rhs) noexcept {
	rhs.impl()->clone(m_storage);
}

inline value::value(value &&rhs) noexcept {
	std::memcpy(m_storage, rhs.m_storage, storage_size);
	new (rhs.m_storage)null_impl_t{};
}

template <typename T>
inline value::value(const std::vector<T> &a,
	const typename std::enable_if<
	std::is_convertible<T, value>::value>::type *p) {
	array tmp(a.begin(), a.end());
	new (m_storage) array_impl_t{ std::move(tmp) };
}

template <typename T>
inline value::value(
	const std::vector<T> &a,
	const typename std::enable_if<std::is_same<
	value, decltype(to_json(std::declval<T>()))>::value>::type *
	p) {
	array tmp;
	tmp.reserve(a.size());
	for (auto const &v : a)
		tmp.push_back(to_json(v));
	new (m_storage) array_impl_t{ std::move(tmp) };
}

template <typename T>
inline value::value(const std::map<std::string, T> &o,
	const typename std::enable_if<
	std::is_convertible<T, value>::value>::type *p) {
	object tmp(o.begin(), o.end());
	new (m_storage) object_impl_t{ std::move(tmp) };
}

template <typename T>
inline value::value(
	const std::map<std::string, T> &o,
	const typename std::enable_if<std::is_same<
	value, decltype(to_json(std::declval<T>()))>::value>::type *
	p) {
	object tmp;
	for (auto it = o.begin(); it != o.end(); ++it)
		tmp.push_back({ it->first, to_json(it->second) });
	new (m_storage) object_impl_t{ std::move(tmp) };
}

inline value &value::operator=(bool b) noexcept {
	destroy();
	new (m_storage) boolean_impl_t{ b };
	return *this;
}

inline value &value::operator=(double d) noexcept {
	destroy();
	new (m_storage) number_impl_t{ d };
	return *this;
}

inline value &value::operator=(std::int32_t i) noexcept {
	*this = double(i);
	return *this;
}

inline value &value::operator=(std::uint32_t i) noexcept {
	*this = double(i);
	return *this;
}

inline value &value::operator=(string const &s) noexcept {

	if (!is_valid_utf8(s.data(), s.data() + s.length())) {
		destroy();
		new (m_storage) null_impl_t{};
		return *this;
	}

#ifdef UJSON_SHORT_STRING_OPTIMIZATION
	auto length = s.length();
	if (length <= sso_max_length) {
		destroy();
		new (m_storage) short_string_impl_t{ s.c_str(), length };
	}
	else {
		char storage[storage_size];
		new (storage) long_string_impl_t{ s }; // may throw
		destroy();
		std::memcpy(m_storage, storage, storage_size);
	}
#elif defined UJSON_REF_COUNTED_STRING
	destroy();
	new (m_storage) string_impl_t{ s };
#endif

	return *this;
}

inline value &value::operator=(string &&s) noexcept {

	if (!is_valid_utf8(s.data(), s.data() + s.length())) {
		destroy();
		new (m_storage) null_impl_t{};
		return *this;
	}

#ifdef UJSON_SHORT_STRING_OPTIMIZATION
	auto length = s.length();
	if (length <= sso_max_length) {
		destroy();
		new (m_storage)short_string_impl_t{ s.c_str(), length };
	}
	else {
		char storage[storage_size];
		new (storage)long_string_impl_t{ std::move(s) }; // may throw
		destroy();
		std::memcpy(m_storage, storage, storage_size);
	}
#elif defined UJSON_REF_COUNTED_STRING
	destroy();
	new (m_storage)string_impl_t{ std::move(s) };
#endif

	return *this;
}

inline value &value::operator=(const char *p) noexcept {
	*this = std::string(p);
	return *this;
}

inline value &value::operator=(array const &a){
	char storage[storage_size];
	new (storage) array_impl_t{ a }; // may throw
	destroy();
	std::memcpy(m_storage, storage, storage_size);
	return *this;
}

inline value &value::operator=(array &&a) {
	char storage[storage_size];
	new (storage)array_impl_t{ std::move(a) }; // may throw
	destroy();
	std::memcpy(m_storage, storage, storage_size);
	return *this;
}

inline value &value::operator=(object const &o) {
	char storage[storage_size];
	new (storage) object_impl_t{ o }; // may throw
	destroy();
	std::memcpy(m_storage, storage, storage_size);
	return *this;
}

inline value &value::operator=(object &&o) {
	char storage[storage_size];
	new (storage)object_impl_t{ std::move(o) }; // may throw
	destroy();
	std::memcpy(m_storage, storage, storage_size);
	return *this;
}

inline value &value::operator=(value const &rhs) noexcept {
	destroy();
	rhs.impl()->clone(m_storage);
	return *this;
}

inline value &value::operator=(value &&rhs) noexcept {
	destroy();
	std::memcpy(m_storage, rhs.m_storage, storage_size);
	new (rhs.m_storage)null_impl_t{};
	return *this;
}

inline value::~value() { destroy(); }

inline bool value::is_null() const noexcept {
	return type() == value_type::null;
}

inline bool value::is_boolean() const noexcept {
	return type() == value_type::boolean;
}
inline bool value::is_number() const noexcept {
	return type() == value_type::number;
}
inline bool value::is_string() const noexcept {
	return type() == value_type::string;
}
inline bool value::is_array() const noexcept {
	return type() == value_type::array;
}
inline bool value::is_object() const noexcept {
	return type() == value_type::object;
}

inline bool value::to_boolean(bool fallback) const noexcept {
	if (is_boolean()) {
		return static_cast<const boolean_impl_t *>(impl())->boolean;
	}
	return fallback;
}

inline int32_t value::to_int32(int32_t fallback) const noexcept {
	if (is_number()) {
		return static_cast<int32_t>(static_cast<const number_impl_t *>(impl())->number);
	}
	return fallback;
}

inline uint32_t value::to_uint32(uint32_t fallback) const noexcept
{
	if (is_number()) {
		return static_cast<uint32_t>(static_cast<const number_impl_t *>(impl())->number);
	}
	return fallback;
}

inline float value::to_float(float fallback) const noexcept
{
	if (is_number()) {
		return static_cast<float>(static_cast<const number_impl_t *>(impl())->number);
	}
	return fallback;
}

inline double value::to_double(double fallback) const noexcept {
	if (is_number()) {
		return static_cast<const number_impl_t *>(impl())->number;
	}
	return fallback;
}

inline string_view value::to_string(const char *fallback) const noexcept
{
	if (is_string()) {
#ifdef UJSON_SHORT_STRING_OPTIMIZATION
		auto short_impl = dynamic_cast<const short_string_impl_t *>(impl());
		if (short_impl)
			return string_view { short_impl->buffer, short_impl->length };
		auto long_impl = dynamic_cast<const long_string_impl_t *>(impl());
		if (long_impl)
			return  string_view { long_impl->ptr->c_str(), long_impl->ptr->length() };
#elif defined UJSON_REF_COUNTED_STRING
		auto impl = dynamic_cast<const string_impl_t *>(impl());
		if (impl)
			return impl->str.c_str();
#endif
	}
	return string_view{ fallback, strlen(fallback) };
}

inline const array *value::to_array() const noexcept {
	if (is_array()) {
		return static_cast<const array_impl_t*>(impl())->ptr.get();
	}
	return nullptr;
}

inline array *value::to_array() noexcept {
	if (is_array()) {
		return static_cast<const array_impl_t*>(impl())->ptr.get();
	}
	return nullptr;
}

inline const object *value::to_object() const noexcept {
	if (is_object()) {
		return static_cast<const object_impl_t*>(impl())->ptr.get();
	}
	return nullptr;
}

inline object *value::to_object() noexcept {
	if (is_object()) {
		return static_cast<const object_impl_t*>(impl())->ptr.get();
	}
	return nullptr;
}

inline value_type value::type() const noexcept { return impl()->type(); }

inline void value::swap(value &other) noexcept {
    char tmp[storage_size];
    std::memcpy(tmp, m_storage, storage_size);
    std::memcpy(m_storage, other.m_storage, storage_size);
    std::memcpy(other.m_storage, tmp, storage_size);
}

inline void value::destroy() noexcept { impl()->~impl_t(); }

inline const value::impl_t *value::impl() const noexcept {
    return reinterpret_cast<const impl_t *>(m_storage);
}

inline void swap(value &lhs, value &rhs) noexcept { lhs.swap(rhs); }

inline bool operator==(const value &lhs, const value &rhs) {

    if (lhs.impl()->impl_type() != rhs.impl()->impl_type())
        return false;

    return lhs.impl()->equals(rhs.impl());
}

inline bool operator!=(const value &lhs, const value &rhs) {
    return !(lhs == rhs);
}

// --------------------------------------------------------------------------

inline string_view::string_view(const char *ptr, std::size_t len)
    : m_ptr(ptr), m_length(len) {}

inline const char *string_view::c_str() const { return m_ptr; }

inline std::size_t string_view::length() const { return m_length; }

inline string_view::operator std::string() const {
    return std::string(m_ptr, m_ptr + m_length);
}

inline const char *string_view::begin() const { return m_ptr; }

inline const char *string_view::cbegin() const { return m_ptr; }

inline const char *string_view::end() const { return m_ptr + m_length; }

inline const char *string_view::cend() const { return m_ptr + m_length; }

inline bool operator==(string_view const &lhs, string_view const &rhs) {
    return lhs.m_ptr == rhs.m_ptr && lhs.m_length == rhs.m_length;
}

// --------------------------------------------------------------------------

inline object::const_iterator find(object const &obj, char const *name) {
	for (auto it = obj.begin(); it != obj.end(); ++it)
		if (it->first == name)
			return it;
	return obj.end();
}

inline object::iterator find(object &obj, char const *name) {
	for (auto it = obj.begin(); it != obj.end(); ++it)
		if (it->first == name)
			return it;
	return obj.end();
}

// --------------------------------------------------------------------------

inline value::impl_t::~impl_t() {}

// null
inline value::null_impl_t::null_impl_t() noexcept {}

inline value_type value::null_impl_t::type() const noexcept {
    return value_type::null;
}
inline value_impl_type value::null_impl_t::impl_type() const noexcept {
    return value_impl_type::null;
}

inline void value::null_impl_t::clone(char *storage) const noexcept {
    new (storage) null_impl_t{};
}

inline bool value::null_impl_t::equals(const impl_t *) const noexcept {
    return true;
}

// boolean

inline value::boolean_impl_t::boolean_impl_t(bool b) noexcept : boolean(b) {}

inline value_type value::boolean_impl_t::type() const noexcept {
    return value_type::boolean;
}
inline value_impl_type value::boolean_impl_t::impl_type() const noexcept {
    return value_impl_type::boolean;
}

inline void value::boolean_impl_t::clone(char *storage) const noexcept {
    new (storage) boolean_impl_t{ boolean };
}

inline bool value::boolean_impl_t::equals(const impl_t *base) const noexcept {
    const boolean_impl_t *ptr = static_cast<const boolean_impl_t *>(base);
    return ptr->boolean == boolean;
}

// number

inline value::number_impl_t::number_impl_t(double n) noexcept : number(n) {}

inline value_type value::number_impl_t::type() const noexcept {
    return value_type::number;
}
inline value_impl_type value::number_impl_t::impl_type() const noexcept {
    return value_impl_type::number;
}

inline void value::number_impl_t::clone(char *storage) const noexcept {
    new (storage) number_impl_t{ number };
}

inline bool value::number_impl_t::equals(const impl_t *base) const noexcept {
    const number_impl_t *ptr = static_cast<const number_impl_t *>(base);
    return ptr->number == number;
}

#ifdef UJSON_SHORT_STRING_OPTIMIZATION

// short string

inline value::short_string_impl_t::short_string_impl_t(const char *ptr,
                                                       std::size_t len) {
    assert(len <= sso_max_length);
    std::memcpy(buffer, ptr, len + 1);
    length = static_cast<std::uint8_t>(len);
}

inline value::short_string_impl_t::short_string_impl_t(const string &s)
    : short_string_impl_t(s.c_str(), s.length()) {}

inline value_type value::short_string_impl_t::type() const noexcept {
    return value_type::string;
}
inline value_impl_type value::short_string_impl_t::impl_type() const noexcept {
    return value_impl_type::short_string;
}

inline void value::short_string_impl_t::clone(char *storage) const noexcept {
    new (storage) short_string_impl_t{ buffer, length };
}

inline bool value::short_string_impl_t::equals(const impl_t *base) const
    noexcept {
    auto ptr = static_cast<const short_string_impl_t *>(base);
    if (length != ptr->length)
        return false;
    return std::memcmp(ptr->buffer, buffer, length) == 0;
}

// long string

inline value::long_string_impl_t::long_string_impl_t(string s) {
    ptr = std::make_shared<string>(std::move(s));
}

inline value::long_string_impl_t::long_string_impl_t(
    std::shared_ptr<string> const &p)
    : ptr(std::move(p)) {}

inline value_type value::long_string_impl_t::type() const noexcept {
    return value_type::string;
}
inline value_impl_type value::long_string_impl_t::impl_type() const noexcept {
    return value_impl_type::long_string;
}


inline void value::long_string_impl_t::clone(char *storage) const noexcept {
    new (storage) long_string_impl_t{ ptr };
}

inline bool value::long_string_impl_t::equals(const impl_t *base) const
    noexcept {
    auto derived = static_cast<const long_string_impl_t *>(base);
    return *derived->ptr == *ptr;
}

#elif defined UJSON_REF_COUNTED_STRING

inline value::string_impl_t::string_impl_t(string s) : str(std::move(s)) {}

inline value_type value::string_impl_t::type() const noexcept {
    return value_type::string;
}
inline value_impl_type value::string_impl_t::impl_type() const noexcept {
    return value_impl_type::string;
}

inline void value::string_impl_t::clone(char *storage) const noexcept {
    new (storage) string_impl_t{ str };
}

inline bool value::string_impl_t::equals(const impl_t *base) const noexcept {
    auto derived = static_cast<const string_impl_t *>(base);
    return derived->str == str;
}

#endif

// array

inline value::array_impl_t::array_impl_t(array a) {
    ptr = std::make_shared<array>(std::move(a));
}

inline value::array_impl_t::array_impl_t(const std::shared_ptr<array> &p)
    : ptr(p) {}

inline value_type value::array_impl_t::type() const noexcept {
    return value_type::array;
}
inline value_impl_type value::array_impl_t::impl_type() const noexcept {
    return value_impl_type::array;
}

inline void value::array_impl_t::clone(char *storage) const noexcept {
    new (storage) array_impl_t{ ptr };
}

inline bool value::array_impl_t::equals(const impl_t *base) const noexcept {
    const array_impl_t *derived = static_cast<const array_impl_t *>(base);
    return *derived->ptr == *ptr;
}

// object

inline value::object_impl_t::object_impl_t(object o) {
    ptr = std::make_shared<object>(std::move(o));
}

inline value::object_impl_t::object_impl_t(const std::shared_ptr<object> &p)
    : ptr(p) {}

inline value_type value::object_impl_t::type() const noexcept {
    return value_type::object;
}
inline value_impl_type value::object_impl_t::impl_type() const noexcept {
    return value_impl_type::object;
}

inline void value::object_impl_t::clone(char *storage) const noexcept {
    new (storage) object_impl_t{ ptr };
}

inline bool value::object_impl_t::equals(const impl_t *base) const noexcept {
    const object_impl_t *derived = static_cast<const object_impl_t *>(base);
    return *derived->ptr == *ptr;
}

} // namespace ujson
