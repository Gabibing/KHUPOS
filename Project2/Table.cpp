#include "framework.h"

/*
#테이블
- 30개의 메뉴
- 계산화면 + 더하기 빼기 버튼
- 주문하기(Scene changer)
- 결제하기(Scene changer)
*/

Table::Table()//기본 테이블 생성
{
	_state_ = 0;
	_totalPrice_ = 0;
	initializeOrder();
}

Table::Table(int state, std::vector<int> order)
{
	_state_ = state;
	_order_ = order;
	calculateTotalPrice();
	DB::getInstance().saveTable();
}

void Table::order(int id, int num) // 주문하기
{
	_order_[id] += num;
	calculateTotalPrice();
}

void Table::cancel(int id) // 주문번호 지우기
{
	if (_order_.at(id) > 0)
	{
		_order_.at(id) -= 1;
		calculateTotalPrice();
		DB::getInstance().saveTable();
	}
	bool flag = false;
	for (const auto& ent : _order_)
		if (ent > 0) flag = true;
	if (flag == false)
		changeState(0);
}

void Table::changeState(int state)
{
	_state_ = state; // 빈자리 0 / 예약 1 / 사용중 2
	DB::getInstance().saveTable();
}

void Table::calculateTotalPrice()
{
	int total = 0;
	for (int i = 0; i < DB::getInstance().getGoodsMax(); i++)
		total += _order_[i] * DB::getInstance().getGoodsPrice(i);
	_totalPrice_ = total;
}

void Table::initializeOrder()
{
	_order_.clear();
	_order_.resize(DB::getInstance().getGoodsMax());
	for (auto ent : _order_)
		ent = 0;
}

int Table::getState()
{
	return _state_;
}

int Table::getOrderNumber(int id)
{
	return _order_.at(id);
}

int Table::getTotalPrice()
{
	return _totalPrice_;
}

int Table::payDone() //주문 끝난 테이블, 총 매출 리턴 후 빈자리 상태
{
	int total = _totalPrice_;
	initializeOrder();
	_state_ = 0;
	_totalPrice_ = 0;
	DB::getInstance().saveTable();
	return total;
}