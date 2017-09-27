package main

import (
	"unsafe"
)

/*
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

*/
import "C"

import (
	"context"
	"encoding/json"
	"io"
	"log"
	"sync"

	"github.com/melias122/bitport/auth"
	"github.com/melias122/bitport/bitport"
	"golang.org/x/oauth2"
)

//export bitport_login_token
func bitport_login_token(id *C.char, secret *C.char, code *C.char) (*C.char, C.int) {

	ts := auth.AccessCode{
		ClientID:     C.GoString(id),
		ClientSecret: C.GoString(secret),
		ClientCode:   C.GoString(code),
	}

	tok, err := ts.Token()
	if err != nil {
		log.Println("bitport_login_token: ", err)
		return nil, 0
	}

	data, err := json.Marshal(tok)
	if err != nil {
		log.Println("bitport_login_token: ", err)
		return nil, 0
	}

	return (*C.char)(C.CBytes(data)), C.int(len(data))
}

//export bitport_sync
//	void
//	bitport_sync(char *token, int tlen, char *directory, int max_downloads, struct bitport_sync_cb)
func bitport_sync(token *C.char, tokenLen C.int, dir *C.char, max C.int, cb C.bitport_sync_cb) C.int {
	client, err := newClient(C.GoBytes(unsafe.Pointer(token), tokenLen))
	if err != nil {
		log.Println("bitport_sync: ", err)
		return 1
	}
	err = syncdir(client, C.GoString(dir), int(max), &progress{cb: cb})
	if err != nil {
		log.Println("bitport_sync: ", err)
		return 1
	}
	return 0
}

func newClient(data []byte) (*bitport.Client, error) {
	tok := new(oauth2.Token)
	err := json.Unmarshal(data, tok)
	return bitport.NewClient(oauth2.NewClient(context.Background(), &jsonToken{tok})), err
}

type jsonToken struct {
	token *oauth2.Token
}

func (t *jsonToken) Token() (*oauth2.Token, error) {
	return t.token, nil
}

type progress struct {
	mu sync.Mutex
	cb C.bitport_sync_cb
}

func (p *progress) newReporter(w io.WriteCloser, name string, size int64) io.WriteCloser {

	ctx := C._bitport_sync_progress_file_new_cb(p.cb.file_new_cb, p.cb.ctx, C.CString(name), C.longlong(size))

	r := func(written int64) {
		C._bitport_sync_progress_file_written_cb(p.cb.file_written_cb, ctx, C.longlong(written))
	}
	c := func(err error) {
		if err != nil {
			C._bitport_sync_progress_file_done_cb(p.cb.file_done_cb, ctx, 1)
		} else {
			C._bitport_sync_progress_file_done_cb(p.cb.file_done_cb, ctx, 0)
		}
	}
	return NewFileReporter(w, r, c)
}

func main() {}
