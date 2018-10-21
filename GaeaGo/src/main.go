package main

import (
	"./rcgraphics"
)

var (
	triangle = []float32{
		0, 0.5, 0, // top
		-0.5, -0.5, 0, // left
		0.5, -0.5, 0, // right
	}
)

func main() {
	window := rcgraphics.InitGlfw()
	prog := rcgraphics.InitOpenGL()
	vao := rcgraphics.CreateVao(triangle)
	// window := initGlfw()
	// defer glfw.Terminate()

	// program := initOpenGL()
	for !window.ShouldClose() {
		//todo
		rcgraphics.Draw(vao, window, prog)
	}
}
