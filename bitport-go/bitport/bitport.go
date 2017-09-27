package bitport

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"path/filepath"

	"golang.org/x/oauth2"
)

var apiURL = "https://api.bitport.io"

// Endpoint is oauth2 endpoint
var Endpoint = oauth2.Endpoint{
	AuthURL:  apiURL + "/v2/oauth2/authorize",
	TokenURL: apiURL + "/v2/oauth2/access-token",
}

// Client represents bitport client
type Client struct {
	client *http.Client
}

// NewClient takes authenticated client c and returns bitport client.
func NewClient(c *http.Client) *Client {
	return &Client{client: c}
}

// GetFolder lists folder with code. Empty code lists root folder.
func (c *Client) GetFolder(code Coder) ([]FileHeader, error) {
	url := apiURL + "/v2/cloud"
	if code != nil && code.Code() != "" {
		url += "/" + code.Code()
	}

	resp, err := c.client.Get(url)
	if err != nil {
		return nil, err
	}
	if resp.Body == nil {
		return nil, fmt.Errorf("bitport: could not read response")
	}
	defer resp.Body.Close()

	var f struct {
		Data []struct {
			Dirs  []*directory `json:"folders"`
			Files []*file      `json:"files"`
		} `json:"data"`
		Error respErrors `json:"errors"`
	}

	if err := json.NewDecoder(resp.Body).Decode(&f); err != nil {
		return nil, err
	}

	if f.Error.hasError() {
		return nil, f.Error
	}

	files := []FileHeader{}
	for _, d := range f.Data {
		for _, dir := range d.Dirs {
			files = append(files, dir)
		}
		for _, file := range d.Files {
			files = append(files, file)
		}
	}

	return files, nil
}

func (c *Client) getAllFiles(dir Directory) ([]FileHeader, error) {
	headers, err := c.GetFolder(dir)
	if err != nil {
		return nil, err
	}

	var (
		cwd   string
		files []FileHeader
	)
	if dir != nil {
		cwd = dir.Name()
	}

	for _, header := range headers {
		switch header.(type) {
		case *directory:
			subdir := header.(*directory)
			subdir.Filename = filepath.Join(cwd, subdir.Filename)

			dirfiles, err := c.getAllFiles(subdir)
			if err != nil {
				return nil, err
			}
			files = append(files, subdir)
			files = append(files, dirfiles...)

		case *file:
			file := header.(*file)
			file.Filename = filepath.Join(cwd, file.Name())
			files = append(files, file)

		default:
			panic("bitport: unknown header")
		}
	}
	return files, nil
}

// GetDirsFiles return all directories and files available in cloud.
// When this funcion is called FileHeader.Name() method will return
// names with relative path same as in cloud.
func (c *Client) GetDirsFiles() ([]FileHeader, error) {
	return c.getAllFiles(nil)
}

// DownloadFile writes content of file to w.
func (c *Client) DownloadFile(w io.Writer, code Coder) error {
	if code == nil || code.Code() == "" {
		return fmt.Errorf("bitport: code must not be empty")
	}

	resp, err := c.client.Get(apiURL + "/v2/files/" + code.Code() + "/download")
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	_, err = io.Copy(w, resp.Body)
	return err
}

// UserInfo constains info about user.
type UserInfo struct {
	Email string `json:"email"`
}

// UserInfo return info about user.
func (c *Client) UserInfo() (*UserInfo, error) {

	resp, err := c.client.Get(apiURL + "/v2/me")
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	var me struct {
		Data   []UserInfo `json:"data"`
		Errors respErrors `json:"errors"`
	}

	if err := json.NewDecoder(resp.Body).Decode(&me); err != nil {
		return nil, err
	}

	if me.Errors.hasError() {
		return nil, me.Errors
	}

	return &me.Data[0], nil
}

type respErrors []*respError

func (r respErrors) hasError() bool {
	return len(r) > 0
}

func (r respErrors) Error() string {
	var buf bytes.Buffer
	for i, err := range r {
		if i > 0 {
			buf.WriteString("\n")
		}
		buf.WriteString(err.Error())
	}
	return buf.String()
}

type respError struct {
	Message string `json:"message"`
	Code    int    `json:"code"`
}

func (r *respError) Error() string {
	return fmt.Sprintf("bitport: %d %s", r.Code, r.Message)
}

type Coder interface {
	Code() string
}

type FileHeader interface {
	Coder
	Name() string
	Size() int64
}

type File interface {
	FileHeader
	Checksum() uint32
}

type Directory interface {
	FileHeader
	FilesCount() int
}

type fileHeader struct {
	Filename string `json:"name"`
	Filecode string `json:"code"`
	Filesize int64  `json:"size"`
}

func (h *fileHeader) Code() string {
	return h.Filecode
}

func (h *fileHeader) Name() string {
	return h.Filename
}

func (h *fileHeader) Size() int64 {
	return h.Filesize
}

type file struct {
	fileHeader
	Crc32 uint32 `json:"crc32"`
}

func (f *file) Checksum() uint32 {
	return f.Crc32
}

type directory struct {
	fileHeader
	NFiles int `json:"files_count"`
}

func (d *directory) FilesCount() int {
	return d.NFiles
}
