#ifndef _HTTPPROTOCOL_H_
#define _HTTPPROTOCOL_H_

/* ----------------------- HTTP Status Codes  ------------------------- */

#define RESPONSE_CODES 38

#define HTTP_CONTINUE                      100
#define HTTP_SWITCHING_PROTOCOLS           101
#define HTTP_OK                            200
#define HTTP_CREATED                       201
#define HTTP_ACCEPTED                      202
#define HTTP_NON_AUTHORITATIVE             203
#define HTTP_NO_CONTENT                    204
#define HTTP_RESET_CONTENT                 205
#define HTTP_PARTIAL_CONTENT               206
#define HTTP_MULTIPLE_CHOICES              300
#define HTTP_MOVED_PERMANENTLY             301
#define HTTP_MOVED_TEMPORARILY             302
#define HTTP_SEE_OTHER                     303
#define HTTP_NOT_MODIFIED                  304
#define HTTP_USE_PROXY                     305
#define HTTP_BAD_REQUEST                   400
#define HTTP_UNAUTHORIZED                  401
#define HTTP_PAYMENT_REQUIRED              402
#define HTTP_FORBIDDEN                     403
#define HTTP_NOT_FOUND                     404
#define HTTP_METHOD_NOT_ALLOWED            405
#define HTTP_NOT_ACCEPTABLE                406
#define HTTP_PROXY_AUTHENTICATION_REQUIRED 407
#define HTTP_REQUEST_TIME_OUT              408
#define HTTP_CONFLICT                      409
#define HTTP_GONE                          410
#define HTTP_LENGTH_REQUIRED               411
#define HTTP_PRECONDITION_FAILED           412
#define HTTP_REQUEST_ENTITY_TOO_LARGE      413
#define HTTP_REQUEST_URI_TOO_LARGE         414
#define HTTP_UNSUPPORTED_MEDIA_TYPE        415
#define HTTP_INTERNAL_SERVER_ERROR         500
#define HTTP_NOT_IMPLEMENTED               501
#define HTTP_BAD_GATEWAY                   502
#define HTTP_SERVICE_UNAVAILABLE           503
#define HTTP_GATEWAY_TIME_OUT              504
#define HTTP_VERSION_NOT_SUPPORTED         505
#define HTTP_VARIANT_ALSO_VARIES           506

#define DOCUMENT_FOLLOWS    HTTP_OK
#define PARTIAL_CONTENT     HTTP_PARTIAL_CONTENT
#define MULTIPLE_CHOICES    HTTP_MULTIPLE_CHOICES
#define MOVED               HTTP_MOVED_PERMANENTLY
#define REDIRECT            HTTP_MOVED_TEMPORARILY
#define USE_LOCAL_COPY      HTTP_NOT_MODIFIED
#define BAD_REQUEST         HTTP_BAD_REQUEST
#define AUTH_REQUIRED       HTTP_UNAUTHORIZED
#define FORBIDDEN           HTTP_FORBIDDEN
#define NOT_FOUND           HTTP_NOT_FOUND
#define METHOD_NOT_ALLOWED  HTTP_METHOD_NOT_ALLOWED
#define NOT_ACCEPTABLE      HTTP_NOT_ACCEPTABLE
#define LENGTH_REQUIRED     HTTP_LENGTH_REQUIRED
#define PRECONDITION_FAILED HTTP_PRECONDITION_FAILED
#define SERVER_ERROR        HTTP_INTERNAL_SERVER_ERROR
#define NOT_IMPLEMENTED     HTTP_NOT_IMPLEMENTED
#define BAD_GATEWAY         HTTP_BAD_GATEWAY
#define VARIANT_ALSO_VARIES HTTP_VARIANT_ALSO_VARIES

#define is_HTTP_INFO(x)         (((x) >= 100)&&((x) < 200))
#define is_HTTP_SUCCESS(x)      (((x) >= 200)&&((x) < 300))
#define is_HTTP_REDIRECT(x)     (((x) >= 300)&&((x) < 400))
#define is_HTTP_ERROR(x)        (((x) >= 400)&&((x) < 600))
#define is_HTTP_CLIENT_ERROR(x) (((x) >= 400)&&((x) < 500))
#define is_HTTP_SERVER_ERROR(x) (((x) >= 500)&&((x) < 600))

#define status_drops_connection(x) (((x) == HTTP_BAD_REQUEST)           || \
                                    ((x) == HTTP_REQUEST_TIME_OUT)      || \
                                    ((x) == HTTP_LENGTH_REQUIRED)       || \
                                    ((x) == HTTP_REQUEST_ENTITY_TOO_LARGE) || \
                                    ((x) == HTTP_REQUEST_URI_TOO_LARGE) || \
                                    ((x) == HTTP_INTERNAL_SERVER_ERROR) || \
                                    ((x) == HTTP_SERVICE_UNAVAILABLE))

//HTTP Sector
#define METHODS			8
#define M_GET			0
#define M_PUT			1
#define M_POST			2
#define M_DELETE		3
#define M_CONNECT		4
#define M_OPTIONS		5
#define M_TRACE			6
#define M_INVALID		7

#define M_UNSUPPORT		9

//SIP Sector
#define M_INVITE		11
#define M_ACK			12
#define M_BYE			13
#define M_INFO			14
#define M_REGISTER		15

//RTSP Sector
//#define M_OPTIONS		21
#define M_DESCRIBE		22
#define M_ANNOUNCE		23
#define M_SETUP			24
#define M_PLAY			25
#define M_PAUSE			26
#define M_TEARDOWN		27
#define M_GET_PARAMETER	28
#define M_SET_PARAMETER	29
#define M_REDIRECT		30
#define M_RECORD		31
#define	M_EMBEDDED		32

#define CGI_MAGIC_TYPE			"application/x-httpd-cgi"
#define INCLUDES_MAGIC_TYPE		"text/x-server-parsed-html"
#define INCLUDES_MAGIC_TYPE3	"text/x-server-parsed-html3"
#define MAP_FILE_MAGIC_TYPE		"application/x-type-map"
#define ASIS_MAGIC_TYPE			"httpd/send-as-is"
#define DIR_MAGIC_TYPE			"httpd/unix-directory"
#define STATUS_MAGIC_TYPE		"application/x-httpd-status"
#define	URLENCODED_MATIC_TYPE	"application/x-www-form-urlencoded"

#define CONTENT_TYPE_XML		1
#define CONTENT_TYPE_SDP		2
#define CONTENT_TYPE_HTTP		3
#define CONTENT_TYPE_HTML		4
#define CONTENT_TYPE_OCTET		5
#define STR_CONTENT_TYPE_XML	"text/xml"
#define STR_CONTENT_TYPE_SDP	"application/sdp"
#define STR_CONTENT_TYPE_HTTP	"application/http"
#define STR_CONTENT_TYPE_HTML	"text/html"
#define STR_CONTENT_TYPE_OCTET	"application/octet-stream"

typedef enum {
	PROTOCOL_HTTP=1,
	PROTOCOL_SIP=2,
	PROTOCOL_RTSP=3
} PROTOCOL_TYPE;

#define MODEL_REQUEST	1
#define MODEL_RESPONSE	2

/* Just in case your linefeed isn't the one the other end is expecting. */
#define LF 10
#define CR 13

/* Possible values for request_rec.read_body (set by handling module):
 *    REQUEST_NO_BODY          Send 413 error if message has any body
 *    REQUEST_CHUNKED_ERROR    Send 411 error if body without Content-Length
 *    REQUEST_CHUNKED_DECHUNK  If chunked, remove the chunks for me.
 *    REQUEST_CHUNKED_PASS     Pass the chunks to me without removal.
 */
#define REQUEST_NO_BODY          0
#define REQUEST_CHUNKED_ERROR    1
#define REQUEST_CHUNKED_DECHUNK  2
#define REQUEST_CHUNKED_PASS     3

#define STR_CONTENT_TYPE		"Content-Type"
#define STR_USER_AGENT			"User-Agent"
#define STR_HOST				"Host"
#define STR_X_CLIENT_ADDRESS	"X-Client-Address"
#define STR_X_TRANSACTION_ID	"X-Transaction-ID"
#define STR_CONTENT_LENGTH		"Content-Length"

#define STR_ACCEPT				"Accept"

#define STR_VIA					"Via"
#define STR_DATE				"Date"
#define STR_SERVER				"Server"
#define STR_SET_COOKIE			"Set-Cookie"
#define STR_COOKIE				"Cookie"

#define STR_FROM				"From"
#define STR_TO					"To"
#define STR_CONTACT				"Contact"
#define STR_CSEQ				"cseq"
#define STR_AUTHORIZATION       "Authorization"
#define STR_CALLID				"Call-ID"
#define STR_MAX_FORWARDS		"Max-Forwards"

#define STR_SIP_VERSION			"SIP/2.0"
#define STR_HTTP_VERSION		"HTTP/1.1"
#define STR_RTSP_VERSION		"RTSP/1.0"

//HTTP Head Field
#define STR_ACCEPT_ENCODING		"Accept-Encoding"
#define STR_ACCEPT_LANGUAGE		"Accept-Language"
#define STR_ALLOW				"Allow"
#define STR_BANDWIDTH			"Bandwidth"
#define	STR_BLOCKSIZE			"Blocksize"
#define STR_CONFERENCE			"Conference"
#define STR_CONNECTION			"Connection"
#define STR_CONTENT_BASE		"Content-Base"
#define STR_CONTENT_ENCODING	"Content-Encoding"
#define STR_CONTENT_LANGUAGE	"Content-Language"

#define STR_RANGE				"Range"
#define STR_RTP_INFO			"RTP-Info"
#define STR_SCALE				"Scale"
#define STR_SPEED				"Speed"
#define STR_SESSION				"Session"
#define STR_TIMESTAMP			"Timestamp"
#define STR_TRANSPORT			"Transport"
#define STR_WWW_AUTHENTICATE	"WWW-Authenticate"
#define STR_LAST_MODIFIED		"Last-Modified"
#define STR_EXPIRES				"Expires"
#define STR_VARY				"Vary"
#define STR_UNSUPPORT			"Unsupported"

#define STR_CACHE_CONTROL_X		"Cache-Control"
#define STR_ACCEPT_RETRANSMIT_X "x-Accept-Retransmit"
#define STR_ACCEPT_DYNAMIC_RATE_X "x-Accept-Dynamic-Rate"
#define STR_DYNAMIC_RATE_X		"x-Dynamic-Rate"
#define STR_TRANSPORT_OPTIONS	"x-transport-options"

#define STR_DATA_PARAM			"Data_Param"

#define HTTP_SUCCESS			0
#define HTTP_ERROR_BASE			-100
#define HTTP_ERROR_NO_ERROR		0
#define HTTP_ERROR_UNSUPPORT	(HTTP_ERROR_BASE - 1)


#define LEN_HTTP_BODY		1024*3
#define LEN_HTTP_BUFFER		1024*4

#endif /*_HTTPPROTOCOL_H_*/



