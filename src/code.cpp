#include "request.h"
#include "httprequestparser.h"

#include "response.h"
#include "httpresponseparser.h"

#include "urlparser.h"

#include <Rcpp.h>

using namespace Rcpp;
using namespace httpparser;

//' Parse an HTTP request
//'
//' @param req HTTP request character string
//' @export
// [[Rcpp::export]]
List parse_request(String req) {

  List l;

  Request request;
  HttpRequestParser parser;

  const char *text = req.get_cstring();

  HttpRequestParser::ParseResult res = parser.parse(request, text, text + strlen(text));

  if (res == HttpRequestParser::ParsingCompleted) {

    StringVector names(request.headers.size());
    StringVector vals(request.headers.size());

    int idx = 0;
    for (std::vector<Request::HeaderItem>::const_iterator it = request.headers.begin();  it != request.headers.end(); ++it) {
      names[idx] = it->name;
      vals[idx++] = it->value;
    }

    DataFrame headers = DataFrame::create(
      _["name"] = names,
      _["value"] = vals
    );

    l = List::create(
      _["method"] = request.method,
      _["uri"] = request.uri,
      _["vers_maj"] = request.versionMajor,
      _["vers_min"] = request.versionMinor,
      _["keepalive"] = request.keepAlive,
      _["headers"] = headers,
      _["content"] = Rcpp::wrap(request.content)
    );

    l.attr("class") = CharacterVector::create("http_request", "list");

  } else {
    Rf_error("Parse error.");
  }

  return(l);

}

//' Parse an HTTP response
//'
//' @param resp HTTP response character string
//' @export
// [[Rcpp::export]]
List parse_response(String resp) {

  List l;

  Response response;
  HttpResponseParser parser;

  const char *text = resp.get_cstring();

  HttpResponseParser::ParseResult res = parser.parse(response, text, text + strlen(text));

  if (res == HttpResponseParser::ParsingCompleted) {

    StringVector names(response.headers.size());
    StringVector vals(response.headers.size());

    int idx = 0;
    for (std::vector<Response::HeaderItem>::const_iterator it = response.headers.begin();  it != response.headers.end(); ++it) {
      names[idx] = it->name;
      vals[idx++] = it->value;
    }

    DataFrame headers = DataFrame::create(
      _["name"] = names,
      _["value"] = vals
    );

    l = List::create(
      _["status_msg"] = response.status,
      _["status_code"] = response.statusCode,
      _["vers_maj"] = response.versionMajor,
      _["vers_min"] = response.versionMinor,
      _["keepalive"] = response.keepAlive,
      _["headers"] = headers,
      _["content"] = Rcpp::wrap(response.content)
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

  for (int idx=0; idx<urls.size(); idx++) {

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



