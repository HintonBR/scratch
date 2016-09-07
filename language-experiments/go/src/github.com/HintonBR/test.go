package main

import (
	"bufio"
	"fmt"
	"os"
)

//
func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	inputString := scanner.Text()
	fmt.Printf(inputString + "\n")

	fmt.Printf("Testing \n")
}
