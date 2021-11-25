package main

import (
	"io"
	"log"
	"net"
	"os"
)

func copy(writer io.Writer, reader io.Reader, res chan int) {
	if _, err := io.Copy(writer, reader); err != nil {
		log.Fatal(err)
	}

	res <- 1
}

func main() {
	if len(os.Args) < 4 {
		log.Fatal("Try using this command: go run clockWall.go NewYork=localhost:8010 Tokyo=localhost:8020 London=localhost:8030")
	}

	res := make(chan int)

	for i := 1; i <= 3; i++ {
		str := os.Args[i]
		conn, err := net.Dial("tcp", "localhost:"+str[len(str)-4:])
		if err != nil {
			log.Fatal(err)
		}

		go copy(os.Stdout, conn, res)
	}
	<-res
}
