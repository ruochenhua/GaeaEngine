package main

import (
	"runtime"

	"./rcgraphics"
	"./rcworld"
)

func init() {
	runtime.LockOSThread()
}

func main() {
	world := rcworld.NewWorld()

	graphics := new(rcgraphics.System)
	world.AddSystem(graphics)
	for world.UpdateSystem() {

	}
}
