/* Created by "go tool cgo" - DO NOT EDIT. */

/* package _/home/melias122/code/qt/bitport-io-systray/bitport */

/* Start of preamble from import "C" comments.  */


#line 7 "/home/melias122/code/qt/bitport-io-systray/bitport/bitport.go"

// bitport_progress
typedef void* (*bitport_sync_progress_file_new_cb)(void *ctx, char *name, long long size);

static inline void*
_bitport_sync_progress_file_new_cb(bitport_sync_progress_file_new_cb cb, void *ctx, char *name, long long size) {
	return cb(ctx, name, size);
}

typedef void (*bitport_sync_progress_file_written_cb)(void *p, long long written);

static inline void
_bitport_sync_progress_file_written_cb(bitport_sync_progress_file_written_cb cb, void *p, long long written) {
	cb(p, written);
}

typedef void (*bitport_sync_progress_file_done_cb)(void *p, int status);

static inline void
_bitport_sync_progress_file_done_cb(bitport_sync_progress_file_done_cb cb, void *p, int status) {
	cb(p, status);
}

typedef struct {
	void *ctx;
	bitport_sync_progress_file_new_cb 		file_new_cb;
	bitport_sync_progress_file_written_cb	file_written_cb;
	bitport_sync_progress_file_done_cb		file_done_cb;
} bitport_sync_cb;


#line 1 "cgo-generated-wrapper"


/* End of preamble from import "C" comments.  */


/* Start of boilerplate cgo prologue.  */
#line 1 "cgo-gcc-export-header-prolog"

#ifndef GO_CGO_PROLOGUE_H
#define GO_CGO_PROLOGUE_H

typedef signed char GoInt8;
typedef unsigned char GoUint8;
typedef short GoInt16;
typedef unsigned short GoUint16;
typedef int GoInt32;
typedef unsigned int GoUint32;
typedef long long GoInt64;
typedef unsigned long long GoUint64;
typedef GoInt64 GoInt;
typedef GoUint64 GoUint;
typedef __SIZE_TYPE__ GoUintptr;
typedef float GoFloat32;
typedef double GoFloat64;
typedef float _Complex GoComplex64;
typedef double _Complex GoComplex128;

/*
  static assertion to make sure the file is being used on architecture
  at least with matching size of GoInt.
*/
typedef char _check_for_64_bit_pointer_matching_GoInt[sizeof(void*)==64/8 ? 1:-1];

typedef struct { const char *p; GoInt n; } GoString;
typedef void *GoMap;
typedef void *GoChan;
typedef struct { void *t; void *v; } GoInterface;
typedef struct { void *data; GoInt len; GoInt cap; } GoSlice;

#endif

/* End of boilerplate cgo prologue.  */

#ifdef __cplusplus
extern "C" {
#endif


/* Return type for bitport_login_token */
struct bitport_login_token_return {
	char* r0;
	int r1;
};

extern struct bitport_login_token_return bitport_login_token(char* p0, char* p1, char* p2);

//	void
//	bitport_sync(char *token, int tlen, char *directory, int max_downloads, struct bitport_sync_cb)

extern int bitport_sync(char* p0, int p1, char* p2, int p3, bitport_sync_cb p4);

#ifdef __cplusplus
}
#endif
