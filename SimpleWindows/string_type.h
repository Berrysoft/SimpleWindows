#pragma once
#include "utility.h"

#include <string>

namespace sw
{
    using char_t = TCHAR;
    using string_t = std::basic_string<char_t>;
    using stringbuf_t = std::basic_stringbuf<char_t>;
    using stringstream_t = std::basic_stringstream<char_t>;
    using istringstream_t = std::basic_istringstream<char_t>;
    using ostringstream_t = std::basic_ostringstream<char_t>;
} // namespace sw

#ifdef SW_CXX17
#include <string_view>

namespace sw
{
    using string_view_t = std::basic_string_view<char_t>;
}
#endif // SW_CXX17
