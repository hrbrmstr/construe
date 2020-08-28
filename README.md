
[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Signed
by](https://img.shields.io/badge/Keybase-Verified-brightgreen.svg)](https://keybase.io/hrbrmstr)
![Signed commit
%](https://img.shields.io/badge/Signed_Commits-100%25-lightgrey.svg)
[![R-CMD-check](https://github.com/hrbrmstr/construe/workflows/R-CMD-check/badge.svg)](https://github.com/hrbrmstr/construe/actions?query=workflow%3AR-CMD-check)
[![Linux build
Status](https://travis-ci.org/hrbrmstr/construe.svg?branch=master)](https://travis-ci.org/hrbrmstr/construe)
[![Windows build
status](https://ci.appveyor.com/api/projects/status/github/hrbrmstr/construe?svg=true)](https://ci.appveyor.com/project/hrbrmstr/construe)
[![Coverage
Status](https://codecov.io/gh/hrbrmstr/construe/branch/master/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/construe)
![Minimal R
Version](https://img.shields.io/badge/R%3E%3D-3.6.0-blue.svg)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

# construe

HTTP Request, Response and URL Parser

## Description

A simple and fast HTTP request, response and URL parser based on the C++
‘httpparser’ library by Alex Nekipelov
(<https://github.com/nekipelov/httpparser>).

## What’s Inside The Tin

The following functions are implemented:

  - `parse_request`: Parse an HTTP request
  - `parse_response`: Parse an HTTP response
  - `parse_url`: Parse URLs

## Installation

``` r
install.packages("construe", repos = c("https://cinc.rud.is", "https://cloud.r-project.org/"))
# or
remotes::install_git("https://git.rud.is/hrbrmstr/construe.git")
# or
remotes::install_git("https://git.sr.ht/~hrbrmstr/construe")
# or
remotes::install_gitlab("hrbrmstr/construe")
# or
remotes::install_bitbucket("hrbrmstr/construe")
# or
remotes::install_github("hrbrmstr/construe")
```

NOTE: To use the ‘remotes’ install options you will need to have the
[{remotes} package](https://github.com/r-lib/remotes) installed.

## Usage

``` r
library(construe)

# current version
packageVersion("construe")
## [1] '0.1.0'
```

### Requests

``` r
paste0(c(
  "GET /uri.cgi HTTP/1.1\r\n",
  "User-Agent: Mozilla/5.0\r\n",
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",
  "Host: 127.0.0.1\r\n", "\r\n"
), collapse = "") -> req

parse_request(req)
## $method
## [1] "GET"
## 
## $uri
## [1] "/uri.cgi"
## 
## $vers_maj
## [1] 1
## 
## $vers_min
## [1] 1
## 
## $keepalive
## [1] TRUE
## 
## $headers
##         name                                                           value
## 1 User-Agent                                                     Mozilla/5.0
## 2     Accept text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
## 3       Host                                                       127.0.0.1
## 
## $content
## character(0)
## 
## attr(,"class")
## [1] "http_request" "list"

microbenchmark::microbenchmark(
  parse_request = parse_request(req)
)
## Unit: microseconds
##           expr     min       lq     mean  median      uq     max neval
##  parse_request 278.654 282.3435 300.1954 300.018 306.678 483.234   100
```

### Responses

``` r
paste0(c(
  "HTTP/1.1 200 OK\r\n",
  "Server: nginx/1.2.1\r\n",
  "Content-Type: text/html\r\n",
  "Content-Length: 8\r\n",
  "Connection: keep-alive\r\n",
  "\r\n",
  "<html />"
), collapse = "") -> resp

parse_response(resp)
## $status_msg
## [1] "OK"
## 
## $status_code
## [1] 200
## 
## $vers_maj
## [1] 1
## 
## $vers_min
## [1] 1
## 
## $keepalive
## [1] TRUE
## 
## $headers
##             name       value
## 1         Server nginx/1.2.1
## 2   Content-Type   text/html
## 3 Content-Length           8
## 4     Connection  keep-alive
## 
## $content
## [1] "<" "h" "t" "m" "l" " " "/" ">"
## 
## attr(,"class")
## [1] "http_response" "list"

microbenchmark::microbenchmark(
  parse_response = parse_response(resp)
)
## Unit: microseconds
##            expr     min       lq     mean  median      uq     max neval
##  parse_response 277.677 297.1635 323.4402 311.109 334.343 500.039   100
```

### URLs

``` r
c(
  "git+ssh://example.com/path/file",
  "https://example.com/path/file",
  "http://www.example.com/dir/subdir?param=1&param=2;param%20=%20#fragment",
  "http://www.example.com",
  "http://username@www.example.com/dir/subdir?param=1&param=2;param%20=%20#fragment",
  "http://username:passwd@www.example.com/dir/subdir?param=1&param=2;param%20=%20#fragment",
  "http://www.example.com:8080/dir/subdir?param=1&param=2;param%20=%20#fragment",
  "http://username:passwd@www.example.com:8080/dir/subdir?param=1&param=2;param%20=%20#fragment",
  "ftp://username:passwd@ftp.example.com/dir/filename.ext",
  "mailto:username@example.com",
  "svn+ssh://hostname-01.org/path/to/file",
  "xddp::://///blah.wat/?"
) -> turls

parse_url(turls)
##     scheme username password        hostname port              path                        query fragment
## 1  git+ssh                       example.com             /path/file                                      
## 2    https                       example.com             /path/file                                      
## 3     http                   www.example.com            /dir/subdir param=1&param=2;param%20=%20 fragment
## 4     http                   www.example.com                      /                                      
## 5     http username          www.example.com            /dir/subdir param=1&param=2;param%20=%20 fragment
## 6     http username   passwd www.example.com            /dir/subdir param=1&param=2;param%20=%20 fragment
## 7     http                   www.example.com 8080       /dir/subdir param=1&param=2;param%20=%20 fragment
## 8     http username   passwd www.example.com 8080       /dir/subdir param=1&param=2;param%20=%20 fragment
## 9      ftp username   passwd ftp.example.com      /dir/filename.ext                                      
## 10  mailto username              example.com                      /                                      
## 11 svn+ssh                   hostname-01.org          /path/to/file                                      
## 12    <NA>     <NA>     <NA>            <NA> <NA>              <NA>                         <NA>     <NA>

microbenchmark::microbenchmark(
  parse_url = parse_url(turls[1])
)
## Unit: microseconds
##       expr    min     lq     mean   median       uq      max neval
##  parse_url 687.11 733.38 787.4106 751.3515 809.7655 1325.274   100
```

## construe Metrics

| Lang         | \# Files |  (%) |  LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :----------- | -------: | ---: | ---: | ---: | ----------: | ---: | -------: | ---: |
| C/C++ Header |        5 | 0.21 | 1561 | 0.43 |         120 | 0.28 |       54 | 0.20 |
| C++          |        2 | 0.08 |  162 | 0.04 |          55 | 0.13 |       20 | 0.07 |
| Rmd          |        1 | 0.04 |   49 | 0.01 |          28 | 0.07 |       37 | 0.13 |
| YAML         |        1 | 0.04 |   22 | 0.01 |           2 | 0.00 |        2 | 0.01 |
| R            |        3 | 0.12 |   13 | 0.00 |           6 | 0.01 |       25 | 0.09 |
| SUM          |       12 | 0.50 | 1807 | 0.50 |         211 | 0.50 |      138 | 0.50 |

clock Package Metrics for construe

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
