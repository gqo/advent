package main

import (
	"bufio"
	"log"
	"os"
	"strconv"
	"strings"
)

func check(e error, msg string) {
	if e != nil {
		log.Println(msg)
		log.Fatal(e)
	}
}

func toNums(xs []string) []int {
	var res []int
	for _, c := range xs {
		num, _ := strconv.Atoi(c)
		res = append(res, num)
	}

	return res
}

func step(fish []int) []int {
	var next = make([]int, 9)

	// All fish progress a day
	for i := 1; i < 9; i++ {
		next[i-1] = fish[i]
	}

	// Fish that are at 0 get reset to 6
	next[6] += fish[0]
	// Fish that are at 0 create new fish at 8
	next[8] += fish[0]

	return next
}

func main() {
	file, err := os.Open("input.txt")
	check(err, "main(): Could not open file")
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	text := strings.Split(scanner.Text(), ",")

	nums := toNums(text)

	// Represent fish as count of fish at each day
	var fish = make([]int, 9)
	for _, n := range nums {
		fish[n]++
	}

	days := 256

	// Progress n days
	for i := 0; i < days; i++ {
		fish = step(fish)
	}

	// Add up fish totals
	acc := 0
	for i := range fish {
		acc += fish[i]
	}

	log.Println("Answer:", acc)
}
