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

func fishDays(days, birth int) int {
	acc := 0

	if (days - birth) <= 0 {
		return 0
	}

	acc++
	firstDays := days - (birth + 1)
	acc += fishDays(firstDays, 8)

	days = firstDays
	for days >= 7 {
		acc++
		days -= 7
		acc += fishDays(days, 8)
	}

	return acc
}

func main() {
	file, err := os.Open("input.txt")
	check(err, "main(): Could not open file")
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	text := strings.Split(scanner.Text(), ",")

	nums := toNums(text)

	days := 256
	acc := 0

	for _, num := range nums {
		acc += fishDays(days, num)
	}

	log.Println("Spawn#:", acc)
	log.Println("Answer:", len(nums)+acc)
}
