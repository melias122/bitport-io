package main

import (
	"io"
)

type FileProgress struct {
	w io.WriteCloser
	// written  int64
	report func(int64)
	close  func(error)
}

func NewFileReporter(w io.WriteCloser, f func(int64), c func(error)) *FileProgress {
	p := &FileProgress{
		w:      w,
		report: f,
		close:  c,
	}
	p.report(0)
	return p
}

func (p *FileProgress) Write(buf []byte) (int, error) {
	n, err := p.w.Write(buf)
	if err != nil {
		return n, err
	}
	// p.written += int64(n)
	p.report(int64(n))
	return n, nil
}

func (p *FileProgress) Close() error {
	err := p.w.Close()
	p.close(err)
	return err
}
