# Generated by using Rcpp::compileAttributes() -> do not edit by hand
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#' Parse an HTTP request
#'
#' You can use the non- `_raw` version on input you know for sure is plain text
#'
#' @param req HTTP request character string
#' @param headers_lowercase if `TRUE` (the default) names in the `headers` data frame
#'        element are converted to lower case
#' @export
#' @examples
#' paste0(c(
#'   "GET /uri.cgi HTTP/1.1\r\n",
#'   "User-Agent: Mozilla/5.0\r\n",
#'   "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",
#'   "Host: 127.0.0.1\r\n", "\r\n"
#' ), collapse = "") -> req
#'
#' res <- parse_request(req)
#' res <- parse_request_raw(charToRaw(req))
parse_request <- function(req, headers_lowercase = TRUE) {
    .Call(`_construe_parse_request`, req, headers_lowercase)
}

#' @rdname parse_request
#' @param req HTTP request character string
#' @param headers_lowercase if `TRUE` (the default) names in the `headers` data frame
#'        element are converted to lower case
#' @export
parse_request_raw <- function(req, headers_lowercase = TRUE) {
    .Call(`_construe_parse_request_raw`, req, headers_lowercase)
}

#' Parse an HTTP response
#'
#' You can use the non- `_raw` version on input you know for sure is plain text
#'
#' @param resp HTTP response character string
#' @param headers_lowercase if `TRUE` (the default) names in the `headers` data frame
#'        element are converted to lower case
#' @export
#' @examples
#' paste0(c(
#'   "HTTP/1.1 200 OK\r\n",
#'   "Server: nginx/1.2.1\r\n",
#'   "Content-Type: text/html\r\n",
#'   "Content-Length: 8\r\n",
#'   "Connection: keep-alive\r\n",
#'   "\r\n",
#'   "<html />"
#' ), collapse = "") -> resp
#'
#' res <- parse_response(resp)
#' res <- parse_response_raw(charToRaw(resp))
parse_response <- function(resp, headers_lowercase = TRUE) {
    .Call(`_construe_parse_response`, resp, headers_lowercase)
}

#' @rdname parse_response
#' @param resp HTTP request character string
#' @param headers_lowercase if `TRUE` (the default) names in the `headers` data frame
#'        element are converted to lower case
#' @export
parse_response_raw <- function(resp, headers_lowercase = TRUE) {
    .Call(`_construe_parse_response_raw`, resp, headers_lowercase)
}

#' Parse URLs
#'
#' @param urls character vector of URLs
#' @export
#' @examples
#' URL <- "http://www.example.com/dir/subdir?param=1&param=2;param%20=%20#fragment"
#' parse_url(URL)
parse_url <- function(urls) {
    .Call(`_construe_parse_url`, urls)
}

#' Read in a file, fast and raw
#'
#' @param fil file to read in (no path expansion is performed)
#' @param buffer_size larger buffer sizes may speed up reading of
#'        very large files. It can also hurt performance, and this
#'        function reads in the entire file into memory, so a
#'        large buffer size also means more (temporary) memory will
#'        be allocated.
#' @export
#' @examples
#' read_file_raw(system.file("extdat", "example.hdr", package = "construe"))
read_file_raw <- function(fil, buffer_size = 16384L) {
    .Call(`_construe_read_file_raw`, fil, buffer_size)
}

