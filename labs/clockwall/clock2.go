// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"flag"
	"io"
	"log"
	"net"
	"time"
)

func handleConn(c net.Conn) {
	defer c.Close()
	for {
		//Get time from local timezone specified at the command line
		_, err := io.WriteString(c, time.Now().Local().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	//Get the port from a golang flag at the command line
	port := flag.String("port", "port", "Insert a port")
	flag.Parse()
	//Create listener on port
	listener, err := net.Listen("tcp", "localhost:"+*port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn)
	}
}
