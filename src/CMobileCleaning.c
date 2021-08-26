#include "dauphin.h"


bool intIsMobRange(unsigned int ux) {
  ux -= 400000000;
  return ux < 100000000;
}

bool intIsAusRange(unsigned int ux, bool mob_ok) {
  // true && purely for alignment
  return
  true &&
    ux >= 100000000 &&
    ux < 1000000000 &&
    (mob_ok ||
    ux <  400000000 ||
    ux >= 500000000);
}



SEXP Cgsub_09(SEXP xx) {
  if (!isString(xx)) {
    error("xx was type '%s' but must be a character vector.", type2char(TYPEOF(xx))); // # nocov
  }
  R_xlen_t N = xlength(xx);
  SEXP ans = PROTECT(allocVector(INTSXP, N));
  int * restrict ansp = INTEGER(ans);
  for (R_xlen_t i = 0; i < N; ++i) {
    SEXP CX = STRING_ELT(xx, i);
    if (CX == NA_STRING) {
      // SET_STRING_ELT(ans, i, CX);
      ansp[i] = NA_INTEGER;
      continue;
    }
    int n = length(CX);
    const char * x = CHAR(CX);
    uint64_t o = 0;
    uint64_t ten = 1;
    for (int j = n - 1; j >= 0; --j) {
      char xj = x[j];
      if (isdigit(xj)) {
        int oj = char2number(xj);
        o += ten * oj;
        ten *= 10;
      } else {
        if (xj != ' ' && xj != '-' && xj != ',') {
          // we continue the number if it's a comma or dash or space etc
          o = 0;
          ten = 1;
        }
      }
    }
    ansp[i] = (unsigned int)o;
  }
  UNPROTECT(1);
  return ans;
}

// bool jchar_is_number(const char * x, int j) {
//   return isdigit(x[j]);
// }
//
// bool jchars_are_numbers(int jj0, int jj1, int jj2, const char * x, int n, int j) {
//   int j0 = jj0 + j;
//   int j1 = jj1 + j;
//   int j2 = jj2 + j;
//   return j2 < n && jchar_is_number(x, j0) && jchar_is_number(x, j1) && jchar_is_number(x, j2);
// }
//
//
//
// int is_04mobile_from(const char * x, int n, char char1) {
//
//   if (n == 10) {
//     if (x[0] == '0' &&
//         x[1] == '4' &&
//         jchar_is_number(x, 2) &&
//         jchar_is_number(x, 3) &&
//
//         jchar_is_number(x, 4) &&
//         jchar_is_number(x, 5) &&
//         jchar_is_number(x, 6) &&
//
//         jchar_is_number(x, 7) &&
//         jchar_is_number(x, 8) &&
//         jchar_is_number(x, 9)) {
//       return 9;
//     }
//     return 0;
//   }
//   if (n == 9) {
//     if (x[0] == '4' &&
//         jchar_is_number(x, 1) &&
//         jchar_is_number(x, 2) &&
//
//         jchar_is_number(x, 3) &&
//         jchar_is_number(x, 4) &&
//         jchar_is_number(x, 5) &&
//
//         jchar_is_number(x, 6) &&
//         jchar_is_number(x, 7) &&
//         jchar_is_number(x, 8)) {
//       return 8;
//     }
//     return 0;
//   }
//   if (n == 15) {
//     if (x[0] == '+' && x[1] == '6' && x[2] == '1' &&
//         x[3] == ' ' && x[4] == '4' &&
//         jchar_is_number(x, 5) &&
//         jchar_is_number(x, 6) &&
//
//         jchar_is_number(x, 8) &&
//         jchar_is_number(x, 9) &&
//         jchar_is_number(x, 10) &&
//
//         jchar_is_number(x, 12) &&
//         jchar_is_number(x, 13) &&
//         jchar_is_number(x, 14)) {
//       return 14;
//     }
//     return 0;
//   }
//
//
//   int j = 0;
//   // start from the digit just before the first '4' in the number
//   // +614 we advance until we hit a plus
//   //  614 we advance until we hit a 6
//   while (char1 == '+' && j < n && x[j] == '+') {
//     ++j;
//   }
//   while (char1 != '0' && j < n && x[j] == '6') {
//     ++j;
//   }
//   while (x[j] == ' ') {
//     ++j;
//   }
//   for (; j < n - 9; ++j) {
//     if (char1 == '0') {
//       if (x[j] != '0' || x[j + 1] != '4') {
//         ++j;
//         continue;
//       }
//     } else {
//       if (x[j] != '1' || x[j + 1] != '4') {
//         ++j;
//         continue;
//       }
//     }
//     if (!jchars_are_numbers(2, 3, 3, x, n, j)) {
//       continue;
//     }
//     if (jchars_are_numbers(4, 5, 6, x, n, j)) {
//       if (jchars_are_numbers(7, 8, 9, x, n, j)) {
//         return j + 9;
//       }
//       if (x[j + 7] == ' ' &&
//           jchars_are_numbers(8, 9, 10, x, n, j)) {
//         return j + 10;
//       }
//       continue;
//     }
//     if (x[j + 4] == ' ' &&
//         jchars_are_numbers(5, 6, 7, x, n, j)) {
//       if (x[j + 8] == ' ' &&
//           jchars_are_numbers(9, 10, 11, x, n, j)) {
//         return j + 11;
//       }
//       if (jchars_are_numbers(8, 9, 10, x, n, j)) {
//         return j + 10;
//       }
//       j += 4;
//     }
//
//
//   }
//   return 0;
// }
//
// int is_au_landline(const char * x, int n) {
//   switch(n) {
//   case 27:
//     return 0;
//   }
//   return 0;
// }


int extract_au_mobile(const char * x, int n) {
  if (n < 9) {
    return NA_INTEGER;
  }
  unsigned int o = 0;
  if (n <= 10) {
    int j1 = x[0] == '0';
    for (int j = j1; j < n; ++j) {
      char xj = x[j];
      if (!isdigit(xj)) {
        return NA_INTEGER;
      }
      o = (o << 1) + (o << 3);
      o += xj - '0';
    }
    return intIsMobRange(o) ? o : NA_INTEGER;
  }

  int j1 = 0;
  while (j1 < n && !isdigit(x[j1]) && x[j1] != '+') {
    ++j1;
  }
  if (j1 + 9 >= n) {
    return NA_INTEGER;
  }
  if (x[j1] == '+') {
    if (x[j1 + 1] != '6') {
      return NA_INTEGER;
    }
    ++j1;
  }
  if (x[j1] == '6') {
    ++j1;
  }
  if (x[j1] == '1') {
    ++j1;
  }
  if (x[j1] == ' ') {
    ++j1;
  }
  if (x[j1] == '0') {
    ++j1;
  }

  if (x[j1] == '4') {
    o = 4;
    ++j1;
    for (int j = j1; j < n; ++j) {
      char xj = x[j];
      if (!isdigit(xj)) {
        // spaces are ok between digits
        if (xj == ' ') {
          continue;
        }
        // if it's not a digit or space, possibly extraneous text
        // hopefully the mobile is formed but we shouldn't add digits
        // trailing this text
        break;
      }
      // Same as o *= 10 but for unsigned int to avoid overflow
      o = (o << 1) + (o << 3);
      o += xj - '0';
    }
    return intIsMobRange(o) ? o : NA_INTEGER;
  }
  return NA_INTEGER;
}


SEXP CStandardMobile(SEXP xx) {
  if (!isString(xx)) {
    error("`mob` was type '%s' but must be type character", type2char(TYPEOF(xx))); // # nocov
  }
  R_xlen_t N = xlength(xx);
  const SEXP * xp = STRING_PTR(xx);
  SEXP ans = PROTECT(allocVector(INTSXP, N));
  SEXP Int = PROTECT(allocVector(RAWSXP, N));
  int * restrict ansp = INTEGER(ans);
  unsigned char * restrict intp = RAW(Int);
  for (R_xlen_t i = 0; i < N; ++i) {
    SEXP CX = xp[i];
    int n = length(CX);
    intp[i] = 0;
    ansp[i] = NA_INTEGER;

    const char * x = CHAR(CX);
    int au_mob = extract_au_mobile(x, n);
    if (au_mob > 0) {
      ansp[i] = au_mob;
      intp[i] = 61;
    }
  }
  SEXP List = PROTECT(allocVector(VECSXP, 2));
  SET_VECTOR_ELT(List, 0, ans);
  SET_VECTOR_ELT(List, 1, Int);
  UNPROTECT(3);
  return List;
}

int extract_landline(const char * x, int n, unsigned int area_cd) {
  if (n < 8) {
    return NA_INTEGER;
  }
  unsigned int o = area_cd;
  if (n == 8) {
    o += atoi(x);
    return intIsAusRange(o, true) ? o : NA_INTEGER;
  }
  int ten = 1;
  if (n == 10) {
    for (int j = 9; j >= 2; --j) {
      o += ten * char2number(x[j]);
      ten *= 10;
    }
    return o;
  }

  // if (n == 12 && x[0] == '(' && x[3] == ')') {
  //   for (int j = n - 1; j >= 4; --j) {
  //     if (ten > 1e8) {
  //       break;
  //     }
  //     if (isdigit(x[j])) {
  //       o += ten * char2number(x[j]);
  //       ten *= 10;
  //     }
  //   }
  //
  //   if (isdigit(x[2])) {
  //     o += 1e8 * (x[2] - '0');
  //   }
  //   return o;
  // }
  if (x[0] == '(' && x[3] == ')') {

    o = isdigit(x[2]) ? x[2] - '0' : (area_cd / 1e8);

    for (int j = 4; j < n; ++j) {
      char xj = x[j];
      if (!isdigit(xj)) {
        // spaces are ok between digits
        if (xj == ' ') {
          continue;
        }
        // if it's not a digit or space, possibly extraneous text
        // hopefully the mobile is formed but we shouldn't add digits
        // trailing this text
        break;
      }
      // Same as o *= 10 but for unsigned int to avoid overflow
      o = (o << 1) + (o << 3);
      o += xj - '0';
    }
    return intIsAusRange(o, false) ? o : NA_INTEGER;
  }

  for (int j = n - 1; j >= 0; --j) {
    char xj = x[j];
    if (o > 1e8) {
      if (isdigit(xj)) {
        o += ten * (x[j] - '0');
        break;
      }
      o += area_cd;
      break;
    }

    if (isdigit(xj)) {
      o += ten * (x[j] - '0');
      ten *= 10;
    }
  }
  return intIsAusRange(o, false) ? o : NA_INTEGER;

}



SEXP C_DauphinLandline(SEXP xx, SEXP AreaCd) {
  const unsigned int area_cd = asInteger(AreaCd);
  if (!intIsAusRange(area_cd, false)) {
    error("`area_cd = %u` which is not a permitted area code.", area_cd); // # nocov
  }
  R_xlen_t N = xlength(xx);
  if (!isString(xx)) {
    return xx;
  }
  SEXP ans = PROTECT(allocVector(INTSXP, N));
  int * restrict ansp = INTEGER(ans);
  const SEXP * xp = STRING_PTR(xx);
  for (R_xlen_t i = 0; i < N; ++i) {
    int n = length(xp[i]);
    ansp[i] = NA_INTEGER;
    if (n < 8 || n == 27 || n == 25) {
      //  Mobile Number Not Provide
      continue; // 0Mobile Number Not Provided
    }
    const char * x = CHAR(xp[i]);
    int au_mob = extract_au_mobile(x, n);
    // If it's simply a mobile
    if (au_mob > 0) {
      ansp[i] = au_mob;
      continue;
    }
    int o = extract_landline(x, n, area_cd);
    ansp[i] = o;
  }
  UNPROTECT(1);
  return ans;
}

SEXP C_Mobile_Home(SEXP xx, SEXP yy, SEXP AreaCd) {
  const unsigned int area_cd = asInteger(AreaCd);
  if (!intIsAusRange(area_cd, false)) {
    error("`area_cd = %u` which is not a permitted area code.", area_cd); // # nocov
  }
  R_xlen_t N = xlength(xx);
  if (N != xlength(yy)) {
    error("Internal error(C_Mobile_Home): Lengths of x and y differ."); // # nocov
  }
  if (!isString(xx) || !isString(yy)) {
    error("Internal error(C_Mobile_Home): Wrong types."); // # nocov
  }
  const SEXP * xp = STRING_PTR(xx);
  const SEXP * yp = STRING_PTR(yy);

  SEXP mob = PROTECT(allocVector(INTSXP, N));
  SEXP hom = PROTECT(allocVector(INTSXP, N));

  int * restrict mobp = INTEGER(mob);
  int * restrict homp = INTEGER(hom);

  for (R_xlen_t i = 0; i < N; ++i) {
    int xn = length(xp[i]);
    int yn = length(yp[i]);

    const char * xpi = CHAR(xp[i]);
    const char * ypi = CHAR(yp[i]);

    int x_mob = extract_au_mobile(xpi, xn);
    int x_hom = extract_landline(xpi, xn, area_cd);
    int y_mob = extract_au_mobile(ypi, yn);
    int y_hom = extract_landline(ypi, yn, area_cd);

    mobp[i] = x_mob != NA_INTEGER ? x_mob : y_mob;
    homp[i] = y_hom != NA_INTEGER ? y_hom : x_hom;
  }
  SEXP ans = PROTECT(allocVector(VECSXP, 2));
  SET_VECTOR_ELT(ans, 0, mob);
  SET_VECTOR_ELT(ans, 1, hom);
  UNPROTECT(3);
  return ans;

}

SEXP C_iMobileiHome(SEXP x, SEXP y) {
  R_xlen_t N = xlength(x);
  if (N != xlength(y)) {
    error("N != xlength(y)"); // # nocov
  }
  if (!isInteger(x) || !isInteger(y)) {
    error("`x` and `y` were not both integer vectors."); // # nocov
  }
  int * xp = INTEGER(x);
  int * yp = INTEGER(y);

  for (R_xlen_t i = 0; i < N; ++i) {
    int xpi = xp[i];
    int ypi = yp[i];

    // note that if ypi = na then the below is false
    bool toggle = intIsMobRange(ypi) && !intIsMobRange(xpi);
    if (toggle) {
      xp[i] = ypi;
      yp[i] = xpi;
    }
  }
  SEXP List = PROTECT(allocVector(VECSXP, 2));
  SET_VECTOR_ELT(List, 0, x);
  SET_VECTOR_ELT(List, 1, y);
  UNPROTECT(1);
  return List;
}

SEXP PrintMobile(SEXP Mob, SEXP Ccd) {
  R_xlen_t N = xlength(Mob);
  if (!isInteger(Mob) || (TYPEOF(Ccd) != RAWSXP)) {
    warning("Internal error: Mob and Ccd not INTSXP and RAWSXP of equal length."); // # nocov
    return R_NilValue; // # nocov
  }
  const bool use_ccd = xlength(Ccd) == N;
  const unsigned char * ccd = RAW(Ccd);
  const int * mob = INTEGER(Mob);
  const R_xlen_t topn = 4;

  bool above_top = true;
  int n_digits_last = log10(N);

  for (R_xlen_t i = 0; i < N; ++i) {
    if (i > topn && i < (N - topn)) {
      if (above_top) {
        for (int lz = 0; lz < (n_digits_last - 1); ++lz) {
          Rprintf(" ");
        }
        Rprintf("---\n");
        above_top = false;
      }
      continue;
    }
    unsigned char cci = use_ccd ? ccd[i] : 61;
    unsigned int ucci = cci;
    int mobi = mob[i];
    if (mobi <= 0) {
      if (i <= topn) {
        for (int lz = 0; lz < n_digits_last; ++lz) {
          Rprintf(" ");
        }
      }
      Rprintf("%lld: NA\n");
      continue;
    }

    int m3 = mobi % 1000;
    int m2 = (mobi / 1000) % 1000;
    int m1 = (mobi / 1000000) % 1000;
    if (i <= topn) {
      for (int lz = 0; lz < n_digits_last; ++lz) {
        Rprintf(" ");
      }
    }
    Rprintf("%lld: +%d %03d %03d %03d\n", i + 1, ucci, m1, m2, m3);
  }
  return R_NilValue;
}


