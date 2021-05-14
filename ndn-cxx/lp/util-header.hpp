/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2018 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_CXX_LP_UTIL_HEADER_HPP
#define NDN_CXX_LP_UTIL_HEADER_HPP

#include "ndn-cxx/lp/tlv.hpp"
#include "ndn-cxx/lp/tags.hpp"

#include <iostream>
#include <vector>

namespace ndn {
namespace lp {

struct UtilStruct {
  std::vector<double> utils;
  std::vector<uint32_t> delays;
};

/** \brief represents a Util header field in NDNLP
 */
class UtilHeader
{
public:
  class Error : public ndn::tlv::Error
  {
  public:
    using ndn::tlv::Error::Error;
  };
  UtilHeader();

  explicit
  UtilHeader(const Block& block);

  /** \brief constructs PrefixAnnouncementHeader using PrefixAnnouncement
   *
   *  \throw Error PrefixAnnouncement does not contain Data.
   */
  explicit
  UtilHeader(UtilStruct utils);

  /** \brief encodes the prefix announcement header to the wire format
   *
   *  \throw Error this instance does not contain a PrefixAnnouncement.
   */
  template<encoding::Tag TAG>
  size_t
  wireEncode(EncodingImpl<TAG>& encoder) const;

  void
  wireDecode(const Block& wire);

  const optional<UtilStruct>&
  getUtilStruct() const
  {
    return m_utils;
  }

  optional<UtilStruct> m_utils;
};

NDN_CXX_DECLARE_WIRE_ENCODE_INSTANTIATIONS(UtilHeader);

} // namespace lp
} // namespace ndn

#endif // NDN_CXX_LP_PREFIX_ANNOUNCEMENT_HEADER_HPP
