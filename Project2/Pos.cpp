#include "framework.h"
void Pos::initialize(HWND hWnd)
{
	srand(time(NULL));
	// 그래픽 세팅
	_hWnd_ = hWnd;
	whiteBrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255));
	outGraphics = new Gdiplus::Graphics(hWnd);
	bitmap = new Gdiplus::Bitmap(WIDTH, HEIGHT);
	bufGraphics = new Gdiplus::Graphics(bitmap);


	// 세팅
	tableNum = 0;
	selectedGoods = -1;

	// 초기 Scene
	drawTable();

	// DB에서 입력 받기
	_total_sales_ = DB::getInstance().getTotalSales();
	_cash_sales_ = DB::getInstance().getCashSales();
	_account_sales_ = DB::getInstance().getAccountSales();
}

void Pos::render()
{
	static Gdiplus::RectF screenRect(0, 0, WIDTH, HEIGHT);
	bufGraphics->FillRectangle(whiteBrush, 0, 0, WIDTH, HEIGHT);
	for (int i = 0; i < sprite.size(); i++)
	{
		Gdiplus::RectF rect = Gdiplus::RectF(sprite[i]->x, sprite[i]->y, sprite[i]->width, sprite[i]->height);
		if (sprite[i]->type & 1)
			bufGraphics->DrawImage(sprite[i]->spriteSet, rect, 0, 0, sprite[i]->spriteSet->GetWidth(), sprite[i]->spriteSet->GetHeight(), Gdiplus::Unit::UnitPixel);
		if (sprite[i]->type & 2)
			bufGraphics->DrawString(sprite[i]->text.c_str(), -1, sprite[i]->font, rect, sprite[i]->textFormat, sprite[i]->textBrush);
		if (sprite[i]->type & 8)
			bufGraphics->FillRectangle(sprite[i]->rectBrush, rect);
		if (sprite[i]->type & 4)
			bufGraphics->DrawRectangle(sprite[i]->rectPen, rect);
	}
	if (inputTarget != nullptr)
	{
		Gdiplus::Pen *pen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 255), 2.0);
		Gdiplus::RectF rect = Gdiplus::RectF(inputTarget->x, inputTarget->y, inputTarget->width, inputTarget->height);
		bufGraphics->DrawRectangle(pen, rect);
	}
	outGraphics->DrawImage(bitmap, screenRect, 0, 0, WIDTH, HEIGHT, Gdiplus::UnitPixel);
}

void Pos::update()
{

	if (GetFocus() != _hWnd_)
		return;
	bool clicked = false;
	bool point = false;
	POINT mouse;

	GetCursorPos(&mouse);
	ScreenToClient(_hWnd_, &mouse);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
		clicked = true;
	for (int i = 0; i < interaction.size(); i++)
	{
		// Scene 전환 후에도 루프가 계속 도는 문제를 해결하기 위해서 changeSceneFlag 를 만들어 두었음.
		if (changeScene)
			break;
		RECT rect;
		rect.left = interaction[i]->x;
		rect.top = interaction[i]->y;
		rect.right = interaction[i]->x + interaction[i]->width;
		rect.bottom = interaction[i]->y + interaction[i]->height;
		if (PtInRect(&rect, mouse))
		{
			point = true;
			if (clicked)
				interaction[i]->func->execute();
		}
	}
	if (point)
		SetCursor(LoadCursor(NULL, IDC_HAND));
	else
		SetCursor(LoadCursor(NULL, IDC_ARROW));

	if (changeScene)
		changeScene = false;
}

void Pos::keydown(WCHAR vk)
{
	if (inputTarget != nullptr)
	{
		if (vk == VK_BACK)
		{
			if (inputBox[inputTarget].length() > 0)
				inputBox[inputTarget].pop_back();
		}
		else
		{
			inputBox[inputTarget].push_back(vk);
		}
		inputTarget->setText(inputBox[inputTarget]);
	}
}


void Pos::clearSprite()
{
	for (auto& ent : sprite)
	{
		delete ent;
	}
	sprite.clear();
}

void Pos::clearInteraction()
{
	for (auto& ent : interaction)
	{
		delete ent;
	}
	interaction.clear();
	changeScene = true;
}

Interactive* Pos::createInteraction(int x, int y, int width, int height, Function *func)
{
	Interactive* i = new Interactive(x, y, width, height, func);
	interaction.push_back(i);
	return i;
}

Interactive * Pos::createInteraction(Graphical * g, Function * func)
{
	Interactive* i = new Interactive(g, func);
	interaction.push_back(i);
	return i;
}

Graphical* Pos::createSprite(int x, int y, int width, int height, std::wstring path)
{
	Graphical* g = new Graphical(x, y, width, height, new Gdiplus::Image(path.c_str()));
	sprite.push_back(g);
	return g;
}

Graphical* Pos::createText(int x, int y, int width, int height, std::wstring text)
{
	Graphical* g = new Graphical(x, y, width, height, text);
	sprite.push_back(g);
	return g;
}

Graphical* Pos::createText(int x, int y, int width, int height, std::wstring text, int size)
{
	Graphical* g = new Graphical(x, y, width, height, text, size);
	sprite.push_back(g);
	return g;
}

Graphical* Pos::createRect(int x, int y, int width, int height, Gdiplus::Color color, Gdiplus::REAL thickness)
{
	Graphical *g = new Graphical(x, y, width, height, color, thickness);
	sprite.push_back(g);
	return g;
}


void Pos::drawTable()
{
	clearSprite();
	clearInteraction();
	Graphical *title = createText(0, 15, WIDTH, HEIGHT, DB::getInstance().getTitle(), 40);
	title->textFormat->SetAlignment(Gdiplus::StringAlignmentCenter);

	for (int i = 0; i < TABLE_MAX; i++)
	{
		Table *t = DB::getInstance().getTable(i);
		std::wstring content;
		WCHAR tableName[255];
		wsprintf(tableName, TEXT("      %d번 테이블\n"), i + 1);
		content.append(tableName);
		for (int j = 0; j < DB::getInstance().getGoodsMax(); j++)
		{
			if (t->getOrderNumber(j) > 0)
			{
				WCHAR str[255];
				wsprintf(str, TEXT("%s  %d\n"), DB::getInstance().getGoodsName(j).c_str(), t->getOrderNumber(j));
				content.append(str);
			}
		}
		Graphical *tableButton = createText(50 + i % 5 * 150, 100 + i / 5 * 150, 150, 150, content, 15);
		tableButton->drawRectangle(Gdiplus::Color(0, 0, 0), 3.0);
		// 누를 때 주문화면로 넘어가야하므로
		createInteraction(tableButton, new Function(&Pos::tableToOrder, this, i));
	}

	// 매출확인 버튼
	Graphical *checkSalesButton = createText(850, 100, 100, 60, L"매출확인");
	checkSalesButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	checkSalesButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	checkSalesButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *checkSales = createInteraction(checkSalesButton, new Function(&Pos::drawCheckSalesWindow, this));

	// 제작자
	Graphical *checkCreditButton = createText(850, 200, 100, 60, L"제작자");
	checkCreditButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	checkCreditButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	checkCreditButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *checkCredit = createInteraction(checkCreditButton, new Function(&Pos::drawCreditWindow, this));

}

void Pos::tableToOrder(int id)
{
	tableNum = id;
	drawOrderTable();
}

void Pos::drawCheckSalesWindow()
{
	// 창 뒤의 Interaction들 모두 비활성화
	clearInteraction();
	// Payment Window Frame
	Graphical *window = createRect(WIDTH / 2 - 185, HEIGHT / 2 - 200, 370, 200, Gdiplus::Color(0, 0, 0), 2.0);
	window->fillRectangle(Gdiplus::Color(255, 255, 255));

	// 안내 문구
	WCHAR notice[255];
	wsprintf(notice, TEXT("현재 총 매출: \\%d\n현금결제: \\%d\n계좌이체: \\%d"), _total_sales_,_cash_sales_,_account_sales_);
	Graphical *text = createText(WIDTH / 2 - 170, HEIGHT / 2 - 170, 370, 170, notice);

	// 확인 버튼
	Graphical *cashPayButton = createText(WIDTH / 2 - 50, HEIGHT / 2 - 70, 100, 60, L"확인");
	cashPayButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	cashPayButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	cashPayButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *cashPay = createInteraction(cashPayButton, new Function(&Pos::drawTable, this));
}

void Pos::drawCreditWindow()
{
	// 창 뒤의 Interaction들 모두 비활성화
	clearInteraction();
	// Payment Window Frame
	Graphical *window = createRect(WIDTH / 2 - 185, HEIGHT / 2 - 200, 370, 200, Gdiplus::Color(0, 0, 0), 2.0);
	window->fillRectangle(Gdiplus::Color(255, 255, 255));
	// 안내 문구
	std::wstring credit = L"컴퓨터공학과 14학번 정해갑\n전자공학과 16학번 위성은\n컴퓨터공학과 18학번 연동현";
	Graphical *text = createText(WIDTH / 2 - 170, HEIGHT / 2 - 170, 370, 170, credit);

	// 확인 버튼
	Graphical *cashPayButton = createText(WIDTH / 2 - 50, HEIGHT / 2 - 70, 100, 60, L"확인");
	cashPayButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	cashPayButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	cashPayButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *cashPay = createInteraction(cashPayButton, new Function(&Pos::drawTable, this));
}


void Pos::orderGoods(int id)
{
	Table *t = DB::getInstance().getTable(tableNum);
	if (t->getState() == 0)
		t->changeState(2);
	t->order(id, 1);
}

void Pos::selectGoods(int i)
{
	if (selectedGoods != i)
		selectedGoods = i;
	else
		selectedGoods = -1;
	drawOrderTable();
}

void Pos::cancelGoods()
{
	if (selectedGoods != -1)
	{
		Table *t = DB::getInstance().getTable(tableNum);
		t->cancel(selectedGoods);
		drawOrderTable();
	}
}

void Pos::payment(int payType) //테이블에서 계산된 값을 총매출에 더함
{
	Table* t = DB::getInstance().getTable(tableNum);
	WCHAR str[255];
	int bill = t->payDone();
	if (payType == 0)
	{
		wsprintf(str, TEXT("%d번 테이블 %d원 현금결제"), tableNum + 1, bill);
		_cash_sales_ += bill;
	}
	else if (payType == 1)
	{
		wsprintf(str, TEXT("%d번 테이블 %d원 계좌이체"), tableNum + 1, bill);
		_account_sales_ += bill;
	}
	_total_sales_ += bill;
	DB::getInstance().setSales(_total_sales_, _cash_sales_, _account_sales_);
	DB::getInstance().logging(str);
	drawTable();
}

void Pos::drawPaymentWindow()
{
	// 창 뒤의 Interaction들 모두 비활성화
	clearInteraction();
	Table *t = DB::getInstance().getTable(tableNum);
	if (t->getState() == 0)
	{
		drawOrderTable();
		return;
	}
	// Payment Window Frame
	Graphical *window = createRect(WIDTH / 2 - 185, HEIGHT / 2 - 200, 370, 200, Gdiplus::Color(0, 0, 0), 2.0);
	window->fillRectangle(Gdiplus::Color(255, 255, 255));
	// 취소 버튼
	Graphical *cancelButton = createText(WIDTH / 2 + 185 - 30, HEIGHT / 2 - 200, 30, 30, L"X");
	cancelButton->setTextColor(Gdiplus::Color(255, 0, 0));
	cancelButton->textFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
	cancelButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	cancelButton->drawRectangle(Gdiplus::Color(255, 0, 0), 2.0);
	Interactive *cancel = createInteraction(cancelButton, new Function(&Pos::drawOrderTable, this));

	// 안내 문구
	WCHAR notice[255];
	wsprintf(notice, TEXT("결제 방식을 선택해주세요\n(\\%d)"), t->getTotalPrice());
	Graphical *text = createText(WIDTH / 2 - 185, HEIGHT / 2 - 170, 370, 170, notice);
	text->setTextAlign(Gdiplus::StringAlignmentCenter);

	// 현금결제
	Graphical *cashPayButton = createText(WIDTH / 2 - 115, HEIGHT / 2 - 100, 100, 60, L"현금결제");
	cashPayButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	cashPayButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	cashPayButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *cashPay = createInteraction(cashPayButton, new Function(&Pos::payment, this, 0));

	// 계좌이체
	Graphical *accountPayButton = createText(WIDTH / 2 + 15, HEIGHT / 2 - 100, 100, 60, L"계좌이체");
	accountPayButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	accountPayButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	accountPayButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *accountPay = createInteraction(accountPayButton, new Function(&Pos::payment, this, 1));
}

void Pos::drawOrderTable()
{
	clearSprite();
	clearInteraction();
	WCHAR titleStr[255];
	wsprintf(titleStr, TEXT("%d번 테이블"), tableNum+1);
	Graphical *title = createText(50, 15, 500, 100, titleStr, 40);
	// Ordering Menu Frame
	createRect(50, 100, 500, 500, Gdiplus::Color(0, 0, 0), 3.0);
	createText(65, 115, 500, 50, L"메뉴                      가격         개수       합계");
	createText(360, 550, 200, 30, L"TOTAL");

	// Draw Goods Button
	for (int i = 0; i < DB::getInstance().getGoodsMax(); i++)
	{
		std::wstring name = DB::getInstance().getGoodsName(i);
		WCHAR price[255];
		wsprintf(price, TEXT("%d"), DB::getInstance().getGoodsPrice(i));
		Graphical *menuButton = createText(650 + i / 5 * 100, 100 + i % 5 * 100, 100, 100, L"\n\n" + name + L'\n' + price, 15);
		menuButton->drawRectangle(Gdiplus::Color(0, 0, 0), 2.0);
		menuButton->setTextAlign(Gdiplus::StringAlignmentCenter);
		// 누를 때마다 주문 입력되도록,
		Interactive* order = createInteraction(menuButton, new Function(&Pos::orderGoods, this, i));
		// 누를 때마다 주문 테이블을 다시 그려줘야하므로,
		Interactive* draw = createInteraction(menuButton, new Function(&Pos::drawOrderTable, this));
	}

	// Draw Table Content (interactive content)
	int contentCount = 0;
	Table *t = DB::getInstance().getTable(tableNum);
	for (int i = 0; i < DB::getInstance().getGoodsMax(); i++)
	{
		if (t->getOrderNumber(i) == 0)
			continue;
		std::wstring nameStr= DB::getInstance().getGoodsName(i);
		std::wstring eachStr;
		std::wstring qtyStr;
		std::wstring eachTotalStr;
		WCHAR num[255], price[255], totalPrice[255];
		wsprintf(price, TEXT("%d"), DB::getInstance().getGoodsPrice(i));
		wsprintf(num, TEXT("%d"), t->getOrderNumber(i));
		wsprintf(totalPrice, TEXT("%d"), DB::getInstance().getGoodsPrice(i)*t->getOrderNumber(i));
		eachStr.append(price);
		qtyStr.append(num);
		eachTotalStr.append(totalPrice);
		Graphical *orderTableContent = createText(65, 160 + contentCount * 35, 470, 30, L"");
		if (i == selectedGoods)
			orderTableContent->drawRectangle(Gdiplus::Color(rand() % 250, rand() % 250, rand() % 250), 3.0f);
		Graphical *orderTableContentName = createText(65, 160 + contentCount * 35, 195, 30, nameStr);
		Graphical *orderTableContentEach = createText(260, 160 + contentCount * 35, 100, 30, eachStr);
		Graphical *orderTableContentQty = createText(360, 160 + contentCount * 35, 90, 30, qtyStr);
		Graphical *orderTableContentTotal = createText(450, 160 + contentCount * 35, 100, 30, eachTotalStr);
		Interactive *orderTableInteraction = createInteraction(orderTableContent, new Function(&Pos::selectGoods, this, i));
		contentCount++;
	}
	// 모든 상품을 합한 총액 표 아래 표시
	WCHAR totalPrice[255];
	wsprintf(totalPrice, TEXT("%d"), t->getTotalPrice());
	Graphical *orderTotal = createText(450, 550, 100, 50, totalPrice);

	// 주문 취소 버튼
	Graphical *cancelButton = createText(65, 540, 130, 50, L"선택취소");
	cancelButton->textFormat->SetAlignment(Gdiplus::StringAlignmentCenter);
	cancelButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	cancelButton->drawRectangle(Gdiplus::Color(200, 80, 80), 3.0f);
	Interactive *cancel = createInteraction(cancelButton, new Function(&Pos::cancelGoods, this));

	// 결제 버튼
	Graphical *payButton = createText(50, 630, 100, 60, L"결제");
	payButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	payButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	payButton->drawRectangle(Gdiplus::Color(0, 60, 233), 3.0f);
	Interactive *pay = createInteraction(payButton, new Function(&Pos::drawPaymentWindow, this));

	// 돌아가기 버튼
	Graphical *backButton = createText(450, 630, 100, 60, L"돌아가기");
	backButton->setTextAlign(Gdiplus::StringAlignmentCenter);
	backButton->textFormat->SetLineAlignment(Gdiplus::StringAlignmentCenter);
	backButton->drawRectangle(Gdiplus::Color(60, 233, 60), 3.0f);
	Interactive *back = createInteraction(backButton, new Function(&Pos::drawTable, this));
}
