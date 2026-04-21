#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include "burger.h"

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


#define ID_CHEESE 1
#define ID_BACON 2
#define ID_SAUCE 3
#define ID_DOUBLE 4
#define ID_ONION 5

HWND hCheese, hBacon, hSauce, hDouble, hOnion, hResult;
HWND hPanel;

ULONG_PTR gdiplusToken;


std::wstring baseBurgerPath = L"C:\\Users\\ilchu\\source\\repos\\LAB2_without_patt\\LAB2_without_patt\\burger_base.jpg";
std::wstring cheesePath = L"C:\\Users\\ilchu\\source\\repos\\LAB2_without_patt\\LAB2_without_patt\\cheese.jpg";
std::wstring baconPath = L"C:\\Users\\ilchu\\source\\repos\\LAB2_without_patt\\LAB2_without_patt\\bacon.jpg";
std::wstring saucePath = L"C:\\Users\\ilchu\\source\\repos\\LAB2_without_patt\\LAB2_without_patt\\sauce.jpg";
std::wstring onionPath = L"C:\\Users\\ilchu\\source\\repos\\LAB2_without_patt\\LAB2_without_patt\\onion.jpg";
std::wstring doublePath = L"C:\\Users\\ilchu\\source\\repos\\LAB2_without_patt\\LAB2_without_patt\\double_patty.jpg";


Image* imgBase = nullptr;
Image* imgCheese = nullptr;
Image* imgBacon = nullptr;
Image* imgSauce = nullptr;
Image* imgOnion = nullptr;
Image* imgDouble = nullptr;


bool selCheese = false;
bool selBacon = false;
bool selSauce = false;
bool selOnion = false;
bool selDouble = false;


void DrawIngredients(HDC hdc) {
    Graphics graphics(hdc);


    RECT rect;
    GetClientRect(hPanel, &rect);
    SolidBrush bgBrush(Color(255, 240, 240, 240));
    RectF rectF(
        static_cast<REAL>(rect.left),
        static_cast<REAL>(rect.top),
        static_cast<REAL>(rect.right - rect.left),
        static_cast<REAL>(rect.bottom - rect.top)
    );
    graphics.FillRectangle(&bgBrush, rectF);

    int panelWidth = 550;
    int panelHeight = 250;

    int burgerW = 200;
    int burgerH = 200;
    int burgerX = (panelWidth - burgerW) / 2;
    int burgerY = (panelHeight - burgerH) / 2;

    if (imgBase) graphics.DrawImage(imgBase, burgerX, burgerY, burgerW, burgerH);

    int ingW = 48;
    int ingH = 48;
    int spacing = 10;

    std::vector<Image*> leftItems;
    std::vector<Image*> rightItems;

    if (selCheese && imgCheese) leftItems.push_back(imgCheese);
    if (selBacon && imgBacon)  leftItems.push_back(imgBacon);
    if (selSauce && imgSauce)  rightItems.push_back(imgSauce);
    if (selDouble && imgDouble) rightItems.push_back(imgDouble);
    if (selOnion && imgOnion)  rightItems.push_back(imgOnion);


    int leftX = burgerX - ingW - spacing;
    int y = burgerY;
    for (auto img : leftItems) {
        graphics.DrawImage(img, leftX, y, ingW, ingH);
        y += ingH + spacing;
    }


    int rightX = burgerX + burgerW + spacing;
    y = burgerY;
    for (auto img : rightItems) {
        graphics.DrawImage(img, rightX, y, ingW, ingH);
        y += ingH + spacing;
    }
}


LRESULT CALLBACK PanelProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        DrawIngredients(hdc);
        EndPaint(hwnd, &ps);
    }
    return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        imgBase = new Image(baseBurgerPath.c_str());
        imgCheese = new Image(cheesePath.c_str());
        imgBacon = new Image(baconPath.c_str());
        imgSauce = new Image(saucePath.c_str());
        imgOnion = new Image(onionPath.c_str());
        imgDouble = new Image(doublePath.c_str());

        // Создаем панель
        hPanel = CreateWindowEx(0, TEXT("STATIC"), NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 550, 250, hwnd, NULL, NULL, NULL);
        SetWindowLongPtr(hPanel, GWLP_WNDPROC, (LONG_PTR)PanelProc);

        // Чекбоксы
        hCheese = CreateWindow(TEXT("BUTTON"), TEXT("Cheese"),
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 260, 150, 25, hwnd, (HMENU)ID_CHEESE, NULL, NULL);
        hBacon = CreateWindow(TEXT("BUTTON"), TEXT("Bacon"),
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 290, 150, 25, hwnd, (HMENU)ID_BACON, NULL, NULL);
        hSauce = CreateWindow(TEXT("BUTTON"), TEXT("Sauce"),
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 320, 150, 25, hwnd, (HMENU)ID_SAUCE, NULL, NULL);
        hDouble = CreateWindow(TEXT("BUTTON"), TEXT("Double Patty"),
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 350, 150, 25, hwnd, (HMENU)ID_DOUBLE, NULL, NULL);
        hOnion = CreateWindow(TEXT("BUTTON"), TEXT("Onion"),
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
            20, 380, 150, 25, hwnd, (HMENU)ID_ONION, NULL, NULL);

        // Результат
        hResult = CreateWindow(TEXT("STATIC"), TEXT("Result:"),
            WS_VISIBLE | WS_CHILD,
            200, 260, 350, 150, hwnd, NULL, NULL, NULL);
        break;

    case WM_COMMAND:
        selCheese = SendMessage(hCheese, BM_GETCHECK, 0, 0) == BST_CHECKED;
        selBacon = SendMessage(hBacon, BM_GETCHECK, 0, 0) == BST_CHECKED;
        selSauce = SendMessage(hSauce, BM_GETCHECK, 0, 0) == BST_CHECKED;
        selOnion = SendMessage(hOnion, BM_GETCHECK, 0, 0) == BST_CHECKED;
        selDouble = SendMessage(hDouble, BM_GETCHECK, 0, 0) == BST_CHECKED;


        {
            Burger burger(selCheese, selBacon, selSauce, selDouble, selOnion);
            std::string text = burger.getDescription() +
                "\nPrice: $" + std::to_string(burger.getPrice());
            std::wstring wtext(text.begin(), text.end());
            SetWindowText(hResult, wtext.c_str());
        }

        InvalidateRect(hPanel, NULL, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, TEXT("Burger Constructor"),
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 450,
        NULL, NULL, hInstance, NULL);

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