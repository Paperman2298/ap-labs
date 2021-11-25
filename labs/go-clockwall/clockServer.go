package main

import (
	"flag"
	"io"
	"log"
	"net"
	"strconv"
	"time"
)

func handleConnections(c net.Conn, localPort string) {
	defer c.Close()

	var timeZone *time.Location // Checks what time zone it uses

	if localPort == "8010" {
		timeZone, _ = time.LoadLocation("US/Eastern")
	} else if localPort == "8020" {
		timeZone, _ = time.LoadLocation("Asia/Tokyo")
	} else if localPort == "8030" {
		timeZone, _ = time.LoadLocation("Europe/London")
	} else {
		timeZone, _ = time.LoadLocation("UTC") // Uses local time zone
	}

	for {
		str := "TZ=" + timeZone.String() + ":  " + time.Now().In(timeZone).Format("15:04:05\n")
		_, err := io.WriteString(c, str)
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {

	localPort := flag.Int("port", 9000, "Clock's server")
	flag.Parse()

	currentPort := strconv.Itoa(*localPort)

	listener, err := net.Listen("tcp", "localhost:"+currentPort)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}

		go handleConnections(conn, currentPort) // handle connections concurrently
	}
}
