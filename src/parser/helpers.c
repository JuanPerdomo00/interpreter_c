#include "parser/helpers.h"
#include "utils/str_view.h"
#include <stddef.h>

double sv_to_double(StrView sv) {
    double result = 0.0;
    double fraction = 0.0;
    double divisor = 1.0;

    int sign = 1;
    int exp_sign = 1;
    int exponent = 0;

    size_t i = 0;

    if (i < sv.len) {
        if (sv.ptr[i] == '-') { sign = -1; i++; }
        else if (sv.ptr[i] == '+') { i++; }
    }

    while (i < sv.len && sv.ptr[i] >= '0' && sv.ptr[i] <= '9') {
        result = result * 10.0 + (sv.ptr[i] - '0');
        i++;
    }

    if (i < sv.len && sv.ptr[i] == '.') {
        i++;
        while (i < sv.len && sv.ptr[i] >= '0' && sv.ptr[i] <= '9') {
            fraction = fraction * 10.0f + (sv.ptr[i] - '0');
            divisor *= 10.0f;
            i++;
        }
        result += fraction / divisor;
    }

    if (i < sv.len && (sv.ptr[i] == 'e' || sv.ptr[i] == 'E')) {
        i++;

        if (i < sv.len) {
            if (sv.ptr[i] == '-') { exp_sign = -1; i++; }
            else if (sv.ptr[i] == '+') { i++; }
        }

        while (i < sv.len && sv.ptr[i] >= '0' && sv.ptr[i] <= '9') {
            exponent = exponent * 10 + (sv.ptr[i] - '0');
            i++;
        }

        exponent *= exp_sign;

        double power = 1.0f;
        while (exponent > 0) { power *= 10.0f; exponent--; }
        while (exponent < 0) { power /= 10.0f; exponent++; }

        result *= power;
    }

    return sign * result;
}

long long sv_to_int(StrView sv) {
  long long result = 0;
  for(size_t i = 0; i < sv.len; ++i) {
    result = result * 10 + (sv.ptr[i] - '0');
  }
  return result;
}




