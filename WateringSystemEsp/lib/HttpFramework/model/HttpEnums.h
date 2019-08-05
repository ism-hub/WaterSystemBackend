/*
 * HttpEnums.h
 *
 *  Created on: 9 αιεμι 2017
 *      Author: IsM
 */

#ifndef HTTP_HTTPENUMS_H_
#define HTTP_HTTPENUMS_H_

namespace Http {

enum HTTPMethod { HTTP_ANY, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_PATCH, HTTP_DELETE, HTTP_OPTIONS };
enum HTTPUploadStatus { UPLOAD_FILE_START, UPLOAD_FILE_WRITE, UPLOAD_FILE_END,
                        UPLOAD_FILE_ABORTED };
enum HTTPClientStatus { HC_NONE, HC_WAIT_READ, HC_WAIT_CLOSE };


} /* namespace Http */

#endif /* HTTP_HTTPENUMS_H_ */
