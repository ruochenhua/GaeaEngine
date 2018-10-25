package rcworld

//Entity : entity
type Entity struct {
	eID      int
	tagArray map[string]bool
}

//AddTag : add tag to entity
func (e *Entity) AddTag(tag string) {
	e.tagArray[tag] = true
}

//RemoveTag : remove tag to entity
func (e *Entity) RemoveTag(tag string) {
	delete(e.tagArray, tag)
}

//EntityMgr : entity manager
type EntityMgr struct {
	newEntityID int
	entityArray [1000]Entity
	Test        int
}

//NewEntityMgr : create a new entity manager
func NewEntityMgr() *EntityMgr {
	return new(EntityMgr)
}

//AddEntity : add an entity
func (eMgr *EntityMgr) AddEntity(initTags []string) int {
	if eMgr.newEntityID > 1000 {
		panic("reach max entity")
	}

	nID := eMgr.newEntityID
	newEntity := eMgr.entityArray[nID]
	newEntity.tagArray = make(map[string]bool)

	for _, t := range initTags {
		newEntity.AddTag(t)
	}

	eMgr.newEntityID++

	return nID
}

//RemoveEntity : remove an entity
func (eMgr *EntityMgr) RemoveEntity(id int) {
	nID := eMgr.newEntityID - 1
	if id > nID || id < 0 {
		panic("entity id invalid")
	}

	eMgr.entityArray[id] = eMgr.entityArray[nID]
	eMgr.newEntityID--
}
