package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"strings"
	"sync"
)

var countryTz = map[string]string{
	"NewYork": "US/Eastern",
	"Tokyo":   "Asia/Tokyo",
	"London":  "Europe/London",
}

func makeCopy(location string, local string, wg *sync.WaitGroup) {
	conn, err := net.Dial("tcp", local)
	if err != nil {
		fmt.Printf("Couldn't connect to '%s=%s'\n", location, local)
		wg.Done()
		return
	}
	defer conn.Close()
	if err == nil {
		time := make([]byte, 256)
		for {
			_, err := conn.Read(time)
			if err != nil {
				if err != io.EOF {
					fmt.Println(err)
				}
				break
			}
			fmt.Printf("%s\t: %s", location, time)
		}
	}
	wg.Done()
}

func main() {

	args := os.Args[1:]
	if len(args) < 1 {
		log.Fatal("No arguments provided")
	}
	var wg sync.WaitGroup
	wg.Add(len(countryTz))

	for i := range args {
		split := strings.Split(args[i], "=")
		_, check := countryTz[split[0]]
		if check {
			go makeCopy(countryTz[split[0]], split[1], &wg)
		}
	}
	wg.Wait()

}
