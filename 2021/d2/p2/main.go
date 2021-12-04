package main

import (
	"bufio"
	"log"
	"os"
	"strconv"
	"strings"
)

type position struct {
	x, z, aim int
}

func check(e error, msg string) {
	if e != nil {
		log.Println(msg)
		log.Fatal(e)
	}
}

func main() {
	file, err := os.Open("input.txt")
	check(err, "main(): Could not open file")
	defer file.Close()

	var p position

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		res := strings.Fields(scanner.Text())

		step, err := strconv.Atoi(res[1])
		check(err, "main(): Failed to convert step to int")

		switch res[0] {
		case "forward":
			p.x += step
			p.z += step * p.aim
		case "down":
			p.aim += step
		case "up":
			p.aim -= step
		}
	}

	log.Println("Answer:", p.x*p.z)
}
