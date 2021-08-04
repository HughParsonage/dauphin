#include "dauphin.h"




SEXP Cgsub_09(SEXP xx) {
  if (!isString(xx)) {
    error("xx was type '%s' but must be a character vector.", type2char(TYPEOF(xx)));
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

bool jchar_is_number(const char * x, int j) {
  return isdigit(x[j]);
}

bool jchars_are_numbers(int jj0, int jj1, int jj2, const char * x, int n, int j) {
  int j0 = jj0 + j;
  int j1 = jj1 + j;
  int j2 = jj2 + j;
  return j2 < n && jchar_is_number(x, j0) && jchar_is_number(x, j1) && jchar_is_number(x, j2);
}



int is_04mobile_from(const char * x, int n, char char1) {

  if (n == 10) {
    if (x[0] == '0' &&
        x[1] == '4' &&
        jchar_is_number(x, 2) &&
        jchar_is_number(x, 3) &&

        jchar_is_number(x, 4) &&
        jchar_is_number(x, 5) &&
        jchar_is_number(x, 6) &&

        jchar_is_number(x, 7) &&
        jchar_is_number(x, 8) &&
        jchar_is_number(x, 9)) {
      return 9;
    }
    return 0;
  }
  if (n == 9) {
    if (x[0] == '4' &&
        jchar_is_number(x, 1) &&
        jchar_is_number(x, 2) &&

        jchar_is_number(x, 3) &&
        jchar_is_number(x, 4) &&
        jchar_is_number(x, 5) &&

        jchar_is_number(x, 6) &&
        jchar_is_number(x, 7) &&
        jchar_is_number(x, 8)) {
      return 8;
    }
    return 0;
  }
  int j = 0;
  // start from the digit just before the first '4' in the number
  // +614 we advance until we hit a plus
  //  614 we advance until we hit a 6
  while (char1 == '+' && j < n && x[j] == '+') {
    ++j;
  }
  while (char1 != '0' && j < n && x[j] == '6') {
    ++j;
  }
  for (; j < n - 9; ++j) {
    if (char1 == '0') {
      if (x[j] != '0' || x[j + 1] != '4') {
        ++j;
        continue;
      }
    } else {
      if (x[j] != '1' || x[j + 1] != '4') {
        ++j;
        continue;
      }
    }
    if (!jchars_are_numbers(2, 3, 3, x, n, j)) {
      continue;
    }
    if (jchars_are_numbers(4, 5, 6, x, n, j)) {
      if (jchars_are_numbers(7, 8, 9, x, n, j)) {
        return j + 9;
      }
      if (x[j + 7] == ' ' &&
          jchars_are_numbers(8, 9, 10, x, n, j)) {
        return j + 10;
      }
      continue;
    }
    if (x[j + 4] == ' ' &&
        jchars_are_numbers(5, 6, 7, x, n, j)) {
      if (x[j + 8] == ' ' &&
          jchars_are_numbers(9, 10, 11, x, n, j)) {
        return j + 11;
      }
      if (jchars_are_numbers(8, 9, 10, x, n, j)) {
        return j + 10;
      }
      j += 4;
    }


  }
  return 0;
}

int is_au_landline(const char * x, int n) {
  switch(n) {
  case 27:
    return 0;
  }
  return 0;
}

int extract_mobile(const char * x, int n) {
  const char prefixes[3] = {'0', '+', '6'};
  for (int p = 0; p < 3; ++p) {
    char prefix = prefixes[p];
    int j_04mob = is_04mobile_from(x, n, prefix);
    if (j_04mob) {
      unsigned int mob_no = 400000000;
      unsigned int ten = 1;
      int left_j = prefix == '0' ? 2 : (prefix == '6' ? 3 : 4);
      for (int j = j_04mob; j >= left_j; --j) {
        if (mob_no > 499999999) {
          break;
        }
        char xj = x[j];
        if (isdigit(xj)) {
          mob_no += (xj - '0');
          ten *= 10;
        }
      }
      return mob_no;
    }
  }
  return NA_INTEGER;
}


SEXP CStandardMobile(SEXP xx) {
  if (!isString(xx)) {
    error("Expected string.");
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
    if (n < 9) {
      intp[i] = 0;
      ansp[i] = NA_INTEGER;
      continue;
    }

    const char * x = CHAR(CX);
    char prefix = '0';
    int j_04mob = is_04mobile_from(x, n, '0');
    if (j_04mob == 0) {
      prefix = '+';
      j_04mob = is_04mobile_from(x, n, '+');
    }
    if (j_04mob == 0) {
      prefix = '6';
      j_04mob = is_04mobile_from(x, n, '6');
    }
    if (j_04mob) {
      // Australian number 04
      unsigned int mob_no = 400000000;
      unsigned int ten = 1;
      int left_j = prefix == '0' ? 2 : (prefix == '6' ? 3 : 4);
      for (int j = j_04mob; j >= left_j; --j) {
        if (mob_no > 499999999) {
          break;
        }
        mob_no += ten * char2number(x[j]);
        ten *= x[j] != ' ' ? 10 : 1;
      }
      ansp[i] = mob_no;
      intp[i] = 61;
      continue;
    }
    ansp[i] = 0;
    intp[i] = 1;




  }
  SEXP List = PROTECT(allocVector(VECSXP, 2));
  SET_VECTOR_ELT(List, 0, ans);
  SET_VECTOR_ELT(List, 1, Int);
  UNPROTECT(3);
  return List;
}

SEXP CStandardHomePh(SEXP xx, SEXP AreaCd) {
  const int area_cd = isInteger(AreaCd) ? asInteger(AreaCd) : 0;
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
    if (x[1] == '4' && x[0] == '0')  {
      // Likely mobile phone number
      ansp[i] = extract_mobile(x, n);
      continue;
    }
    if (x[0] == '6' && x[1] == '1' && x[2] == '4') {
      ansp[i] = extract_mobile(x, n);
      continue;
    }
    if (x[0] == '+' && x[1] == '6' && x[2] == '1' && x[3] == '4') {
      ansp[i] = extract_mobile(x, n);
      continue;
    }
    if (x[0] == '6' && x[1] == '1' && x[2] == '4') {
      ansp[i] = extract_mobile(x, n);
      continue;
    }




    int o = 0;
    int ten = 1;
    if (n == 10) {
      for (int j = 9; j >= 2; --j) {
        o += ten * char2number(x[j]);
        ten *= 10;
      }
      ansp[i] = o;
      continue;
    }
    // (03)12345678
    if (x[0] == '(' && x[3] == ')') {
      for (int j = n - 1; j >= 4; --j) {
        if (ten > 1e8) {
          break;
        }
        if (isdigit(x[j])) {
          o += ten * char2number(x[j]);
          ten *= 10;
        }
      }
      ansp[i] = o;
      continue;
    }
    if (n <= 9) {
      for (int j = n - 1; j >= 0; --j) {
        o += ten * char2number(x[j]);
        ten *= 10;
      }
      ansp[i] = o;
      continue;
    }


    for (int j = n - 1; j >= 0; --j) {
      if (ten > 1e8) {
        break;
      }
      char xj = x[j];
      if (isdigit(xj)) {
        o += ten * (x[j] - '0');
        ten *= 10;
      }
    }
    ansp[i] = o;
  }
  UNPROTECT(1);
  return ans;
}

SEXP C_iMobileiHome(SEXP x, SEXP y) {
  R_xlen_t N = xlength(x);
  if (N != xlength(y)) {
    error("N != xlength(y)");
  }
  if (!isInteger(x) || !isInteger(y)) {
    error("`x` and `y` were not both integer vectors.");
  }
  int * xp = INTEGER(x);
  int * yp = INTEGER(y);

  for (R_xlen_t i = 0; i < N; ++i) {
    int xpi = xp[i];
    int ypi = yp[i];

    // note that if ypi = na then the below is false
    bool y_mob = ypi >= 400000000 && ypi <= 499999999;
    if (y_mob && xpi == NA_INTEGER) {
      xp[i] = ypi;
      yp[i] = NA_INTEGER;
      continue;
    }



  }
  SEXP List = PROTECT(allocVector(VECSXP, 2));
  SET_VECTOR_ELT(List, 0, x);
  SET_VECTOR_ELT(List, 1, y);
  UNPROTECT(1);
  return List;
}

