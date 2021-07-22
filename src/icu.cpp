#include <Rcpp.h>
#include "unicode.h"
#include "unicode/uvernum.h"
using namespace Rcpp;

// [[Rcpp::export]]
Rcpp::List icu_info() {
  return Rcpp::List::create(
      Named("VERSION") = U_ICU_VERSION,
      Named("VERSION_MAJOR") = U_ICU_VERSION_MAJOR_NUM,
      Named("VERSION_MINOR") = U_ICU_VERSION_MINOR_NUM,
      Named("DATA_VERSION") = U_ICU_DATA_VERSION
  );
}

