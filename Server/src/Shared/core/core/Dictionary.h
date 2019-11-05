#pragma once

class Dictionary : public Object
{
public:
	Dictionary();
	Dictionary(const Dictionary& other);
	~Dictionary();
	Dictionary& operator=(const Dictionary& other);
	virtual bool operator >> (BinaryStream& bytes);
	virtual bool operator << (BinaryStream& bytes);
	Variant& operator[](const Variant& key);

	void Add(const Variant& key, const Variant& value);
	void Remove(const Variant& key);
	bool ContainsKey(const Variant& key);
	Variant* GetVariant(const Variant& key);
	void Clear();
	const std::map<Variant, Variant>& MapVariant() const { return mMapVariant; }
	uint32 GetCount() { return (uint32)mMapVariant.size(); }
protected:
	std::map<Variant, Variant> mMapVariant;
protected:
};