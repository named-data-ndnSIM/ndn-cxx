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

#include "ndn-cxx/lp/util-header.hpp"
#include "ndn-cxx/lp/tlv.hpp"

namespace ndn {
namespace lp {

UtilHeader::UtilHeader() {
  m_utils = make_optional<UtilStruct>();
}

UtilHeader::UtilHeader(const Block& block)
{
  m_utils = make_optional<UtilStruct>();
  wireDecode(block);
}

UtilHeader::UtilHeader(UtilStruct utils)
  : m_utils(std::move(utils))
{
}

template<encoding::Tag TAG>
size_t
UtilHeader::wireEncode(EncodingImpl<TAG>& encoder) const
{
  if (m_utils == nullopt) {
    BOOST_THROW_EXCEPTION(Error("UtilHeader does not contain a utilization"));
  }

  // std::cout << "Wire encoding" << std::endl;

  size_t length = 0;
  for (auto vec_size = 0; vec_size < m_utils->utils.size(); vec_size++) {
    //std::cout << "m_utils->delays.size(): " << m_utils->delays.size() << std::endl;
    //std::cout << "m_utils->utils.size(): " << m_utils->utils.size() << std::endl;
    length += prependNonNegativeIntegerBlock(encoder, 80, m_utils->delays[vec_size]);
    length += prependNonNegativeIntegerBlock(encoder, 81, int(m_utils->utils[vec_size]));
  }
  length += encoder.prependVarNumber(length);
  length += encoder.prependVarNumber(tlv::UtilTag);
  return length;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(UtilHeader);

void
UtilHeader::wireDecode(const Block& wire)
{
  if (wire.type() != tlv::UtilTag) {
    BOOST_THROW_EXCEPTION(Error("Unexpected TLV-TYPE " + to_string(wire.type())));
  }
  //std::cout << "Wire decoding" << std::endl;
  wire.parse();
  for (auto element = wire.elements_begin(); element != wire.elements_end(); ++element) {
    //std::cout << "in wire decoding loop" << std::endl;
    //element->parse();
    //std::cout << "Element type: " << element->type() << std::endl;
    if (element->type() == 81) {
      // std::cout << "Found type 81" << std::endl;
      m_utils->utils.push_back(readNonNegativeInteger(*element));
    }
    else if (element->type() == 80) {
      // std::cout << "Found type 80" << std::endl;
      m_utils->delays.push_back(readNonNegativeInteger(*element));
    }
  }
}

} // namespace lp
} // namespace ndn
