package main

import (
	"bufio"
	"log"
	"os"
)

// Error helper function
func check(e error, msg string) {
	if e != nil {
		log.Println(msg)
		log.Fatalln(e)
	}
}

// Stack data structure for runes
type Stack []rune

func (s *Stack) Clear() {
	*s = []rune{}
}

func (s *Stack) IsEmpty() bool {
	return len(*s) == 0
}

func (s *Stack) Push(r rune) {
	*s = append(*s, r)
}

func (s *Stack) Pop() (rune, bool) {
	if s.IsEmpty() {
		return rune(0), false
	} else {
		index := len(*s) - 1
		element := (*s)[index]
		*s = (*s)[:index]
		return element, true
	}
}

func (s *Stack) Display() {
	msg := "Stack["
	for _, v := range *s {
		msg += string(v) + " "
	}
	msg += "]"

	log.Println(msg)
}

// Problem specific functions
func findClosingRune(r rune) rune {
	switch r {
	case '(':
		return ')'
	case '[':
		return ']'
	case '{':
		return '}'
	case '<':
		return '>'
	default:
		log.Fatalln("findClosingRune(): Passed unsupported char")
		return '?'
	}
}

func syntaxErrorValue(r rune) int {
	switch r {
	case ')':
		return 3
	case ']':
		return 57
	case '}':
		return 1197
	case '>':
		return 25137
	default:
		return 0
	}
}

func main() {
	file, err := os.Open("ex.txt")
	check(err, "main(): Could not open file")
	defer file.Close()

	var stack Stack
	acc := 0

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		for i := range line {
			char := rune(line[i])
			switch char {
			case '(':
				fallthrough
			case '[':
				fallthrough
			case '{':
				fallthrough
			case '<':
				stack.Push(char)
			case ')':
				fallthrough
			case ']':
				fallthrough
			case '}':
				fallthrough
			case '>':
				v, exists := stack.Pop()
				if exists && findClosingRune(v) != char {
					acc += syntaxErrorValue(char)
					break
				}
			}
		}
	}

	log.Println("Part1:", acc)
}
