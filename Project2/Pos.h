#pragma once
class Pos
{
private:
	Pos() = default;
	~Pos() = default;
	Pos(const Pos&) = delete;
	Pos& operator=(const Pos&) = delete;
	// Graphical Member
	HWND _hWnd_;
	Gdiplus::Graphics* outGraphics;
	Gdiplus::Graphics* bufGraphics;
	Gdiplus::Bitmap* bitmap;
	Gdiplus::SolidBrush* whiteBrush;

	// Sprite Member
	std::vector<Graphical*> sprite;
	// Interaction Member
	std::vector<Interactive*> interaction;

	// Input Member
	Graphical* inputTarget; // input target을 설정합니다. nullptr 이면 입력하지 않습니다.
	std::map<Graphical*, std::wstring> inputBox; // input target 별 string 값을 기억합니다.
	
	// Change Scene Flag
	// Scene 전환 후, 클릭 명령이 남아있어서 바로 화면이 바뀌어버리는 문제를 해결하기 위해서 changeSceneFlag 를 만들어 두었음.
	bool changeScene;


	// Ordering Goods Scene
	int tableNum; // 현재 Table Number
	int selectedGoods; //  선택한 상품 Number
	int orderNum; // 주문 몇 번 받았는지

	int _total_sales_; //총 매출
	int _cash_sales_, _account_sales_;// 현금 매출, 계좌이체된 돈
	std::map<int, int> _reserve_; // 준비금

public:
	static Pos& getInstance() {
		static Pos instance;
		// volatile int dummy{};
		return instance;
	}
	void initialize(HWND hWnd);

	void render();
	void update();
	void keydown(WCHAR vk);

	void clearSprite();
	void clearInteraction();
	
	// Table Scene
	void drawTable();
	void tableToOrder(int id);

	// Check Sales Scene
	void drawCheckSalesWindow();
	
	// Credit
	void drawCreditWindow();

	// Ordering Scene
	void drawOrderTable(); // Scene을 그립니다.
	void orderGoods(int id); // id 에 해당하는 상품을 주문합니다.
	void selectGoods(int id); // 상품을 선택합니다.
	void cancelGoods(); // 선택된 상품을 취소합니다.
	void payment(int payType); // 테이블을 결제합니다.

	// Payment Scene
	void drawPaymentWindow();

	// 특정 rect를 클릭할 때 함수를 실행합니다.
	Interactive* createInteraction(int x, int y, int width, int height, Function *func);
	Interactive* createInteraction(Graphical *g, Function *func);

	// 그림을 만듭니다.
	Graphical* createSprite(int x, int y, int width, int height, std::wstring path);
	// 텍스트를 만듭니다.
	Graphical* createText(int x, int y, int width, int height, std::wstring text);
	Graphical* createText(int x, int y, int width, int height, std::wstring text, int size);
	// 직각삼각형을 만듭니다.
	Graphical* createRect(int x, int y, int width, int height, Gdiplus::Color color, Gdiplus::REAL thickness = 1.0f);
};