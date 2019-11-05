#pragma once


class ShopModule : public Module
{
public:
	DECLARE_CLASS(ShopModule)
public:
	ShopModule();
	~ShopModule();
public:
	Product* AddProduct(Product* product);
	Product* FindProduct(uint32 insId);
	void DelProduct(uint32 insId);

	void DoAddProduct(Player* aPlr, uint32 productId);
	void DoFindProductList(Player* aPlr, uint32 idx, uint32 count);
public:
	void OnSellProduct(Player* aPlr, Product* product);
protected:
	virtual bool Initialize();
	virtual bool Update(float time, float delay);
	virtual bool Destroy();
	virtual bool onEnterWorld(Player* player, Dictionary& dict);
	virtual bool onLeaveWorld(Player* player, Dictionary& dict);
	virtual bool onLoad(Player* player, Dictionary& bytes);
	virtual bool onSave(Player* player, Dictionary& bytes);
protected:
	std::map<uint32, Product*>			mMapProduct;
};