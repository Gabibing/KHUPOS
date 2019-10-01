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
	Graphical* inputTarget; // input target�� �����մϴ�. nullptr �̸� �Է����� �ʽ��ϴ�.
	std::map<Graphical*, std::wstring> inputBox; // input target �� string ���� ����մϴ�.
	
	// Change Scene Flag
	// Scene ��ȯ ��, Ŭ�� ����� �����־ �ٷ� ȭ���� �ٲ������� ������ �ذ��ϱ� ���ؼ� changeSceneFlag �� ����� �ξ���.
	bool changeScene;


	// Ordering Goods Scene
	int tableNum; // ���� Table Number
	int selectedGoods; //  ������ ��ǰ Number
	int orderNum; // �ֹ� �� �� �޾Ҵ���

	int _total_sales_; //�� ����
	int _cash_sales_, _account_sales_;// ���� ����, ������ü�� ��
	std::map<int, int> _reserve_; // �غ��

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
	void drawOrderTable(); // Scene�� �׸��ϴ�.
	void orderGoods(int id); // id �� �ش��ϴ� ��ǰ�� �ֹ��մϴ�.
	void selectGoods(int id); // ��ǰ�� �����մϴ�.
	void cancelGoods(); // ���õ� ��ǰ�� ����մϴ�.
	void payment(int payType); // ���̺��� �����մϴ�.

	// Payment Scene
	void drawPaymentWindow();

	// Ư�� rect�� Ŭ���� �� �Լ��� �����մϴ�.
	Interactive* createInteraction(int x, int y, int width, int height, Function *func);
	Interactive* createInteraction(Graphical *g, Function *func);

	// �׸��� ����ϴ�.
	Graphical* createSprite(int x, int y, int width, int height, std::wstring path);
	// �ؽ�Ʈ�� ����ϴ�.
	Graphical* createText(int x, int y, int width, int height, std::wstring text);
	Graphical* createText(int x, int y, int width, int height, std::wstring text, int size);
	// �����ﰢ���� ����ϴ�.
	Graphical* createRect(int x, int y, int width, int height, Gdiplus::Color color, Gdiplus::REAL thickness = 1.0f);
};