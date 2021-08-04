#include "dauphin.h"

SEXP Cgrepl_digit(SEXP xx) {
  if (!isString(xx)) {
    error("xx must be a character vector.");
  }
  R_xlen_t N = xlength(xx);
  SEXP ans = PROTECT(allocVector(LGLSXP, N));
  int * restrict ansp = LOGICAL(ans);
  for (R_xlen_t i = 0; i < N; ++i) {
    SEXP CX = STRING_ELT(xx, i);
    int n = length(CX);
    const char * x = CHAR(CX);
    bool o = false;
    for (int j = 0; j < n; ++j) {
      if (isdigit(x[j])) {
        o = true;
        break;
      }
    }
    ansp[i] = o;
  }
  UNPROTECT(1);
  return ans;
}

SEXP C_all_digits(SEXP xx) {
  if (!isString(xx)) {
    return ScalarLogical(isNumeric(xx));
  }
  R_xlen_t N = xlength(xx);
  const SEXP * xp = STRING_PTR(xx);
  SEXP ans = PROTECT(allocVector(LGLSXP, N));
  int * restrict ansp = LOGICAL(ans);
  for (R_xlen_t i = 0; i < N; ++i) {
    int n = length(xp[i]);
    ansp[i] = 1;
    if (n < 1) {
      continue;
    }
    const char * x = CHAR(xp[i]);
    bool o = true;
    for (int j = 0; j < n; ++j) {
      if (!isdigit(x[j])) {
        o = false;
        break;
      }
    }
    ansp[i] = o;
  }
  UNPROTECT(1);
  return ans;
}

