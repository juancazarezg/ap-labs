// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"flag"
	"fmt"
	"os"

	"gopl.io/ch5/links"
)

//!+sema
var tokens = make(chan struct{}, 20)

var depth = flag.Int("depth", 1, "Insert crawler depth")
var file = flag.String("results", "default.txt", "Insert the name of the result file")

type Site struct {
	currentDepth int
	url          string
}

func crawl(website Site) []Site {
	f, err := os.OpenFile(*file, os.O_APPEND|os.O_WRONLY, 0644)
	if err != nil {
		fmt.Println(err)
	}
	_, err = fmt.Fprintln(f, website.url)
	if err != nil {
		fmt.Println(err)
		f.Close()
	}
	err = f.Close()
	if err != nil {
		fmt.Println(err)
	}
	if website.currentDepth < *depth {
		tokens <- struct{}{} // acquire a token
		list, err := links.Extract(website.url)
		sites := make([]Site, 0)
		for _, link := range list {
			sites = append(sites, Site{url: link, currentDepth: website.currentDepth + 1})
		}
		<-tokens // release the token
		if err != nil {
		}
		return sites

	}
	return []Site{}
}

//!-sema

//!+
func main() {
	flag.Parse()
	if len(os.Args) < 4 {
		fmt.Println("Insert depth, result file name and a link to proceed.")
		return
	}
	worklist := make(chan []Site)
	var n int // number of pending sends to worklist

	// Start with the command-line arguments.
	n++

	file, err := os.Create(*file)
	if err != nil {
		fmt.Println(err)
		return
	}
	err = file.Close()
	if err != nil {
		fmt.Println(err)
		return
	}

	go func() {
		urls := make([]Site, 0)
		urls = append(urls, Site{url: (os.Args[3:])[0], currentDepth: 0})
		worklist <- urls
	}()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		list := <-worklist
		for _, link := range list {
			if !seen[link.url] {
				seen[link.url] = true
				n++
				go func(link Site) {
					worklist <- crawl(link)
				}(link)
			}
		}
	}

}

//!-
