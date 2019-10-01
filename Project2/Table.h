#pragma once
#include <vector>
class Table
{
private:
	std::vector<int> _order_; // 주문내용 저장 벡터
	int _totalPrice_; // 총 가격
	int _state_; // 0:빈자리 / 1:예약 / 2:사용중
	void calculateTotalPrice();
	void initializeOrder();
public:
	Table();
	Table(int state, std::vector<int> order);
	void order(int id, int num); // 주문하기
	void cancel(int id); // 주문번호 지우기
	void changeState(int state); // 상태 바꾸기
	int getState();
	int getOrderNumber(int id);
	int getTotalPrice();
	int payDone(); //주문 끝난 테이블, 총 매출 리턴 후 빈자리 상태
};
