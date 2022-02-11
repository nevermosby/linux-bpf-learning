package main

import (
	"C"
	// not working for "github.com/aquasecurity/libbpfgo"
	// use the old version instead
	bpf "github.com/aquasecurity/tracee/libbpfgo"
)
import (
	"fmt"
	"os"
	"os/signal"
)

func main() {
	// use signal to handle the exit finalizer
	sig := make(chan os.Signal, 1)
	signal.Notify(sig, os.Interrupt)

	bpfModule, err := bpf.NewModuleFromFile("hellokprobe.o")
	must(err)
	defer bpfModule.Close()

	err = bpfModule.BPFLoadObject()
	must(err)

	prog, err := bpfModule.GetProgram("hellokprobe")
	must(err)
	_, err = prog.AttachKprobe(sys_execve)
	must(err)

	// print the output from the default bpf pipe
	go bpf.TracePrint()

	<-sig
	fmt.Println("Shuting down...")
}

func must(err error) {
	if err != nil {
		panic(err)
	}
}
