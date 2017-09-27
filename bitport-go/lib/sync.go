package main

import (
	"os"
	"path/filepath"

	"golang.org/x/sync/errgroup"

	"github.com/melias122/bitport/bitport"
)

func syncdir(c *bitport.Client, dir string, maxWorkers int, prog *progress) error {

	files, err := c.GetDirsFiles()
	if err != nil {
		return err
	}

	var (
		g    errgroup.Group
		sema = make(chan int, maxWorkers)
	)

	for _, file := range files {

		path := filepath.Join(dir, file.Name())

		switch file.(type) {
		case bitport.Directory:
			path := path
			g.Go(func() error {
				return os.MkdirAll(path, os.ModePerm)
			})

		case bitport.File:
			file := file
			g.Go(func() error {

				// take semaphore and then release
				sema <- 1
				defer func() {
					<-sema
				}()

				if err := os.MkdirAll(filepath.Dir(path), os.ModePerm); err != nil {
					return err
				}

				// file exist, check file size
				if fi, err := os.Stat(path); !os.IsNotExist(err) {
					if file.Size() == fi.Size() {
						return nil
					}
					// TODO: check checksum
				}

				w, err := os.Create(path)
				if err != nil {
					return err
				}
				wc := prog.newReporter(w, filepath.Base(path), file.Size())
				if err := c.DownloadFile(wc, file); err != nil {
					wc.Close()
					return err
				}
				return wc.Close()
			})
		}
	}
	return g.Wait()
}
