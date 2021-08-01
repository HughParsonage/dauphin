#' Standardize Australian mobilie phone numbers
#'
#' @param x A vector, generally a character vector, in which mobile numbers
#' are expected.
#'
#' @param ignore_calling_code \code{logical(1)} Whether to ignore the calling code
#' \code{+61} in the result.
#'
#' @param mob,landline Character vectors in which mobile numbers and landline
#' numbers are expected.
#'
#' @return
#' Mobile phone numbers are represented as integer vectors. International
#' calling prefixes extend the number beyond the representation of signed
#' integers. We use \code{raw} vectors for the international prefix, if required.
#'
#'
#' If \code{ignore_calling_code = TRUE}, the integer vector is returned.
#' Elements of \code{x} for which the mobile phone number could not be
#' extracted map to \code{NA_integer_} in the result.
#'
#' If \code{ignore_calling_code = FALSE}, then a list is returned. The second element
#' of the list is the calling prefix.
#'
#' If \code{ignore_calling_code = NA} then it is set to \code{TRUE} if \code{x}
#' appears to have international prefixes already.
#'
#' @examples
#' dauphin_mobile("0400 123 456")
#' dauphin_mobile("+61400123456", ignore_calling_code = TRUE)
#'
#' dauphin_mobile_landline("0424 123 456", "03 1234 5678")
#' dauphin_mobile_landline(c("0424 123 456", "03 1234 5678"),
#'                         c(NA, "0424 123 456"))
#'
#' @export

dauphin_mobile <- function(x, ignore_calling_code = NA) {
  cc_required <- .Call("C_CCRequired", x, ignore_calling_code, PACKAGE = packageName())
  ans <- .Call("CStandardMobile", x, PACKAGE = packageName())
  if (cc_required) {
    return(.subset2(ans, 1L))
  }
  ans
}

dauphin_mobile_cc <- function(x) {
  .Call("CStandardMobile", x, PACKAGE = packageName())
}

#' @rdname dauphin_mobile
#' @export
dauphin_mobile_landline <- function(mob, landline) {
  dmob <- .Call("CStandardMobile", mob, PACKAGE = packageName())
  dhom <- .Call("CStandardHomePh", landline, 3L, PACKAGE = packageName())
  .Call("C_iMobileiHome", dmob[[1]], dhom, PACKAGE = packageName())
}

