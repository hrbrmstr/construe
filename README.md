
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
## 1 user-agent                                                     Mozilla/5.0
## 2     accept text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
## 3       host                                                       127.0.0.1
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
## 1 user-agent                                                     Mozilla/5.0
## 2     accept text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
## 3       host                                                       127.0.0.1
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
##               expr     min       lq     mean   median       uq     max neval
##      parse_request 280.122 298.3705 331.3345 310.9885 330.9345 681.193   100
##  parse_request_raw 278.973 296.7420 326.5637 311.9000 327.2100 596.188   100
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
## [1] 0
## 
## $keepalive
## [1] TRUE
## 
## $headers
##             name       value
## 1         server nginx/1.2.1
## 2   content-type   text/html
## 3 content-length           8
## 4     connection  keep-alive
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
## [1] 0
## 
## $keepalive
## [1] TRUE
## 
## $headers
##             name       value
## 1         server nginx/1.2.1
## 2   content-type   text/html
## 3 content-length           8
## 4     connection  keep-alive
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
##                expr     min      lq     mean   median      uq     max neval
##      parse_response 275.622 290.722 307.2666 302.0490 316.040 477.767   100
##  parse_response_raw 276.222 288.578 309.7851 300.0225 313.248 506.450   100
```

### curl output example

`HEAD` request:

``` r
sys::exec_internal(
  cmd = "curl", 
  args = c("--include", "--head", "--silent", "https://httpbin.org/")
) -> res

str(parse_response(rawToChar(res$stdout)), 2)
## List of 7
##  $ status_msg : chr "OK"
##  $ status_code: num 200
##  $ vers_maj   : int 1
##  $ vers_min   : int 0
##  $ keepalive  : logi TRUE
##  $ headers    :'data.frame': 7 obs. of  2 variables:
##   ..$ name : chr [1:7] "date" "content-type" "content-length" "connection" ...
##   ..$ value: chr [1:7] "Sun, 30 Aug 2020 14:47:54 GMT" "text/html; charset=utf-8" "9593" "keep-alive" ...
##  $ content    : raw(0) 
##  - attr(*, "class")= chr [1:2] "http_response" "list"
```

`GET` request:

``` r
sys::exec_internal(
  cmd = "curl", 
  args = c("--include", "--silent", "https://httpbin.org/")
) -> res

str(parse_response_raw(res$stdout), 2)
## List of 7
##  $ status_msg : chr "OK"
##  $ status_code: num 200
##  $ vers_maj   : int 1
##  $ vers_min   : int 0
##  $ keepalive  : logi TRUE
##  $ headers    :'data.frame': 7 obs. of  2 variables:
##   ..$ name : chr [1:7] "date" "content-type" "content-length" "connection" ...
##   ..$ value: chr [1:7] "Sun, 30 Aug 2020 14:47:54 GMT" "text/html; charset=utf-8" "9593" "keep-alive" ...
##  $ content    : raw [1:9593] 3c 21 44 4f ...
##  - attr(*, "class")= chr [1:2] "http_response" "list"
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
##       expr     min      lq     mean  median       uq      max neval
##  parse_url 692.524 739.097 797.8908 771.758 824.2855 1049.674   100
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
## [1] 0
## 
## $keepalive
## [1] TRUE
## 
## $headers
##                         name
## 1                       date
## 2               content-type
## 3             content-length
## 4                 connection
## 5                       etag
## 6                     pragma
## 7              cache-control
## 8                    expires
## 9            x-frame-options
## 10                set-cookie
## 11                set-cookie
## 12                set-cookie
## 13                set-cookie
## 14                set-cookie
## 15 strict-transport-security
## 16          x-xss-protection
## 17    x-content-type-options
## 18   content-security-policy
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
| C/C++ Header |        5 | 0.21 | 1556 | 0.40 |         122 | 0.23 |       66 | 0.11 |
| C++          |        2 | 0.08 |  300 | 0.08 |          95 | 0.18 |      101 | 0.17 |
| Rmd          |        1 | 0.04 |   68 | 0.02 |          43 | 0.08 |       47 | 0.08 |
| R            |        3 | 0.12 |   22 | 0.01 |           9 | 0.02 |       80 | 0.14 |
| YAML         |        1 | 0.04 |   22 | 0.01 |           2 | 0.00 |        2 | 0.00 |
| SUM          |       12 | 0.50 | 1968 | 0.50 |         271 | 0.50 |      296 | 0.50 |

clock Package Metrics for construe

## Code of Conduct

Please note that this project is released with a Contributor Code of
Conduct. By participating in this project you agree to abide by its
terms.
