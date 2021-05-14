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

#include "ndn-cxx/lp/grid-header.hpp"
#include "ndn-cxx/lp/tlv.hpp"

namespace ndn {
namespace lp {

GridHeader::GridHeader() {
  m_grids = make_optional<std::vector<Grid>>();
}

GridHeader::GridHeader(const Block& block)
{
  m_grids = make_optional<std::vector<Grid>>();
  wireDecode(block);
}

GridHeader::GridHeader(std::vector<Grid> grids)
  : m_grids(std::move(grids))
{
}

template<encoding::Tag TAG>
size_t
GridHeader::wireEncode(EncodingImpl<TAG>& encoder) const
{
  if (m_grids == nullopt) {
    BOOST_THROW_EXCEPTION(Error("GridHeader does not contain any info"));
  }

  // std::cout << "Wire encoding" << std::endl;

  size_t length = 0;
  std::cout << "vector size: " << m_grids->size() << std::endl;
  for (auto it = m_grids->begin(); it != m_grids->end(); it++) {
    std::cout << "it->x_start: " << it->x_start << std::endl;
    std::cout << "it->x_end: " << it->x_end << std::endl;
    std::cout << "it->y_start: " << it->y_start << std::endl;
    std::cout << "it->y_end: " << it->y_end << std::endl;
    length += prependNonNegativeIntegerBlock(encoder, 90, it->x_start);
    length += prependNonNegativeIntegerBlock(encoder, 91, it->x_end);
    length += prependNonNegativeIntegerBlock(encoder, 92, it->y_start);
    length += prependNonNegativeIntegerBlock(encoder, 93, it->y_end);
  }
  length += encoder.prependVarNumber(length);
  length += encoder.prependVarNumber(tlv::GridTag);
  return length;
}

NDN_CXX_DEFINE_WIRE_ENCODE_INSTANTIATIONS(GridHeader);

void
GridHeader::wireDecode(const Block& wire)
{
  if (wire.type() != tlv::GridTag) {
    BOOST_THROW_EXCEPTION(Error("Unexpected TLV-TYPE " + to_string(wire.type())));
  }

  std::cout << "Wire decoding" << std::endl;
  wire.parse();
  for (auto element = wire.elements_begin(); element != wire.elements_end(); ++element) {
    Grid grid;
    std::cout << "in wire decoding loop" << std::endl;
    //element->parse();
    std::cout << "Element type: " << element->type() << std::endl;
    if (element->type() == 93) {
      std::cout << "Found type 39" << std::endl;
      grid.y_end = encoding::readNonNegativeInteger(*element);
    }
    ++element;
    //element->parse();
    if (element->type() == 92) {
      std::cout << "Found type 92" << std::endl;
      grid.y_start = encoding::readNonNegativeInteger(*element);
    }
    ++element;
    //element->parse();
    if (element->type() == 91) {
      std::cout << "Found type 91" << std::endl;
      grid.x_end = encoding::readNonNegativeInteger(*element);
    }
    ++element;
    //element->parse();
    if (element->type() == 90) {
      std::cout << "Found type 90" << std::endl;
      grid.x_start = encoding::readNonNegativeInteger(*element);
    }
    m_grids->push_back(grid);
  }
}

} // namespace lp
} // namespace ndn
