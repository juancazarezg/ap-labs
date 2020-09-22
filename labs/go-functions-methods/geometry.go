// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

// Point this is a point
type Point struct{ x, y float64 }

// X returns point x value
func (p Point) X() float64 {
	return p.x
}

// Y returns point y value
func (p Point) Y() float64 {
	return p.y
}

// Distance traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// Distance same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Round(math.Hypot(q.x-p.x, q.y-p.y)*100) / 100
}

func onSegment(p, q, r Point) bool {
	if (q.X() <= math.Max(p.X(), r.X())) && (q.X() >= math.Min(p.X(), r.X())) && (q.Y() <= math.Max(q.Y(), r.Y())) && (q.Y() >= math.Min(p.Y(), r.Y())) {
		return true
	}
	return false
}

func orientation(p, q, r Point) int {
	val := (q.Y()-p.Y())*(r.X()-q.X()) - (q.X()-p.X())*(r.Y()-q.Y())
	if val == 0 {
		return 0
	}
	if val > 0 {
		return 1
	}
	return 2
}

func doIntersect(p1, q1, p2, q2 Point) bool {
	o1 := orientation(p1, q1, p2)
	o2 := orientation(p1, q1, q2)
	o3 := orientation(p2, q2, p1)
	o4 := orientation(p2, q2, q1)

	if (o1 != o2) && (o3 != o4) {
		return true
	}

	if (o1 == 0) && onSegment(p1, p2, q1) {
		return true
	}

	if (o2 == 0) && onSegment(p1, q2, q1) {
		return true
	}

	if (o3 == 0) && onSegment(p2, p1, q2) {
		return true
	}

	if (o4 == 0) && onSegment(p2, q1, q2) {
		return true
	}

	return false
}

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return math.Round(sum*100) / 100
}

// Function to check if points intersect
func (path Path) pointsIntersect() bool {
	if len(path) <= 3 {
		return false
	}
	intersect := false
	for i := 0; i < len(path)-3; i++ {
		for j := 0; j < i+1; j++ {
			intersect = doIntersect(path[j], path[j+1], path[len(path)-2], path[len(path)-1])
		}
	}
	return intersect
}

func randomFloat() float64 {
	seed := rand.NewSource(time.Now().UnixNano())
	random := rand.New(seed)
	return math.Round((((random.Float64() * 200) - 100) * 100) / 100)
}

func main() {
	if len(os.Args) > 1 {
		sides, _ := strconv.Atoi(os.Args[1])
		if sides < 3 {
			fmt.Println("ERROR: Assign more than 2 sides")
			return
		}
		fmt.Printf("- Generating a [%d] sides figure\n", sides)
		fmt.Println("- Figure's vertices")

		figure := Path{}
		for i := 0; i < sides; i++ {
			figure = append(figure, Point{randomFloat(), randomFloat()})
			for figure.pointsIntersect() {
				figure[i] = Point{randomFloat(), randomFloat()}
			}
			fmt.Printf("	- ( %.0f, %.0f)\n", figure[i].X(), figure[i].Y())
		}

		fmt.Println("- Figure's Perimeter")
		var perimeter = 0.0
		fmt.Printf("- ")
		fmt.Printf("%.2f", figure[0].Distance(figure[(1)%sides]))
		perimeter = perimeter + figure[0].Distance(figure[(1)%sides])
		for i := 1; i < sides; i++ {
			perimeter = perimeter + figure[i].Distance(figure[(i+1)%sides])
			fmt.Printf(" + %.2f", figure[i].Distance(figure[(i+1)%sides]))

		}
		fmt.Printf(" = %.2f\n", perimeter)
	} else {
		fmt.Println("Missing number of sides")
	}
}
