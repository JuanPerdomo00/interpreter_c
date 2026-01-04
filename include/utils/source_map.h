#ifndef SOURCE_MAP_H
#define SOURCE_MAP_H

#include <stdint.h>
#define DEFAULT_CAPACITY 100000
#include "utils/vector.h"
typedef struct SourceMap SourceMap;

struct SourceMap {
  // ints or  
  Vector map;
};


inline SourceMap source_map_create();

inline uint32_t source_map_line_of(SourceMap* src_map, uint32_t offset);

inline uint32_t source_map_column_of(uint32_t offset, uint32_t line) ;

#endif
