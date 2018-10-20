#ifndef __HAP_COMPATIBILITY_H
#define __HAP_COMPATIBILITY_H


#if defined _WIN32 || defined __CYGWIN__
#define HAP_MODULE_EXPORT __declspec(dllexport)
#else
#if __GNUC__ >= 4
#define HAP_MODULE_EXPORT __attribute__ ((visibility ("default")))
#else
#define HAP_MODULE_EXPORT
#endif
typedef int errno_t;
#define strtok_s(token, delimiter, _) strtok(token, delimiter)
#define fopen_s(stream, filename, mode) ((*(stream))=fopen((filename),(mode)))==0
#define strcpy_s(destination, _, source) (strcpy(destination, source))==0
#define strerror_s(message, length, error) sprintf(message, "error code %d", error)
#endif


#endif
