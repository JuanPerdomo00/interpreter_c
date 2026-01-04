#include "utils/source_map.h"
#include "utils/binary_search.h"
#include <stdint.h>

SourceMap source_map_create() {
  return (SourceMap){.map = vector_create(sizeof(uint32_t), DEFAULT_CAPACITY)};
}

uint32_t source_map_line_of(SourceMap *src_map, uint32_t offset) {
  return binary_search_upper_bound(&src_map->map, offset);
}

uint32_t source_map_column_of(uint32_t offset, uint32_t line) {
  return offset - line;
}
