//Package rcworld : this package create a world
package rcworld

import "runtime"

type rcSystem interface {
	New()
	Init()
	Update()
	Terminate()
}

//RcWorld : world structure
type RcWorld struct {
	systemList []rcSystem
	ready      int
}

func init() {
	runtime.LockOSThread()
}

//AddSystem : add a system to world
//must be *RcWorld
func (w *RcWorld) AddSystem(sys rcSystem) {
	sys.Init()
	w.systemList = append(w.systemList, sys)
	w.ready = 100
}

//UpdateSystem : update system
func (w *RcWorld) UpdateSystem() {
	for _, sys := range w.systemList {
		sys.Update()
	}
}

//NewWorld : create a new world
func NewWorld() *RcWorld {
	return new(RcWorld)
}
