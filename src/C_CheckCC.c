#include "dauphin.h"

SEXP C_CCRequired(SEXP x, SEXP ignore_calling_code) {
  if (!isString(x)) {
    error("`x` was type '%s', but must be a character vector.", type2char(TYPEOF(x)));
  }
  if (!isLogical(ignore_calling_code) || xlength(ignore_calling_code) != 1) {
    error("`ignore_calling_code` was a '%s' of length %llu. Change `ignore_calling_code` to TRUE, FALSE, or NA.",
          type2char(TYPEOF(ignore_calling_code)), xlength(ignore_calling_code));
  }
  int icc = asLogical(ignore_calling_code);
  if (icc != NA_INTEGER) {
    return ScalarLogical(icc);
  }
  R_xlen_t N = xlength(x);
  for (R_xlen_t i = 0; i < N; ++i) {
    int n = length(STRING_ELT(x, i));
    if (n <= 2) {
      continue;
    }
    const char * xi = CHAR(STRING_ELT(x, i));
    if (xi[0] == '0' && xi[1] == '4') {
      continue;
    }
    if (xi[0] == '+') {
      return ScalarLogical(1);
    }
    if (xi[0] == '6' && xi[1] == '1') {
      return ScalarLogical(1);
    }
  }
  return ScalarLogical(0);
}
