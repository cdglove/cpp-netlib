#ifndef SERVER_REQUEST_PARSERS_IMPL_UW3PM6V6
#define SERVER_REQUEST_PARSERS_IMPL_UW3PM6V6

#define BOOST_SPIRIT_UNICODE
#include <boost/spirit/include/qi.hpp>

// Copyright 2013 Google, Inc.
// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tuple>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/network/protocol/http/message/header.hpp>

#ifdef BOOST_NETWORK_NO_LIB
#ifndef BOOST_NETWORK_INLINE
#define BOOST_NETWORK_INLINE inline
#endif
#else
#define BOOST_NETWORK_INLINE
#endif
#include <vector>

namespace boost {
namespace spirit {
namespace traits {

typedef std::basic_string<uint32_t> u32_string;

template <>  // <typename Attrib, typename T, typename Enable>
struct assign_to_container_from_value<std::string, u32_string, void> {
  static void call(u32_string const& val, std::string& attr) {
    u32_to_u8_iterator<u32_string::const_iterator> begin = val.begin(),
                                                   end = val.end();
    for (; begin != end; ++begin) attr += *begin;
  }
};

}  // namespace traits
}  // namespace spirit
}  // namespace boost

namespace boost {
namespace network {
namespace http {

BOOST_NETWORK_INLINE void parse_version(
    std::string const& partial_parsed,
    std::tuple<std::uint8_t, std::uint8_t>& version_pair) {
  using boost::spirit::qi::lit;
  using boost::spirit::qi::ushort_;

  boost::spirit::qi::parse(partial_parsed.begin(), partial_parsed.end(),
        (lit("HTTP/") >> ushort_ >> '.' >> ushort_), version_pair);
}

BOOST_NETWORK_INLINE void parse_headers(
    std::string const& input, std::vector<request_header_narrow>& container) {
  u8_to_u32_iterator<std::string::const_iterator> begin = input.begin(),
                                                  end = input.end();
  using boost::spirit::qi::lit;
  using boost::spirit::qi::alnum;
  using boost::spirit::qi::punct;
  using boost::spirit::qi::space;
  //using boost::spirit::qi::lit;
  typedef boost::spirit::qi::as<boost::spirit::traits::u32_string> as_u32_string;
  boost::spirit::qi::parse(begin, end,
        *(+((alnum | punct) - ':') >> lit(": ") >>
          as_u32_string()[+((boost::spirit::qi::unicode::alnum | space | punct) - '\r' - '\n')] >>
          lit("\r\n")) >>
          lit("\r\n"),
        container);
}

}  // namespace http
}  // namespace network
}  // namespace boost

#endif /* SERVER_REQUEST_PARSERS_IMPL_UW3PM6V6 */
