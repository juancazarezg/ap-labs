// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

var (
	dir      = flag.String("server", "localhost", "Default to localhost.")
	userName = flag.String("user", "user", "Insert an username")
	reader   = bufio.NewReader(os.Stdin)
	run      = true
)

//!+
func main() {

	flag.Parse()

	conn, err := net.Dial("tcp", *dir)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Fprintln(conn, *userName)

	done := make(chan struct{})

	go func() {
		buf := make([]byte, 1024)
		for {
			n, err := conn.Read(buf)
			if err != nil {
				if err != io.EOF {
					fmt.Println("read error:", err)
				}
				break
			}
			//In here the command sent gets print on the user console
			if n > 0 {
				fmt.Printf("\r" + string(buf[:n]) + *userName + " > ")
			}
		}
		fmt.Printf("\r")
		log.Println("done")
		run = false
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(conn io.Writer) {
	//In here the command sent gets print on the user console
	for run {
		fmt.Printf(*userName + " > ")
		r, _ := reader.ReadString('\n')
		_, err := fmt.Fprint(conn, r)
		if err != nil {
			break
		}
	}
}
