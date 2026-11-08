#include <windows.h>
#include <commdlg.h>
#include <cstdint>
#include <cstdio>
#include <string>
#include "SimulatorEngine.h"

namespace {

SimulatorEngine* engine = nullptr;
std::string statusMsg = "Load a program to begin";
int hoverBtn = -1;
HFONT fTitle, fText, fBold, fMono, fSmall;

const COLORREF cBg = RGB(17, 19, 30);
const COLORREF cCard = RGB(29, 32, 49);
const COLORREF cCell = RGB(40, 44, 66);
const COLORREF cAccent = RGB(99, 102, 241);
const COLORREF cAccentHot = RGB(129, 140, 248);
const COLORREF cGreen = RGB(52, 211, 153);
const COLORREF cRed = RGB(248, 113, 113);
const COLORREF cText = RGB(232, 234, 246);
const COLORREF cDim = RGB(120, 126, 156);

struct Btn {
    int x, y, w, h;
    const char* label;
    COLORREF color;
};

Btn btns[4] = {
    {24, 76, 150, 42, "Load Program", RGB(99, 102, 241)},
    {186, 76, 150, 42, "Step", RGB(14, 165, 233)},
    {348, 76, 150, 42, "Run All", RGB(16, 185, 129)},
    {510, 76, 150, 42, "Reset", RGB(239, 68, 68)}
};

void box(HDC dc, int x, int y, int w, int h, COLORREF c, int r) {
    HBRUSH b = CreateSolidBrush(c);
    HPEN p = CreatePen(PS_NULL, 0, 0);
    HGDIOBJ ob = SelectObject(dc, b);
    HGDIOBJ op = SelectObject(dc, p);
    if (r > 0) RoundRect(dc, x, y, x + w + 1, y + h + 1, r, r);
    else Rectangle(dc, x, y, x + w + 1, y + h + 1);
    SelectObject(dc, ob);
    SelectObject(dc, op);
    DeleteObject(b);
    DeleteObject(p);
}

void label(HDC dc, const char* s, int x, int y, int w, int h, COLORREF c, HFONT f, UINT fmt) {
    SelectObject(dc, f);
    SetTextColor(dc, c);
    RECT r = {x, y, x + w, y + h};
    DrawTextA(dc, s, -1, &r, fmt | DT_SINGLELINE | DT_NOPREFIX);
}

void drawAll(HDC dc, int W, int H) {
    box(dc, 0, 0, W, H, cBg, 0);
    SetBkMode(dc, TRANSPARENT);

    label(dc, "Vole Machine Simulator", 24, 14, 600, 40, cText, fTitle, DT_LEFT | DT_VCENTER);
    label(dc, "8-bit educational CPU", 24, 50, 600, 20, cDim, fSmall, DT_LEFT | DT_VCENTER);

    for (int i = 0; i < 4; ++i) {
        COLORREF c = btns[i].color;
        if (i == hoverBtn) c = RGB(GetRValue(c) + (255 - GetRValue(c)) / 5,
                                   GetGValue(c) + (255 - GetGValue(c)) / 5,
                                   GetBValue(c) + (255 - GetBValue(c)) / 5);
        box(dc, btns[i].x, btns[i].y, btns[i].w, btns[i].h, c, 12);
        label(dc, btns[i].label, btns[i].x, btns[i].y, btns[i].w, btns[i].h, RGB(255, 255, 255), fBold,
              DT_CENTER | DT_VCENTER);
    }

    int pc = static_cast<int>(engine->getCPU().getPC());
    int ir = static_cast<int>(engine->getCPU().getIR().getRawWord());
    bool halted = engine->getCPU().isHalted();
    char buf[64];

    box(dc, 24, 138, 330, 130, cCard, 16);
    label(dc, "CPU STATE", 44, 150, 200, 20, cDim, fSmall, DT_LEFT | DT_VCENTER);
    label(dc, "PC", 44, 178, 60, 24, cDim, fText, DT_LEFT | DT_VCENTER);
    snprintf(buf, sizeof(buf), "0x%02X", pc & 0xFF);
    label(dc, buf, 110, 178, 100, 24, cText, fMono, DT_LEFT | DT_VCENTER);
    label(dc, "IR", 44, 206, 60, 24, cDim, fText, DT_LEFT | DT_VCENTER);
    snprintf(buf, sizeof(buf), "0x%04X", ir & 0xFFFF);
    label(dc, buf, 110, 206, 100, 24, cText, fMono, DT_LEFT | DT_VCENTER);
    box(dc, 44, 236, 130, 24, halted ? RGB(80, 40, 48) : RGB(24, 70, 58), 12);
    label(dc, halted ? "HALTED" : "RUNNING", 44, 236, 130, 24, halted ? cRed : cGreen, fBold,
          DT_CENTER | DT_VCENTER);

    box(dc, 24, 284, 330, 300, cCard, 16);
    label(dc, "REGISTERS", 44, 296, 200, 20, cDim, fSmall, DT_LEFT | DT_VCENTER);
    for (int i = 0; i < 16; ++i) {
        int col = i % 4, row = i / 4;
        int x = 40 + col * 76, y = 326 + row * 62;
        box(dc, x, y, 70, 54, cCell, 10);
        snprintf(buf, sizeof(buf), "R%X", i);
        label(dc, buf, x, y + 5, 70, 16, cDim, fSmall, DT_CENTER | DT_VCENTER);
        int v = static_cast<int>(engine->getCPU().getRegisters().read(i)) & 0xFF;
        snprintf(buf, sizeof(buf), "%02X", v);
        label(dc, buf, x, y + 22, 70, 26, v ? cAccentHot : cText, fMono, DT_CENTER | DT_VCENTER);
    }

    box(dc, 374, 138, 602, 446, cCard, 16);
    label(dc, "MEMORY", 394, 150, 200, 20, cDim, fSmall, DT_LEFT | DT_VCENTER);
    for (int c = 0; c < 16; ++c) {
        snprintf(buf, sizeof(buf), "%X", c);
        label(dc, buf, 436 + c * 34, 172, 32, 18, cDim, fSmall, DT_CENTER | DT_VCENTER);
    }
    for (int r = 0; r < 16; ++r) {
        snprintf(buf, sizeof(buf), "%X0", r);
        label(dc, buf, 392, 194 + r * 24, 36, 22, cDim, fSmall, DT_CENTER | DT_VCENTER);
        for (int c = 0; c < 16; ++c) {
            int addr = r * 16 + c;
            int v = static_cast<int>(engine->getMemory().readByte(static_cast<uint16_t>(addr))) & 0xFF;
            bool atPC = (addr == pc || addr == pc + 1);
            int x = 436 + c * 34, y = 194 + r * 24;
            box(dc, x, y, 32, 22, atPC ? cAccent : cCell, 6);
            snprintf(buf, sizeof(buf), "%02X", v);
            label(dc, buf, x, y, 32, 22, atPC ? RGB(255, 255, 255) : (v ? cText : cDim), fMono,
                  DT_CENTER | DT_VCENTER);
        }
    }

    box(dc, 24, 600, 952, 40, cCard, 12);
    box(dc, 40, 616, 8, 8, halted ? cRed : cGreen, 8);
    label(dc, statusMsg.c_str(), 60, 600, 900, 40, cText, fText, DT_LEFT | DT_VCENTER);
}

int hit(int mx, int my) {
    for (int i = 0; i < 4; ++i)
        if (mx >= btns[i].x && mx < btns[i].x + btns[i].w && my >= btns[i].y && my < btns[i].y + btns[i].h)
            return i;
    return -1;
}

void doLoad(HWND hw) {
    char path[MAX_PATH] = "";
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hw;
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameA(&ofn)) {
        if (engine->loadProgramFromFile(path)) {
            std::string p = path;
            size_t s = p.find_last_of("\\/");
            statusMsg = "Loaded: " + (s == std::string::npos ? p : p.substr(s + 1));
        } else {
            statusMsg = "Failed to load program";
        }
    }
}

void action(HWND hw, int id) {
    if (id == 0) {
        doLoad(hw);
    } else if (id == 1) {
        if (engine->getCPU().isHalted()) {
            statusMsg = "CPU is halted. Press Reset to start over";
        } else if (engine->runStep()) {
            statusMsg = "Executed one step";
        } else {
            statusMsg = "Cannot execute: program ended or invalid instruction";
        }
    } else if (id == 2) {
        engine->runAll();
        statusMsg = engine->getCPU().isHalted() ? "Program finished (HALT)" : "Run stopped";
    } else if (id == 3) {
        engine->reset();
        statusMsg = "Machine reset";
    }
    InvalidateRect(hw, nullptr, FALSE);
}

LRESULT CALLBACK proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        engine = new SimulatorEngine();
        fTitle = CreateFontA(-30, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
        fText = CreateFontA(-17, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
        fBold = CreateFontA(-17, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
        fSmall = CreateFontA(-13, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Segoe UI");
        fMono = CreateFontA(-16, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, CLEARTYPE_QUALITY, 0, "Consolas");
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hw, &ps);
        RECT cr;
        GetClientRect(hw, &cr);
        HDC mem = CreateCompatibleDC(hdc);
        HBITMAP bmp = CreateCompatibleBitmap(hdc, cr.right, cr.bottom);
        HGDIOBJ old = SelectObject(mem, bmp);
        drawAll(mem, cr.right, cr.bottom);
        BitBlt(hdc, 0, 0, cr.right, cr.bottom, mem, 0, 0, SRCCOPY);
        SelectObject(mem, old);
        DeleteObject(bmp);
        DeleteDC(mem);
        EndPaint(hw, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    case WM_MOUSEMOVE: {
        int h = hit(LOWORD(lp), HIWORD(lp));
        if (h != hoverBtn) {
            hoverBtn = h;
            SetCursor(LoadCursor(nullptr, h >= 0 ? IDC_HAND : IDC_ARROW));
            InvalidateRect(hw, nullptr, FALSE);
        }
        return 0;
    }
    case WM_LBUTTONDOWN: {
        int h = hit(LOWORD(lp), HIWORD(lp));
        if (h >= 0) action(hw, h);
        return 0;
    }
    case WM_DESTROY:
        delete engine;
        engine = nullptr;
        DeleteObject(fTitle);
        DeleteObject(fText);
        DeleteObject(fBold);
        DeleteObject(fSmall);
        DeleteObject(fMono);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hw, msg, wp, lp);
}

}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE, LPSTR, int show) {
    WNDCLASSA wc = {};
    wc.lpfnWndProc = proc;
    wc.hInstance = inst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = "VoleSimWindow";
    RegisterClassA(&wc);

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    RECT r = {0, 0, 1000, 664};
    AdjustWindowRect(&r, style, FALSE);
    HWND hw = CreateWindowA("VoleSimWindow", "Vole Machine Simulator", style, CW_USEDEFAULT, CW_USEDEFAULT,
                            r.right - r.left, r.bottom - r.top, nullptr, nullptr, inst, nullptr);
    ShowWindow(hw, show);
    UpdateWindow(hw);

    MSG m;
    while (GetMessageA(&m, nullptr, 0, 0) > 0) {
        TranslateMessage(&m);
        DispatchMessageA(&m);
    }
    return static_cast<int>(m.wParam);
}