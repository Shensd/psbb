#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

// program wide macros
#define VERSION "0.0.1"
#define DEFAULT_PAGE_DIR "./pages"

// request type constants
const int REQUEST_GET     = 0;
const int REQUEST_HEAD    = 1;
const int REQUEST_POST    = 2;
const int REQUEST_PUT     = 3;
const int REQUEST_DELETE  = 4;
const int REQUEST_TRACE   = 5;
const int REQUEST_OPTIONS = 6;
const int REQUEST_CONNECT = 7;
const int REQUEST_PATCH   = 8;
const int REQUEST_OTHER   = 9; // use for anything not real

// Informational 
const char RESPONSE_100[] = "HTTP/1.1 101 Continue";
const char RESPONSE_101[] = "HTTP/1.1 101 Switching Protocol";
const char RESPONSE_102[] = "HTTP/1.1 102 Processing";

// Successful
const char RESPONSE_200[] = "HTTP/1.1 200 OK";
const char RESPONSE_201[] = "HTTP/1.1 201 Created";
const char RESPONSE_202[] = "HTTP/1.1 202 Accepted";
const char RESPONSE_203[] = "HTTP/1.1 203 Non-Authoritative Information";
const char RESPONSE_204[] = "HTTP/1.1 204 No Content";
const char RESPONSE_205[] = "HTTP/1.1 205 Resent Content";
const char RESPONSE_206[] = "HTTP/1.1 206 Partial Content";
const char RESPONSE_226[] = "HTTP/1.1 226 IM Used";

// Redirection
const char RESPONSE_300[] = "HTTP/1.1 300 Multiple Choice";
const char RESPONSE_301[] = "HTTP/1.1 301 Moved Permanently";
const char RESPONSE_302[] = "HTTP/1.1 302 Found";
const char RESPONSE_303[] = "HTTP/1.1 303 See Other";
const char RESPONSE_304[] = "HTTP/1.1 304 Not Modified";
const char RESPONSE_305[] = "HTTP/1.1 305 Use Proxy";
const char RESPONSE_306[] = "HTTP/1.1 306 You Shouldn't Have Gotten This"; 
const char RESPONSE_307[] = "HTTP/1.1 307 Temporary Redirect";
const char RESPONSE_308[] = "HTTP/1.1 308 Permanent Redirect";

// Client Error
const char RESPONSE_400[] = "HTTP/1.1 400 Bad Request";
const char RESPONSE_401[] = "HTTP/1.1 401 Unauthorized";
const char RESPONSE_402[] = "HTTP/1.1 402 Payment Required";
const char RESPONSE_403[] = "HTTP/1.1 403 Forbidden";
const char RESPONSE_404[] = "HTTP/1.1 404 Not Found";
const char RESPONSE_405[] = "HTTP/1.1 405 Method Not Allowed";
const char RESPONSE_406[] = "HTTP/1.1 406 Not Acceptable";
const char RESPONSE_407[] = "HTTP/1.1 407 Proxy Authentication Required";
const char RESPONSE_408[] = "HTTP/1.1 408 Request Timeout";
const char RESPONSE_409[] = "HTTP/1.1 409 Conflict";
const char RESPONSE_410[] = "HTTP/1.1 410 Gone";
const char RESPONSE_411[] = "HTTP/1.1 411 Length Required";
const char RESPONSE_412[] = "HTTP/1.1 412 Precondition Failed";
const char RESPONSE_413[] = "HTTP/1.1 413 Payload Too Large";
const char RESPONSE_414[] = "HTTP/1.1 414 URI Too Long";
const char RESPONSE_415[] = "HTTP/1.1 415 Unsupported Media Type";
const char RESPONSE_416[] = "HTTP/1.1 416 Requested Range Not Satisfiable";
const char RESPONSE_417[] = "HTTP/1.1 417 Expectation Failed";
const char RESPONSE_418[] = "HTTP/1.1 418 I'm a teapot";
const char RESPONSE_421[] = "HTTP/1.1 421 Misdirected Request";
const char RESPONSE_425[] = "HTTP/1.1 425 Too Early";
const char RESPONSE_426[] = "HTTP/1.1 426 Upgrade Required";
const char RESPONSE_428[] = "HTTP/1.1 428 Precondition Required";
const char RESPONSE_429[] = "HTTP/1.1 429 Too Many Requests";
const char RESPONSE_431[] = "HTTP/1.1 431 Request Header Fields Too Large";
const char RESPONSE_451[] = "HTTP/1.1 451 Unavailable For Legal Reasons";

// Server Error
const char RESPONSE_500[] = "HTTP/1.1 500 Internal Server Error";
const char RESPONSE_501[] = "HTTP/1.1 501 Not Implemented";
const char RESPONSE_502[] = "HTTP/1.1 502 Bad Gateway";
const char RESPONSE_503[] = "HTTP/1.1 503 Service Unavailable";
const char RESPONSE_504[] = "HTTP/1.1 504 Gateway Timeout";
const char RESPONSE_505[] = "HTTP/1.1 505 HTTP Version Not Supported";
const char RESPONSE_506[] = "HTTP/1.1 506 Variant Also Negotiates";
const char RESPONSE_507[] = "HTTP/1.1 507 Insufficient Storage";
const char RESPONSE_508[] = "HTTP/1.1 508 Loop Detected";
const char RESPONSE_510[] = "HTTP/1.1 510 Not Extended";
const char RESPONSE_511[] = "HTTP/1.1 511 Network Authentication Required";


#endif // !DEFINITIONS_HPP