package main

import (
	"fmt"
	"runtime"
	"time"
)

func main() {

	var memory runtime.MemStats
	runtime.ReadMemStats(&memory)
	channel := make(chan int)
	maxStages := 0

	defer func() {
		if goroutine := recover(); goroutine != nil {
			fmt.Println("Maximum number of pipeline stages   : ", maxStages)
			startingTime := time.Now()
			for ; maxStages > 0; maxStages-- {
				channel <- maxStages
			}
			transitTime := time.Since(startingTime)
			fmt.Println("Time to transit trough the pipeline : ", transitTime)
		}

	}()

	for {
		go func() {
			maxStages++
			<-channel
		}()
		runtime.ReadMemStats(&memory)
		if memory.TotalAlloc/1024/1024 > 256 {
			panic("MAXIMUM AMOUNT OF MEMORY REACHED")
		}
	}
}
