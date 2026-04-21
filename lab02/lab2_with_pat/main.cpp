#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include "burger.h"

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#define ID_CHEESE_ADD 1
#define ID_CHEESE_REM 2
#define ID_BACON_ADD 3
#define ID_BACON_REM 4
#define ID_SAUCE_ADD 5
#define ID_SAUCE_REM 6
#define ID_DOUBLE_ADD 7
#define ID_DOUBLE_REM 8
#define ID_ONION_ADD 9
#define ID_ONION_REM 10
#define ID_CONFIRM 11

HWND hPanel, hResult;
HWND buttons[12];
ULONG_PTR gdiplusToken;


std::wstring baseBurgerPath = L"burger_base.jpg";
std::wstring cheesePath = L"cheese.jpg";
std::wstring baconPath = L"bacon.jpg";
std::wstring saucePath = L"sauce.jpg";
std::wstring onionPath = L"onion.jpg";
std::wstring doublePath = L"double_patty.jpg";

Image* imgBase = nullptr, * imgCheese = nullptr, * imgBacon = nullptr,
* imgSauce = nullptr, * imgOnion = nullptr, * imgDouble = nullptr;


std::map<int, int> ingredientCount = {
    {ID_CHEESE_ADD,0},
    {ID_BACON_ADD,0},
    {ID_SAUCE_ADD,0},
    {ID_ONION_ADD,0},
    {ID_DOUBLE_ADD,0}
};

Image* GetIngredientImage(int id) {
    switch (id) {
    case ID_CHEESE_ADD: return imgCheese;
    case ID_BACON_ADD: return imgBacon;
    case ID_SAUCE_ADD: return imgSauce;
    case ID_ONION_ADD: return imgOnion;
    case ID_DOUBLE_ADD: return imgDouble;
    default: return nullptr;
    }
}



void DrawIngredients(HDC hdc) {
    Graphics g(hdc);

    SolidBrush bg(Color(255, 240, 240, 240));
    g.FillRectangle(&bg, 0, 0, 550, 250);

    int burgerW = 200, burgerH = 200;
    int burgerX = (550 - burgerW) / 2;
    int burgerY = (250 - burgerH) / 2;

    if (imgBase)
        g.DrawImage(imgBase, burgerX, burgerY, burgerW, burgerH);

    int ingW = 48, ingH = 48, spacing = 10;

    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 12, FontStyleBold, UnitPixel);
    SolidBrush brush(Color(255, 0, 0, 0));

    int y = burgerY;
    int leftX = burgerX - ingW - spacing;

    for (int id : {ID_CHEESE_ADD, ID_BACON_ADD}) {
        if (ingredientCount[id] > 0 && GetIngredientImage(id)) {
            g.DrawImage(GetIngredientImage(id), leftX, y, ingW, ingH);

            std::wstring txt = L"x" + std::to_wstring(ingredientCount[id]);
            g.DrawString(txt.c_str(), -1, &font,
                PointF((REAL)(leftX + ingW + 2), (REAL)y), &brush);

            y += ingH + spacing;
        }
    }


    y = burgerY;
    int rightX = burgerX + burgerW + spacing;

    for (int id : {ID_SAUCE_ADD, ID_DOUBLE_ADD, ID_ONION_ADD}) {
        if (ingredientCount[id] > 0 && GetIngredientImage(id)) {
            g.DrawImage(GetIngredientImage(id), rightX, y, ingW, ingH);

            std::wstring txt = L"x" + std::to_wstring(ingredientCount[id]);
            g.DrawString(txt.c_str(), -1, &font,
                PointF((REAL)(rightX + ingW + 2), (REAL)y), &brush);

            y += ingH + spacing;
        }
    }
}


LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawIngredients(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


void UpdateResult() {
    IBurger* burger = new BaseBurger();

    for (int i = 0; i < ingredientCount[ID_CHEESE_ADD]; i++)
        burger = new CheeseDecorator(burger);

    for (int i = 0; i < ingredientCount[ID_BACON_ADD]; i++)
        burger = new BaconDecorator(burger);

    for (int i = 0; i < ingredientCount[ID_SAUCE_ADD]; i++)
        burger = new SauceDecorator(burger);

    for (int i = 0; i < ingredientCount[ID_ONION_ADD]; i++)
        burger = new OnionDecorator(burger);

    for (int i = 0; i < ingredientCount[ID_DOUBLE_ADD]; i++)
        burger = new DoublePattyDecorator(burger);

    std::ostringstream oss;
    oss << burger->getDescription()
        << "\nPrice: $"
        << std::fixed << std::setprecision(2)
        << burger->getPrice();

    std::string text = oss.str();
    std::wstring wtext(text.begin(), text.end());

    SetWindowText(hResult, wtext.c_str());

    delete burger;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CREATE:
        imgBase = new Image(baseBurgerPath.c_str());
        imgCheese = new Image(cheesePath.c_str());
        imgBacon = new Image(baconPath.c_str());
        imgSauce = new Image(saucePath.c_str());
        imgOnion = new Image(onionPath.c_str());
        imgDouble = new Image(doublePath.c_str());

        hPanel = CreateWindowEx(0, TEXT("STATIC"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 550, 250, hwnd, NULL, NULL, NULL);

        SetWindowLongPtr(hPanel, GWLP_WNDPROC, (LONG_PTR)PanelProc);

        // Êíîïêè
        buttons[ID_CHEESE_ADD] = CreateWindow(TEXT("BUTTON"), TEXT("Add Cheese"), WS_CHILD | WS_VISIBLE, 20, 260, 100, 25, hwnd, (HMENU)ID_CHEESE_ADD, NULL, NULL);
        buttons[ID_CHEESE_REM] = CreateWindow(TEXT("BUTTON"), TEXT("Remove Cheese"), WS_CHILD | WS_VISIBLE, 130, 260, 120, 25, hwnd, (HMENU)ID_CHEESE_REM, NULL, NULL);

        buttons[ID_BACON_ADD] = CreateWindow(TEXT("BUTTON"), TEXT("Add Bacon"), WS_CHILD | WS_VISIBLE, 20, 290, 100, 25, hwnd, (HMENU)ID_BACON_ADD, NULL, NULL);
        buttons[ID_BACON_REM] = CreateWindow(TEXT("BUTTON"), TEXT("Remove Bacon"), WS_CHILD | WS_VISIBLE, 130, 290, 120, 25, hwnd, (HMENU)ID_BACON_REM, NULL, NULL);

        buttons[ID_SAUCE_ADD] = CreateWindow(TEXT("BUTTON"), TEXT("Add Sauce"), WS_CHILD | WS_VISIBLE, 20, 320, 100, 25, hwnd, (HMENU)ID_SAUCE_ADD, NULL, NULL);
        buttons[ID_SAUCE_REM] = CreateWindow(TEXT("BUTTON"), TEXT("Remove Sauce"), WS_CHILD | WS_VISIBLE, 130, 320, 120, 25, hwnd, (HMENU)ID_SAUCE_REM, NULL, NULL);

        buttons[ID_DOUBLE_ADD] = CreateWindow(TEXT("BUTTON"), TEXT("Add Double"), WS_CHILD | WS_VISIBLE, 20, 350, 100, 25, hwnd, (HMENU)ID_DOUBLE_ADD, NULL, NULL);
        buttons[ID_DOUBLE_REM] = CreateWindow(TEXT("BUTTON"), TEXT("Remove Double"), WS_CHILD | WS_VISIBLE, 130, 350, 120, 25, hwnd, (HMENU)ID_DOUBLE_REM, NULL, NULL);

        buttons[ID_ONION_ADD] = CreateWindow(TEXT("BUTTON"), TEXT("Add Onion"), WS_CHILD | WS_VISIBLE, 20, 380, 100, 25, hwnd, (HMENU)ID_ONION_ADD, NULL, NULL);
        buttons[ID_ONION_REM] = CreateWindow(TEXT("BUTTON"), TEXT("Remove Onion"), WS_CHILD | WS_VISIBLE, 130, 380, 120, 25, hwnd, (HMENU)ID_ONION_REM, NULL, NULL);


        buttons[ID_CONFIRM] = CreateWindow(TEXT("BUTTON"), TEXT("Confirm / Reset"),
            WS_CHILD | WS_VISIBLE,
            20, 420, 230, 30,
            hwnd, (HMENU)ID_CONFIRM, NULL, NULL);

        hResult = CreateWindow(TEXT("STATIC"), TEXT("Result:"),
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            270, 260, 300, 190,
            hwnd, NULL, NULL, NULL);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case ID_CHEESE_ADD: ingredientCount[ID_CHEESE_ADD]++; break;
        case ID_CHEESE_REM: if (ingredientCount[ID_CHEESE_ADD] > 0) ingredientCount[ID_CHEESE_ADD]--; break;

        case ID_BACON_ADD: ingredientCount[ID_BACON_ADD]++; break;
        case ID_BACON_REM: if (ingredientCount[ID_BACON_ADD] > 0) ingredientCount[ID_BACON_ADD]--; break;

        case ID_SAUCE_ADD: ingredientCount[ID_SAUCE_ADD]++; break;
        case ID_SAUCE_REM: if (ingredientCount[ID_SAUCE_ADD] > 0) ingredientCount[ID_SAUCE_ADD]--; break;

        case ID_DOUBLE_ADD: ingredientCount[ID_DOUBLE_ADD]++; break;
        case ID_DOUBLE_REM: if (ingredientCount[ID_DOUBLE_ADD] > 0) ingredientCount[ID_DOUBLE_ADD]--; break;

        case ID_ONION_ADD: ingredientCount[ID_ONION_ADD]++; break;
        case ID_ONION_REM: if (ingredientCount[ID_ONION_ADD] > 0) ingredientCount[ID_ONION_ADD]--; break;

        case ID_CONFIRM:
            for (auto& p : ingredientCount)
                p.second = 0;
            break;
        }

        UpdateResult();
        InvalidateRect(hPanel, NULL, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    const TCHAR CLASS_NAME[] = TEXT("BurgerWindow");

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, TEXT("Burger Constructor"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 620, 520,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    delete imgBase; delete imgCheese; delete imgBacon;
    delete imgSauce; delete imgOnion; delete imgDouble;

    GdiplusShutdown(gdiplusToken);
    return 0;
}
