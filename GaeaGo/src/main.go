package main

import (
	"./rcgraphics"
	"./rcworld"
)

func main() {
	graphics := new(rcgraphics.System)
	world := rcworld.NewWorld()
	world.AddSystem(graphics)

	for {
		world.UpdateSystem()
	}
}
