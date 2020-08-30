#include "request.h"
#include "httprequestparser.h"

#include "response.h"
#include "httpresponseparser.h"

#include "urlparser.h"

#include <fstream>
#include <string>
#include <sys/stat.h>
#include <Rcpp.h>

// #ifdef _WIN32
// #include <windows.h>
// #endif

using namespace Rcpp;
using namespace httpparser;

std::string str_tolower(std::string str) {
  std::transform(
    str.begin(), str.end(), str.begin(),
    [](unsigned char c){ return(std::tolower(c)); }
  );
  return(str);
}

//' Parse an HTTP request
//'
//' You can use the non- `_raw` version on input you know for sure is plain text
//'
//' @param req HTTP request character string
//' @param headers_lowercase if `TRUE` (the default) names in the `headers` data frame
//'        element are converted to lower case
//' @export
//' @examples
//' paste0(c(
//'   "GET /uri.cgi HTTP/1.1\r\n",
//'   "User-Agent: Mozilla/5.0\r\n",
//'   "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",
//'   "Host: 127.0.0.1\r\n", "\r\n"
//' ), collapse = "") -> req
//'
//' res <- parse_request(req)
//' res <- parse_request_raw(charToRaw(req))
// [[Rcpp::export]]
List parse_request(String req, bool headers_lowercase = true) {

  List l;

  Request request;
  HttpRequestParser parser;

  const char *text = req.get_cstring();

  HttpRequestParser::ParseResult res = parser.parse(request, (const unsigned char *)text, (const unsigned char *)(text + strlen(text)));

  if ((res == HttpRequestParser::ParsingCompleted) || (res == HttpRequestParser::ParsingIncompleted)) {

    StringVector names(request.headers.size());
    StringVector vals(request.headers.size());

    R_xlen_t idx = 0;
    for (std::vector<Request::HeaderItem>::const_iterator it = request.headers.begin();  it != request.headers.end(); ++it) {
      names[idx] = headers_lowercase ? str_tolower(it->name) : it->name;
      vals[idx++] = it->value;
    }

    DataFrame headers = DataFrame::create(
      _["name"] = names,
      _["value"] = vals
    );

    RawVector content(request.content.begin(), request.content.end());

    l = List::create(
      _["method"] = request.method,
      _["uri"] = request.uri,
      _["vers_maj"] = request.versionMajor,
      _["vers_min"] = request.versionMinor,
      _["keepalive"] = request.keepAlive,
      _["headers"] = headers,
      _["content"] = content
    );

    l.attr("class") = CharacterVector::create("http_request", "list");

  } else {
    Rf_error("Parse error.");
  }

  return(l);

}

//' @rdname parse_request
//' @export
// [[Rcpp::export]]
List parse_request_raw(RawVector req, bool headers_lowercase = true) {

  List l;

  Request request;
  HttpRequestParser parser;

  HttpRequestParser::ParseResult res = parser.parse(request, req.begin(), req.end());

  if ((res == HttpRequestParser::ParsingCompleted) || (res == HttpRequestParser::ParsingIncompleted)) {

    StringVector names(request.headers.size());
    StringVector vals(request.headers.size());

    R_xlen_t idx = 0;
    for (std::vector<Request::HeaderItem>::const_iterator it = request.headers.begin();  it != request.headers.end(); ++it) {
      names[idx] = headers_lowercase ? str_tolower(it->name) : it->name;
      vals[idx++] = it->value;
    }

    DataFrame headers = DataFrame::create(
      _["name"] = names,
      _["value"] = vals
    );

    RawVector content(request.content.begin(), request.content.end());

    l = List::create(
      _["method"] = request.method,
      _["uri"] = request.uri,
      _["vers_maj"] = request.versionMajor,
      _["vers_min"] = request.versionMinor,
      _["keepalive"] = request.keepAlive,
      _["headers"] = headers,
      _["content"] = content
    );

    l.attr("class") = CharacterVector::create("http_request", "list");

  } else {
    Rf_error("Parse error.");
  }

  return(l);

}

//' Parse an HTTP response
//'
//' You can use the non- `_raw` version on input you know for sure is plain text
//'
//' @param resp HTTP response character string
//' @param headers_lowercase if `TRUE` (the default) names in the `headers` data frame
//'        element are converted to lower case
//' @export
//' @examples
//' paste0(c(
//'   "HTTP/1.1 200 OK\r\n",
//'   "Server: nginx/1.2.1\r\n",
//'   "Content-Type: text/html\r\n",
//'   "Content-Length: 8\r\n",
//'   "Connection: keep-alive\r\n",
//'   "\r\n",
//'   "<html />"
//' ), collapse = "") -> resp
//'
//' res <- parse_response(resp)
//' res <- parse_response_raw(charToRaw(resp))
// [[Rcpp::export]]
List parse_response(String resp, bool headers_lowercase = true) {

  List l;

  Response response;
  HttpResponseParser parser;

  const char *text = resp.get_cstring();

  HttpResponseParser::ParseResult res = parser.parse(response, (const unsigned char *)text, (const unsigned char *)(text + strlen(text)));

  if ((res == HttpResponseParser::ParsingCompleted) || (res == HttpResponseParser::ParsingIncompleted)) {

    StringVector names(response.headers.size());
    StringVector vals(response.headers.size());

    R_xlen_t idx = 0;
    for (std::vector<Response::HeaderItem>::const_iterator it = response.headers.begin();  it != response.headers.end(); ++it) {
      names[idx] = headers_lowercase ? str_tolower(it->name) : it->name;
      vals[idx++] = it->value;
    }

    DataFrame headers = DataFrame::create(
      _["name"] = names,
      _["value"] = vals
    );

    RawVector content(response.content.begin(), response.content.end());

    l = List::create(
      _["status_msg"] = response.status,
      _["status_code"] = response.statusCode,
      _["vers_maj"] = response.versionMajor,
      _["vers_min"] = response.versionMinor,
      _["keepalive"] = response.keepAlive,
      _["headers"] = headers,
      _["content"] = content
    );

    l.attr("class") = CharacterVector::create("http_response", "list");

  } else {
    Rf_error("Parse error.");
  }

  return(l);

}

//' @rdname parse_response
//' @export
// [[Rcpp::export]]
List parse_response_raw(RawVector resp, bool headers_lowercase = true) {

  List l;

  Response response;
  HttpResponseParser parser;

  HttpResponseParser::ParseResult res = parser.parse(response, resp.begin(), resp.end());

  if ((res == HttpResponseParser::ParsingCompleted) || (res == HttpResponseParser::ParsingIncompleted)) {

    StringVector names(response.headers.size());
    StringVector vals(response.headers.size());

    R_xlen_t idx = 0;
    for (std::vector<Response::HeaderItem>::const_iterator it = response.headers.begin();  it != response.headers.end(); ++it) {
      names[idx] = headers_lowercase ? str_tolower(it->name) : it->name;
      vals[idx++] = it->value;
    }

    DataFrame headers = DataFrame::create(
      _["name"] = names,
      _["value"] = vals
    );

    RawVector content(response.content.begin(), response.content.end());

    l = List::create(
      _["status_msg"] = response.status,
      _["status_code"] = response.statusCode,
      _["vers_maj"] = response.versionMajor,
      _["vers_min"] = response.versionMinor,
      _["keepalive"] = response.keepAlive,
      _["headers"] = headers,
      _["content"] = content
    );

    l.attr("class") = CharacterVector::create("http_response", "list");

  } else {
    Rf_error("Parse error.");
  }

  return(l);

}

//' Parse URLs
//'
//' @param urls character vector of URLs
//' @export
//' @examples
//' URL <- "http://www.example.com/dir/subdir?param=1&param=2;param%20=%20#fragment"
//' parse_url(URL)
// [[Rcpp::export]]
DataFrame parse_url(std::vector < std::string > urls) {

  UrlParser parser;

  StringVector scheme(urls.size());
  StringVector username(urls.size());
  StringVector password(urls.size());
  StringVector hostname(urls.size());
  StringVector port(urls.size());
  StringVector path(urls.size());
  StringVector query(urls.size());
  StringVector fragment(urls.size());

  UrlParser u;

  for (R_xlen_t idx=0; idx<urls.size(); idx++) {

    int res = u.parse(urls[idx].c_str());

    if (res) {

      scheme[idx] = u.url.scheme;
      username[idx] = u.url.username;
      password[idx] = u.url.password;
      hostname[idx] = u.url.hostname;
      port[idx] = u.url.port;
      path[idx] = u.url.path;
      query[idx] = u.url.query;
      fragment[idx] = u.url.fragment;
      port[idx] = u.url.port;

    } else {

      scheme[idx] = NA_STRING;
      username[idx] = NA_STRING;
      password[idx] = NA_STRING;
      hostname[idx] = NA_STRING;
      port[idx] = NA_STRING;
      path[idx] = NA_STRING;
      query[idx] = NA_STRING;
      fragment[idx] = NA_STRING;
      port[idx] = NA_STRING;

    }

  }

  return(DataFrame::create(
    _["scheme"] = scheme,
    _["username"] = username,
    _["password"] = password,
    _["hostname"] = hostname,
    _["port"] = port,
    _["path"] = path,
    _["query"] = query,
    _["fragment"] = fragment
  ));

}

//' Read in a file, fast and raw
//'
//' @param fil file to read in (no path expansion is performed)
//' @param buffer_size larger buffer sizes may speed up reading of
//'        very large files. It can also hurt performance, and this
//'        function reads in the entire file into memory, so a
//'        large buffer size also means more (temporary) memory will
//'        be allocated.
//' @export
//' @examples
//' read_file_raw(system.file("extdat", "example.hdr", package = "construe"))
// [[Rcpp::export]]
RawVector read_file_raw(CharacterVector fil, int buffer_size = 16384) {

// #ifdef _WIN32
//   wchar_t* buf;
//
//   size_t len = MultiByteToWideChar(CP_UTF8, 0, fil[0].begin(), -1, NULL, 0);
//
//   if (len <= 0) Rf_error("Filenamt Unicode conversion error.")
//
//   buf = (wchar_t*)R_alloc(len, sizeof(wchar_t));
//
//   MultiByteToWideChar(CP_UTF8, 0, fil[0].begin(), -1, buf, len);
//
//   std::wstring wfil(buf, buf+len);
//   std::wifstream in(wfil, std::ios::in | std::ios::binary);
// #else
//   std::ifstream in(fil[0], std::ios::in | std::ios::binary);
// #endif

  char buf[buffer_size];
  std::ifstream in;
  in.rdbuf()->pubsetbuf(buf, sizeof buf);

  in.open(fil[0], std::ios::in | std::ios::binary);

  if (in) {

#ifdef _WIN32
    struct _stati64 st;
    _wstati64(fil[0].begin(), &st)
    // _wstati64(wfil.begin(), &st)
#else
    struct stat st;
    stat(fil[0].begin(), &st);
#endif

    RawVector out(st.st_size);

    in.seekg(0, std::ios::beg);
    in.read((char *)out.begin(), st.st_size);
    in.close();

    return(out);

  } else {
    return(R_NilValue);
  }

}

