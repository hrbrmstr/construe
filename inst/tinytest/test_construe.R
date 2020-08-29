library(construe)

paste0(c(
  "GET /uri.cgi HTTP/1.1\r\n",
  "User-Agent: Mozilla/5.0\r\n",
  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",
  "Host: 127.0.0.1\r\n", "\r\n"
), collapse = "") -> req

res <- parse_request(req)
res <- parse_request_raw(charToRaw(req))

expect_true(res$method[1] == "GET")
expect_true(res$keepalive[1] == TRUE)
expect_true("Host" %in% res$headers$name)

paste0(c(
  "HTTP/1.1 200 OK\r\n",
  "Server: nginx/1.2.1\r\n",
  "Content-Type: text/html\r\n",
  "Content-Length: 8\r\n",
  "Connection: keep-alive\r\n",
  "\r\n",
  "<html />"
), collapse = "") -> resp

res <- parse_response(resp)
res <- parse_response_raw(charToRaw(resp))

expect_true(res$status_msg[1] == "OK")
expect_true(res$keepalive[1] == TRUE)
expect_true("Server" %in% res$headers$name)

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

res <- parse_url(turls)

expect_true(is.na(res$scheme[12]))
expect_true(res$scheme[1] == "git+ssh")

parse_response_raw(
  read_file_raw(
    system.file("extdat", "example.hdr", package = "construe")
  )
) -> res

expect_true(res$headers$name[[5]] == "ETag")




