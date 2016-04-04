package main

import "fmt"
import "strings"
import "os"
import "log"
import "bufio"
import "github.com/jacobsa/go-serial/serial"
import "github.com/jessevdk/go-flags"

func main() {

	var opts struct {
		BaudRate uint   `short:"b" long:"baud" default:"9600" description:"Baud rate for transimission"`
		PortName string `short:"p" long:"port" required:"true" description:"Serial port to read"`
	}

	_, err := flags.Parse(&opts)
	if err != nil {
		os.Exit(0)
		log.Fatal(err)
	}

	options := serial.OpenOptions{
		PortName:        opts.PortName,
		BaudRate:        opts.BaudRate,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 4,
	}

	// Open the port.
	port, err := serial.Open(options)
	if err != nil {
		log.Fatalf("serial.Open: %v", err)
	}

	// Make sure to close it later.
	defer port.Close()

	scanner := bufio.NewScanner(port)
	for scanner.Scan() {
		line := scanner.Text()
		fmt.Println(line)
		if strings.HasPrefix(line, "Total:") {
			os.Exit(0)
		}
	}

	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
}
