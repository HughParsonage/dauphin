#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP C_CCRequired(SEXP, SEXP);
extern SEXP Cgrepl_digit(SEXP);
extern SEXP Cgsub_09(SEXP);
extern SEXP CStandardMobile(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"C_CCRequired",    (DL_FUNC) &C_CCRequired,    2},
    {"Cgrepl_digit",    (DL_FUNC) &Cgrepl_digit,    1},
    {"Cgsub_09",        (DL_FUNC) &Cgsub_09,        1},
    {"CStandardMobile", (DL_FUNC) &CStandardMobile, 1},
    {NULL, NULL, 0}
};

void R_init_dauphin(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
