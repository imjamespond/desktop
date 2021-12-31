package output

/*
#cgo windows CFLAGS: -DCGO_OS_WINDOWS=1 -I .
#cgo LDFLAGS: -L${SRCDIR} -ltest1

#include "test1.h"
#include <stdlib.h>

int Add(int a, int b);
char* Hello(char* str);

*/
import "C"
import (
	"fmt"
	"syscall"
	"unsafe"
)

func TestDll1() {
	fmt.Println(C.Add(11, 22))
	foobar := C.CString("Foobar")
	defer C.free(unsafe.Pointer(foobar))
	hello := C.Hello(foobar)
	defer C.free(unsafe.Pointer(hello))
	fmt.Println(C.GoString(hello))
}

func TestDll12() {
	dll := syscall.NewLazyDLL("./test1.dll")
	// add := dll.NewProc("Sum")
	// r1, r2, err := add.Call(22, 33)
	// fmt.Println(r1, r2, err)
	hello := dll.NewProc("Hello")
	name := C.CString("Andy")
	defer C.free(unsafe.Pointer(name))
	r1, r2, err := hello.Call(uintptr(unsafe.Pointer(name)))
	rs := (*C.char)(unsafe.Pointer(r1))
	defer C.free(unsafe.Pointer(rs))
	fmt.Println(C.GoString(rs), r2, err)
}
