/// Json-cpp amalgamated forward header (http://jsoncpp.sourceforge.net/).
/// It is intended to be used with +ACM-include +ACI-json/json-forwards.h+ACI-
/// This header provides forward declaration for all JsonCpp types.

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////

/+ACo-
The JsonCpp library's source code, including accompanying documentation, 
tests and demonstration applications, are licensed under the following
conditions...

Baptiste Lepilleur and The JsonCpp Authors explicitly disclaim copyright in all 
jurisdictions which recognize such a disclaimer. In such jurisdictions, 
this software is released into the Public Domain.

In jurisdictions which do not recognize Public Domain property (e.g. Germany as of
2010), this software is Copyright (c) 2007-2010 by Baptiste Lepilleur and
The JsonCpp Authors, and is released under the terms of the MIT License (see below).

In jurisdictions which recognize Public Domain property, the user of this 
software may choose to accept it either as 1) Public Domain, 2) under the 
conditions of the MIT License (see below), or 3) under the terms of dual 
Public Domain/MIT License conditions described here, as they choose.

The MIT License is about as close to Public Domain as a license can get, and is
described in clear, concise terms at:

   http://en.wikipedia.org/wiki/MIT+AF8-License
   
The full text of the MIT License follows:

+AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9-
Copyright (c) 2007-2010 Baptiste Lepilleur and The JsonCpp Authors

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the +ACI-Software+ACI-), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED +ACI-AS IS+ACI-, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
+AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9AD0APQA9-
(END LICENSE TEXT)

The MIT license is compatible with both the GPL and commercial
software, affording one all of the rights of Public Domain with the
minor nuisance of being required to keep the above copyright notice
and license text in the source code. Note also that by accepting the
Public Domain +ACI-license+ACI- you can re-license your copy using whatever
license you like.

+ACo-/

// //////////////////////////////////////////////////////////////////////
// End of content of file: LICENSE
// //////////////////////////////////////////////////////////////////////





+ACM-ifndef JSON+AF8-FORWARD+AF8-AMALGAMATED+AF8-H+AF8-INCLUDED
+ACM- define JSON+AF8-FORWARD+AF8-AMALGAMATED+AF8-H+AF8-INCLUDED
/// If defined, indicates that the source file is amalgamated
/// to prevent private header inclusion.
+ACM-define JSON+AF8-IS+AF8-AMALGAMATION

// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/config.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

+ACM-ifndef JSON+AF8-CONFIG+AF8-H+AF8-INCLUDED
+ACM-define JSON+AF8-CONFIG+AF8-H+AF8-INCLUDED
+ACM-include +ADw-stddef.h+AD4-
+ACM-include +ADw-stdint.h+AD4- //typedef int64+AF8-t, uint64+AF8-t
+ACM-include +ADw-string+AD4-   //typedef String

/// If defined, indicates that json library is embedded in CppTL library.
//+ACM- define JSON+AF8-IN+AF8-CPPTL 1

/// If defined, indicates that json may leverage CppTL library
//+ACM-  define JSON+AF8-USE+AF8-CPPTL 1
/// If defined, indicates that cpptl vector based map should be used instead of
/// std::map
/// as Value container.
//+ACM-  define JSON+AF8-USE+AF8-CPPTL+AF8-SMALLMAP 1

// If non-zero, the library uses exceptions to report bad input instead of C
// assertion macros. The default is to use exceptions.
+ACM-ifndef JSON+AF8-USE+AF8-EXCEPTION
+ACM-define JSON+AF8-USE+AF8-EXCEPTION 1
+ACM-endif

/// If defined, indicates that the source file is amalgamated
/// to prevent private header inclusion.
/// Remarks: it is automatically defined in the generated amalgamated header.
// +ACM-define JSON+AF8-IS+AF8-AMALGAMATION

+ACM-ifdef JSON+AF8-IN+AF8-CPPTL
+ACM-include +ADw-cpptl/config.h+AD4-
+ACM-ifndef JSON+AF8-USE+AF8-CPPTL
+ACM-define JSON+AF8-USE+AF8-CPPTL 1
+ACM-endif
+ACM-endif

+ACM-ifdef JSON+AF8-IN+AF8-CPPTL
+ACM-define JSON+AF8-API CPPTL+AF8-API
+ACM-elif defined(JSON+AF8-DLL+AF8-BUILD)
+ACM-if defined(+AF8-MSC+AF8-VER) +AHwAfA- defined(+AF8AXw-MINGW32+AF8AXw-)
+ACM-define JSON+AF8-API +AF8AXw-declspec(dllexport)
+ACM-define JSONCPP+AF8-DISABLE+AF8-DLL+AF8-INTERFACE+AF8-WARNING
+ACM-endif // if defined(+AF8-MSC+AF8-VER)
+ACM-elif defined(JSON+AF8-DLL)
+ACM-if defined(+AF8-MSC+AF8-VER) +AHwAfA- defined(+AF8AXw-MINGW32+AF8AXw-)
+ACM-define JSON+AF8-API +AF8AXw-declspec(dllimport)
+ACM-define JSONCPP+AF8-DISABLE+AF8-DLL+AF8-INTERFACE+AF8-WARNING
+ACM-endif // if defined(+AF8-MSC+AF8-VER)
+ACM-endif // ifdef JSON+AF8-IN+AF8-CPPTL
+ACM-if +ACE-defined(JSON+AF8-API)
+ACM-define JSON+AF8-API
+ACM-endif

// If JSON+AF8-NO+AF8-INT64 is defined, then Json only support C+-+- +ACI-int+ACI- type for
// integer
// Storages, and 64 bits integer support is disabled.
// +ACM-define JSON+AF8-NO+AF8-INT64 1

+ACM-if defined(+AF8-MSC+AF8-VER) // MSVC
+ACM-if +AF8-MSC+AF8-VER +ADwAPQ- 1200  // MSVC 6
// Microsoft Visual Studio 6 only support conversion from +AF8AXw-int64 to double
// (no conversion from unsigned +AF8AXw-int64).
+ACM-define JSON+AF8-USE+AF8-INT64+AF8-DOUBLE+AF8-CONVERSION 1
// Disable warning 4786 for VS6 caused by STL (identifier was truncated to '255'
// characters in the debug information)
// All projects I've ever seen with VS6 were using this globally (not bothering
// with pragma push/pop).
+ACM-pragma warning(disable : 4786)
+ACM-endif // MSVC 6

+ACM-if +AF8-MSC+AF8-VER +AD4APQ- 1500 // MSVC 2008
                     /// Indicates that the following function is deprecated.
+ACM-define JSONCPP+AF8-DEPRECATED(message) +AF8AXw-declspec(deprecated(message))
+ACM-endif

+ACM-endif // defined(+AF8-MSC+AF8-VER)

// In c+-+-11 the override keyword allows you to explicitly define that a function
// is intended to override the base-class version.  This makes the code more
// manageable and fixes a set of common hard-to-find bugs.
+ACM-if +AF8AXw-cplusplus +AD4APQ- 201103L
+ACM-define JSONCPP+AF8-OVERRIDE override
+ACM-define JSONCPP+AF8-NOEXCEPT noexcept
+ACM-define JSONCPP+AF8-OP+AF8-EXPLICIT explicit
+ACM-elif defined(+AF8-MSC+AF8-VER) +ACYAJg- +AF8-MSC+AF8-VER +AD4- 1600 +ACYAJg- +AF8-MSC+AF8-VER +ADw- 1900
+ACM-define JSONCPP+AF8-OVERRIDE override
+ACM-define JSONCPP+AF8-NOEXCEPT throw()
+ACM-if +AF8-MSC+AF8-VER +AD4APQ- 1800 // MSVC 2013
+ACM-define JSONCPP+AF8-OP+AF8-EXPLICIT explicit
+ACM-else
+ACM-define JSONCPP+AF8-OP+AF8-EXPLICIT
+ACM-endif
+ACM-elif defined(+AF8-MSC+AF8-VER) +ACYAJg- +AF8-MSC+AF8-VER +AD4APQ- 1900
+ACM-define JSONCPP+AF8-OVERRIDE override
+ACM-define JSONCPP+AF8-NOEXCEPT noexcept
+ACM-define JSONCPP+AF8-OP+AF8-EXPLICIT explicit
+ACM-else
+ACM-define JSONCPP+AF8-OVERRIDE
+ACM-define JSONCPP+AF8-NOEXCEPT throw()
+ACM-define JSONCPP+AF8-OP+AF8-EXPLICIT
+ACM-endif

+ACM-ifndef JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES

+ACM-if defined(+AF8-MSC+AF8-VER) +ACYAJg- +AF8-MSC+AF8-VER +AD4APQ- 1600 // MSVC +AD4APQ- 2010
+ACM-define JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES 1
+ACM-endif // MSVC +AD4APQ- 2010

+ACM-ifdef +AF8AXw-clang+AF8AXw-
+ACM-if +AF8AXw-has+AF8-feature(cxx+AF8-rvalue+AF8-references)
+ACM-define JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES 1
+ACM-endif // has+AF8-feature

+ACM-elif defined +AF8AXw-GNUC+AF8AXw- // not clang (gcc comes later since clang emulates gcc)
+ACM-if defined(+AF8AXw-GXX+AF8-EXPERIMENTAL+AF8-CXX0X+AF8AXw-) +AHwAfA- (+AF8AXw-cplusplus +AD4APQ- 201103L)
+ACM-define JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES 1
+ACM-endif // GXX+AF8-EXPERIMENTAL

+ACM-endif // +AF8AXw-clang+AF8AXw- +AHwAfA- +AF8AXw-GNUC+AF8AXw-

+ACM-endif // not defined JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES

+ACM-ifndef JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES
+ACM-define JSON+AF8-HAS+AF8-RVALUE+AF8-REFERENCES 0
+ACM-endif

+ACM-ifdef +AF8AXw-clang+AF8AXw-
+ACM-if +AF8AXw-has+AF8-extension(attribute+AF8-deprecated+AF8-with+AF8-message)
+ACM-define JSONCPP+AF8-DEPRECATED(message) +AF8AXw-attribute+AF8AXw-((deprecated(message)))
+ACM-endif
+ACM-elif defined +AF8AXw-GNUC+AF8AXw- // not clang (gcc comes later since clang emulates gcc)
+ACM-if (+AF8AXw-GNUC+AF8AXw- +AD4- 4 +AHwAfA- (+AF8AXw-GNUC+AF8AXw- +AD0APQ- 4 +ACYAJg- +AF8AXw-GNUC+AF8-MINOR+AF8AXw- +AD4APQ- 5))
+ACM-define JSONCPP+AF8-DEPRECATED(message) +AF8AXw-attribute+AF8AXw-((deprecated(message)))
+ACM-elif (+AF8AXw-GNUC+AF8AXw- +AD4- 3 +AHwAfA- (+AF8AXw-GNUC+AF8AXw- +AD0APQ- 3 +ACYAJg- +AF8AXw-GNUC+AF8-MINOR+AF8AXw- +AD4APQ- 1))
+ACM-define JSONCPP+AF8-DEPRECATED(message) +AF8AXw-attribute+AF8AXw-((+AF8AXw-deprecated+AF8AXw-))
+ACM-endif // GNUC version
+ACM-endif // +AF8AXw-clang+AF8AXw- +AHwAfA- +AF8AXw-GNUC+AF8AXw-

+ACM-if +ACE-defined(JSONCPP+AF8-DEPRECATED)
+ACM-define JSONCPP+AF8-DEPRECATED(message)
+ACM-endif // if +ACE-defined(JSONCPP+AF8-DEPRECATED)

+ACM-if +AF8AXw-GNUC+AF8AXw- +AD4APQ- 6
+ACM-define JSON+AF8-USE+AF8-INT64+AF8-DOUBLE+AF8-CONVERSION 1
+ACM-endif

+ACM-if +ACE-defined(JSON+AF8-IS+AF8-AMALGAMATION)

+ACM-include +ACI-version.h+ACI-

+ACM-if JSONCPP+AF8-USING+AF8-SECURE+AF8-MEMORY
+ACM-include +ACI-allocator.h+ACI- //typedef Allocator
+ACM-endif

+ACM-endif // if +ACE-defined(JSON+AF8-IS+AF8-AMALGAMATION)

namespace Json +AHs-
typedef int Int+ADs-
typedef unsigned int UInt+ADs-
+ACM-if defined(JSON+AF8-NO+AF8-INT64)
typedef int LargestInt+ADs-
typedef unsigned int LargestUInt+ADs-
+ACM-undef JSON+AF8-HAS+AF8-INT64
+ACM-else                 // if defined(JSON+AF8-NO+AF8-INT64)
// For Microsoft Visual use specific types as long long is not supported
+ACM-if defined(+AF8-MSC+AF8-VER) // Microsoft Visual Studio
typedef +AF8AXw-int64 Int64+ADs-
typedef unsigned +AF8AXw-int64 UInt64+ADs-
+ACM-else                 // if defined(+AF8-MSC+AF8-VER) // Other platforms, use long long
typedef int64+AF8-t Int64+ADs-
typedef uint64+AF8-t UInt64+ADs-
+ACM-endif                // if defined(+AF8-MSC+AF8-VER)
typedef Int64 LargestInt+ADs-
typedef UInt64 LargestUInt+ADs-
+ACM-define JSON+AF8-HAS+AF8-INT64
+ACM-endif // if defined(JSON+AF8-NO+AF8-INT64)
+ACM-if JSONCPP+AF8-USING+AF8-SECURE+AF8-MEMORY
+ACM-define JSONCPP+AF8-STRING                                                         +AFw-
  std::basic+AF8-string+ADw-char, std::char+AF8-traits+ADw-char+AD4-, Json::SecureAllocator+ADw-char+AD4- +AD4-
+ACM-define JSONCPP+AF8-OSTRINGSTREAM                                                  +AFw-
  std::basic+AF8-ostringstream+ADw-char, std::char+AF8-traits+ADw-char+AD4-,                       +AFw-
                           Json::SecureAllocator+ADw-char+AD4- +AD4-
+ACM-define JSONCPP+AF8-OSTREAM std::basic+AF8-ostream+ADw-char, std::char+AF8-traits+ADw-char+AD4- +AD4-
+ACM-define JSONCPP+AF8-ISTRINGSTREAM                                                  +AFw-
  std::basic+AF8-istringstream+ADw-char, std::char+AF8-traits+ADw-char+AD4-,                       +AFw-
                           Json::SecureAllocator+ADw-char+AD4- +AD4-
+ACM-define JSONCPP+AF8-ISTREAM std::istream
+ACM-else
+ACM-define JSONCPP+AF8-STRING std::string
+ACM-define JSONCPP+AF8-OSTRINGSTREAM std::ostringstream
+ACM-define JSONCPP+AF8-OSTREAM std::ostream
+ACM-define JSONCPP+AF8-ISTRINGSTREAM std::istringstream
+ACM-define JSONCPP+AF8-ISTREAM std::istream
+ACM-endif // if JSONCPP+AF8-USING+AF8-SECURE+AF8-MEMORY
+AH0- // end namespace Json

+ACM-endif // JSON+AF8-CONFIG+AF8-H+AF8-INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/config.h
// //////////////////////////////////////////////////////////////////////






// //////////////////////////////////////////////////////////////////////
// Beginning of content of file: include/json/forwards.h
// //////////////////////////////////////////////////////////////////////

// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

+ACM-ifndef JSON+AF8-FORWARDS+AF8-H+AF8-INCLUDED
+ACM-define JSON+AF8-FORWARDS+AF8-H+AF8-INCLUDED

+ACM-if +ACE-defined(JSON+AF8-IS+AF8-AMALGAMATION)
+ACM-include +ACI-config.h+ACI-
+ACM-endif // if +ACE-defined(JSON+AF8-IS+AF8-AMALGAMATION)

namespace Json +AHs-

// writer.h
class FastWriter+ADs-
class StyledWriter+ADs-

// reader.h
class Reader+ADs-

// features.h
class Features+ADs-

// value.h
typedef unsigned int ArrayIndex+ADs-
class StaticString+ADs-
class Path+ADs-
class PathArgument+ADs-
class Value+ADs-
class ValueIteratorBase+ADs-
class ValueIterator+ADs-
class ValueConstIterator+ADs-

+AH0- // namespace Json

+ACM-endif // JSON+AF8-FORWARDS+AF8-H+AF8-INCLUDED

// //////////////////////////////////////////////////////////////////////
// End of content of file: include/json/forwards.h
// //////////////////////////////////////////////////////////////////////





+ACM-endif //ifndef JSON+AF8-FORWARD+AF8-AMALGAMATED+AF8-H+AF8-INCLUDED
