#include "framework.h"
using namespace std;
void DB::initialize()
{
	logFile.open(TEXT("setting\\LogFile.txt"), ios::app);
	logFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
	loadTitle();
	loadGoods();
	loadTable();
	loadSales();
}

Table* DB::getTable(int id)
{
	return tables.at(id);
}

wstring DB::getGoodsName(int id)
{
	if (id >= goodsMax)
		return wstring();
	return goodsName[id];
}

int DB::getGoodsPrice(int id)
{
	if (id >= goodsMax)
		return 0;
	return goodsPrice[id];
}

int DB::getGoodsMax()
{
	return goodsMax;
}

int DB::getTotalSales()
{
	return _totalSales_;
}

int DB::getCashSales()
{
	return _cashSales_;
}

int DB::getAccountSales()
{
	return _accountSales_;
}

std::wstring DB::getTitle()
{
	return _title_;
}

void DB::loadSales()
{
	wifstream inSalesFile;
	inSalesFile.open(TEXT("setting\\SalesFile.txt"));
	inSalesFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));
	while (!inSalesFile.eof() && !inSalesFile.fail())
	{
		inSalesFile >> _totalSales_ >> _cashSales_ >> _accountSales_;
	}
}

void DB::loadGoods()
{
	wifstream goodsFile;
	goodsFile.open(TEXT("setting\\GoodsFile.txt"));
	goodsFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));
	for (goodsMax = 0; !goodsFile.eof() && !goodsFile.fail(); goodsMax++)
	{
		wstring name;
		int price;
		goodsFile >> name >> price;
		if (name.empty())
			break;
		goodsName.push_back(name);
		goodsPrice.push_back(price);
	}
	goodsFile.close();
}

void DB::loadTable()
{
	wifstream tableFile;
	tableFile.open(TEXT("setting\\TableFile.txt"));
	tableFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));
	for (int i = 0; i < TABLE_MAX; i++)
	{
		wstring input;
		getline(tableFile, input);
		if (tableFile.fail())
		{
			tables.push_back(new Table());
			continue;
		}
		wstringstream sstream(input);
		int state;
		vector<int> order;
		order.resize(goodsMax);
		sstream >> state;
		for (int j = 0; !sstream.eof() && j < goodsMax; j++)
		{
			sstream >> order[j];
		}
		tables.push_back(new Table(state, order));
	}
	tableFile.close();
}

void DB::loadTitle()
{
	wifstream inTitleFile;
	inTitleFile.open(TEXT("setting\\TitleFile.txt"));
	inTitleFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>));
	if (!inTitleFile.eof() && !inTitleFile.fail())
		getline(inTitleFile, _title_);
	inTitleFile.close();
}

void DB::saveTable()
{
	wofstream tableFile(TEXT("setting\\TableFile.txt"));
	tableFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
	for (auto &ent : tables)
	{
		tableFile << ent->getState() << '\t';
		for (int i = 0; i < goodsMax; i++)
			tableFile << ent->getOrderNumber(i) << '\t';
		tableFile << endl;
	}
	tableFile.close();
}

void DB::saveSales()
{
	wofstream outSalesFile("setting/SalesFile.txt");
	outSalesFile.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
	outSalesFile << _totalSales_ << '\t' << _cashSales_ << '\t' << _accountSales_ << endl;
	outSalesFile.close();
}

void DB::setSales(int total, int cash, int account)
{
	_totalSales_ = total;
	_cashSales_ = cash;
	_accountSales_ = account;
	saveSales();
}

void DB::logging(std::wstring content)
{
	SYSTEMTIME st;
	WCHAR str[128];
	GetLocalTime(&st);
	wsprintf(str, TEXT("[%04d.%02d.%02d %02d:%02d:%02d] "), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	wstring log(str);
	log += content;
	logFile << log << endl;
}

