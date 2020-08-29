
[![Project Status: Active – The project has reached a stable, usable
state and is being actively
developed.](https://www.repostatus.org/badges/latest/active.svg)](https://www.repostatus.org/#active)
[![Signed
by](https://img.shields.io/badge/Keybase-Verified-brightgreen.svg)](https://keybase.io/hrbrmstr)
![Signed commit
%](https://img.shields.io/badge/Signed_Commits-100%25-lightgrey.svg)
[![R-CMD-check](https://github.com/hrbrmstr/construe/workflows/R-CMD-check/badge.svg)](https://github.com/hrbrmstr/construe/actions?query=workflow%3AR-CMD-check)
[![Linux build
Status](https://travis-ci.org/hrbrmstr/construe.svg?branch=batman)](https://travis-ci.org/hrbrmstr/construe)
[![Windows build
status](https://ci.appveyor.com/api/projects/status/github/hrbrmstr/construe?svg=true)](https://ci.appveyor.com/project/hrbrmstr/construe)
[![Coverage
Status](https://codecov.io/gh/hrbrmstr/construe/branch/batman/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/construe)
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
  - `read_file_raw`: Read in a file, fast and raw

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

req_raw <- charToRaw(req)

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
## raw(0)
## 
## attr(,"class")
## [1] "http_request" "list"

parse_request_raw(req_raw)
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
## raw(0)
## 
## attr(,"class")
## [1] "http_request" "list"

microbenchmark::microbenchmark(
  parse_request = parse_request(req),
  parse_request_raw = parse_request_raw(req_raw)
)
## Unit: microseconds
##               expr     min      lq     mean  median       uq      max neval
##      parse_request 143.584 145.733 167.8424 147.296 168.3755  513.639   100
##  parse_request_raw 143.359 145.970 191.6970 151.342 187.7785 1105.030   100
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

resp_raw <- charToRaw(resp)

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
## [1] 3c 68 74 6d 6c 20 2f 3e
## 
## attr(,"class")
## [1] "http_response" "list"

parse_response_raw(resp_raw)
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
## [1] 3c 68 74 6d 6c 20 2f 3e
## 
## attr(,"class")
## [1] "http_response" "list"

microbenchmark::microbenchmark(
  parse_response = parse_response(resp),
  parse_response_raw = parse_response_raw(resp_raw)
)
## Unit: microseconds
##                expr     min       lq     mean   median      uq      max neval
##      parse_response 143.810 147.3945 171.2707 151.5035 175.551  558.811   100
##  parse_response_raw 142.829 146.1390 184.2711 152.0070 186.137 1088.825   100
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
##       expr     min      lq     mean  median      uq     max neval
##  parse_url 347.202 361.913 409.4541 377.388 412.527 837.142   100
```

### Parse headers from Palo Alto `HEAD` requests

``` r
hdr <- read_file_raw(system.file("extdat", "example.hdr", package = "construe"))

cat(rawToChar(hdr))
## HTTP/1.1 200 OK
## Date: Mon, 13 Jul 2020 11:23:49 GMT
## Content-Type: text/html; charset=UTF-8
## Content-Length: 11757
## Connection: keep-alive
## ETag: "6e185d1cea69"
## Pragma: no-cache
## Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0
## Expires: Thu, 19 Nov 1981 08:52:00 GMT
## X-FRAME-OPTIONS: DENY
## Set-Cookie: PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## Set-Cookie: PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## Set-Cookie: PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## Set-Cookie: PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## Set-Cookie: PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## Strict-Transport-Security: max-age=31536000;
## X-XSS-Protection: 1; mode=block;
## X-Content-Type-Options: nosniff
## Content-Security-Policy: default-src 'self'; script-src 'self' 'unsafe-inline'; img-src * data:; style-src 'self' 'unsafe-inline';
## 

parse_response_raw(hdr)
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
##                         name
## 1                       Date
## 2               Content-Type
## 3             Content-Length
## 4                 Connection
## 5                       ETag
## 6                     Pragma
## 7              Cache-Control
## 8                    Expires
## 9            X-FRAME-OPTIONS
## 10                Set-Cookie
## 11                Set-Cookie
## 12                Set-Cookie
## 13                Set-Cookie
## 14                Set-Cookie
## 15 Strict-Transport-Security
## 16          X-XSS-Protection
## 17    X-Content-Type-Options
## 18   Content-Security-Policy
##                                                                                                        value
## 1                                                                              Mon, 13 Jul 2020 11:23:49 GMT
## 2                                                                                   text/html; charset=UTF-8
## 3                                                                                                      11757
## 4                                                                                                 keep-alive
## 5                                                                                             "6e185d1cea69"
## 6                                                                                                   no-cache
## 7                                             no-store, no-cache, must-revalidate, post-check=0, pre-check=0
## 8                                                                              Thu, 19 Nov 1981 08:52:00 GMT
## 9                                                                                                       DENY
## 10                                      PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## 11                                      PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## 12                                      PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## 13                                      PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## 14                                      PHPSESSID=d242c76e2e6ad2e71dd0c524c63e66d0; path=/; secure; HttpOnly
## 15                                                                                         max-age=31536000;
## 16                                                                                            1; mode=block;
## 17                                                                                                   nosniff
## 18 default-src 'self'; script-src 'self' 'unsafe-inline'; img-src * data:; style-src 'self' 'unsafe-inline';
## 
## $content
## raw(0)
## 
## attr(,"class")
## [1] "http_response" "list"
```

## construe Metrics

| Lang         | \# Files |  (%) |  LoC |  (%) | Blank lines |  (%) | \# Lines |  (%) |
| :----------- | -------: | ---: | ---: | ---: | ----------: | ---: | -------: | ---: |
| C/C++ Header |        5 | 0.21 | 1561 | 0.40 |         120 | 0.23 |       54 | 0.16 |
| C++          |        2 | 0.08 |  298 | 0.08 |          97 | 0.18 |       38 | 0.11 |
| Rmd          |        1 | 0.04 |   58 | 0.01 |          36 | 0.07 |       40 | 0.12 |
| R            |        3 | 0.12 |   22 | 0.01 |           9 | 0.02 |       37 | 0.11 |
| YAML         |        1 | 0.04 |   22 | 0.01 |           2 | 0.00 |        2 | 0.01 |
| SUM          |       12 | 0.50 | 1961 | 0.50 |         264 | 0.50 |      171 | 0.50 |

clock Package Metrics for construe

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
