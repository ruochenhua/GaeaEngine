//Package rcworld : this package create a world
package rcworld

//RcWorld : world structure
type RcWorld struct {
	systemList []rcSystem
	ready      int
	entityMgr  EntityMgr
}

func init() {
}

//AddSystem : add a system to world
//must be *RcWorld
func (w *RcWorld) AddSystem(sys rcSystem) {
	sys.Init()
	w.systemList = append(w.systemList, sys)
	w.ready = 100
}

//UpdateSystem : update system
func (w *RcWorld) UpdateSystem() bool {
	for _, sys := range w.systemList {
		if !sys.Update(&w.entityMgr) {
			return false
		}
	}

	return true
}

//NewWorld : create a new world
func NewWorld() *RcWorld {
	return new(RcWorld)
}
