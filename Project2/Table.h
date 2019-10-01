#pragma once
#include <vector>
class Table
{
private:
	std::vector<int> _order_; // �ֹ����� ���� ����
	int _totalPrice_; // �� ����
	int _state_; // 0:���ڸ� / 1:���� / 2:�����
	void calculateTotalPrice();
	void initializeOrder();
public:
	Table();
	Table(int state, std::vector<int> order);
	void order(int id, int num); // �ֹ��ϱ�
	void cancel(int id); // �ֹ���ȣ �����
	void changeState(int state); // ���� �ٲٱ�
	int getState();
	int getOrderNumber(int id);
	int getTotalPrice();
	int payDone(); //�ֹ� ���� ���̺�, �� ���� ���� �� ���ڸ� ����
};
