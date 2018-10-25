package rcworld

type rcSystem interface {
	New()
	Init()
	Update(eMgr *EntityMgr) bool
	Terminate()
}
