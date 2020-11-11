package main

import (
	"fmt"
	"time"
)

func send(x chan int, y chan int) {
	for {
		y <- (1 + <-x)
	}
}

func main() {
	x := make(chan int)
	y := make(chan int)
	commsPerSecond := 0

	go send(x, y)
	go send(y, x)

	for i := 0; i < 1; i++ {
		x <- 0
		time.Sleep(time.Duration(1) * time.Second)
		messages := <-x
		commsPerSecond += messages
	}

	fmt.Println("Communications Per Second : ", commsPerSecond)
}
