package main

import "C"
import (
	"io/fs"
	"io/ioutil"
)

//export Sum
func Sum(a int, b int) int {
	return a + b
}

//export Add
func Add(a C.int, b C.int) C.int {
	return a + b
}

//export Hello
func Hello(str *C.char) *C.char {
	hello := "Hello " + C.GoString(str)
	cstr := C.CString(hello)
	// https://github.com/golang/go/blob/ee714947c511715c752179fe738a45045ffa205c/misc/cgo/test/issue20910.go
	// https://github.com/golang/go/blob/ee714947c511715c752179fe738a45045ffa205c/misc/cgo/test/issue20910.c
	return cstr
}

//export Print
func Print(str *C.char) {
	data := C.GoString(str)
	ioutil.WriteFile(".test", []byte(data), fs.FileMode(0777))
}

func main() {
	// Need a main function to make CGO compile package as C shared library
}
