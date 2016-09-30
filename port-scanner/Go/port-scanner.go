/*
author: Forec
last edit date: 2016/09/15
email: forec@bupt.edu.cn
LICENSE
Copyright (c) 2015-2017, Forec <forec@bupt.edu.cn>

Permission to use, copy, modify, and/or distribute this code for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

package main

import (
	"flag"
	"fmt"
	"net"
	"runtime"
	"strconv"
)

var numCores = flag.Int("cores", 2, "number of CPU cores to use")
var ip = flag.String("host", "127.0.0.1", "target ip address")

type Scanner struct {
	ip string
}

func (scanner *Scanner) scan(signal chan int, port int) {
	_, err := net.Dial("tcp", scanner.ip+":"+strconv.Itoa(port))
	if err != nil {
		signal <- 0
		return
	}
	signal <- 0
	fmt.Println("Port open:", port)
}

func main() {
	flag.Parse()
	runtime.GOMAXPROCS(*numCores)
	scanner := Scanner{*ip}
	ch := make(chan int, 65535)
	for i := 0; i <= 65535; i++ {
		go scanner.scan(ch, i)
	}
	for i := 0; i <= 65535; i++ {
		<-ch
	}
}
