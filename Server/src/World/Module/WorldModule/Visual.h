
typedef std::set<Entity*> EntitySet;
typedef std::map<Entity*, EntitySet> ViewEntitySet;

class Visual
{
public:
	void SetThis(Entity* ent) { mEntThis = ent; }
	Entity* GetThis() { return mEntThis; }
	bool EnterView(Entity* tar);
	bool LeaveView(Entity* tar);
	bool ClearView();
	bool CheckView(Entity* tar);
	EntitySet& GetEntitySet() { return mObjects; }
	uint32 GetViewCount() { return (uint32)mObjects.size(); }
protected:
	Entity*		mEntThis = NULL;
	EntitySet	mObjects;
	uint8		mLine = 0;
};
