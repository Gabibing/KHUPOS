#include "framework.h"

/*
#���̺�
- 30���� �޴�
- ���ȭ�� + ���ϱ� ���� ��ư
- �ֹ��ϱ�(Scene changer)
- �����ϱ�(Scene changer)
*/

Table::Table()//�⺻ ���̺� ����
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

void Table::order(int id, int num) // �ֹ��ϱ�
{
	_order_[id] += num;
	calculateTotalPrice();
}

void Table::cancel(int id) // �ֹ���ȣ �����
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
	_state_ = state; // ���ڸ� 0 / ���� 1 / ����� 2
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

int Table::payDone() //�ֹ� ���� ���̺�, �� ���� ���� �� ���ڸ� ����
{
	int total = _totalPrice_;
	initializeOrder();
	_state_ = 0;
	_totalPrice_ = 0;
	DB::getInstance().saveTable();
	return total;
}