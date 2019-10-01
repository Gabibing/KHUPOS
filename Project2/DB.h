#pragma once
class Table;

#define TABLE_MAX 20

class DB
{
private:
	DB() = default;
	~DB() = default;
	DB(const DB&) = delete;
	DB& operator=(const DB&) = delete;

	std::wofstream logFile;

	std::vector<std::wstring> goodsName;
	std::vector<int> goodsPrice;
	int goodsMax;

	std::vector<Table*> tables;

	int _totalSales_;
	int _cashSales_;
	int _accountSales_;
	std::wstring _title_;

	void loadSales();
	void loadGoods();
	void loadTable();
	void loadTitle();

public:
	static DB& getInstance() {
		static DB instance;
		return instance;
	}
	void initialize();

	Table* getTable(int id);

	std::wstring getGoodsName(int id);
	int getGoodsPrice(int id);
	int getGoodsMax();
	int getTotalSales();
	int getCashSales();
	int getAccountSales();
	std::wstring getTitle();
	void saveTable();
	void saveSales();
	void setSales(int total, int cash, int account);
	void logging(std::wstring content);
};