#ifndef SIMDJSON_WESTMERE_STAGE1_FIND_MARKS_H
#define SIMDJSON_WESTMERE_STAGE1_FIND_MARKS_H

#include "simdjson/portability.h"

#ifdef IS_X86_64

#include "westmere/bitmask.h"
#include "westmere/simd.h"
#include "simdjson/stage1_find_marks.h"

TARGET_WESTMERE
namespace simdjson::westmere {

using namespace simd;

really_inline void find_whitespace_and_operators(
  const simd8x64<uint8_t> in,
  uint64_t &whitespace, uint64_t &op) {

  whitespace = in.map([&](simd8<uint8_t> _in) {
    return _in == _in.lookup_lower_4_bits<uint8_t>(' ', 100, 100, 100, 17, 100, 113, 2, 100, '\t', '\n', 112, 100, '\r', 100, 100);
  }).to_bitmask();

  op = in.map([&](simd8<uint8_t> _in) {
    return (_in | 32) == (_in+0xd4u).lookup_lower_4_bits<uint8_t>(',', '}', 0, 0, 0xc0u, 0, 0, 0, 0, 0, 0, 0, 0, 0, ':', '{');
  }).to_bitmask();
}

#include "generic/simdutf8check.h"
#include "generic/stage1_find_marks.h"

} // namespace westmere
UNTARGET_REGION

TARGET_WESTMERE
namespace simdjson {

template <>
int find_structural_bits<Architecture::WESTMERE>(const uint8_t *buf, size_t len, simdjson::ParsedJson &pj, bool streaming) {
  return westmere::stage1::find_structural_bits<64>(buf, len, pj, streaming);
}

} // namespace simdjson
UNTARGET_REGION

#endif // IS_X86_64
#endif // SIMDJSON_WESTMERE_STAGE1_FIND_MARKS_H
