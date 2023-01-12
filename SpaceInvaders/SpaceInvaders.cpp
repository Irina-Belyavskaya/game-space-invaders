#include "framework.h"
#include "SpaceInvaders.h"
#include <thread>
#include <vector>
#include <string>
#include "windowsx.h"

#include "GameObject.h"
#include "LevelsTypes.h"

#include "PlayerShip.h"
#include "PlayerGun.h"
#include "AlienShip.h"
#include "AlienGun.h"

#include "GameSettings.h"

using namespace std;

#define MAX_LOADSTRING 100
#define TIMER_1_ID 1
#define TIMER_2_ID 2
#define TIMER_3_ID 3

#define WINDOW_L 90
#define WINDOW_T 70
#define WINDOW_R 1300
#define WINDOW_B 700

#define BUTTON_ID_START 100
#define BUTTON_ID_RETURN 101

#define BUTTON_LEVEL_1 201
#define BUTTON_LEVEL_2 202
#define BUTTON_LEVEL_3 203
#define BUTTON_LEVEL_4 204

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE handleBuffer;
HDC hdcBuffer;
RECT windowSize;

HWND buttonHwnd;
HWND level_1_Hwnd;
HWND level_2_Hwnd;
HWND level_3_Hwnd;
HWND level_4_Hwnd;

PlayerShip playerShipObject;

vector<AlienShip> aliens;
vector<PlayerGun> playerGuns;
vector<AlienGun> alienGuns;

vector<HANDLE> threads;

static CRITICAL_SECTION csection;

bool is_UP_pressed = false;
bool is_DOWN_pressed = false;
bool is_LEFT_pressed = false;
bool is_RIGHT_pressed = false;

bool isOnce = false;
bool isEnableSpace = true;

bool isRectCaptured = false;
bool isGameOver = false;
bool isPlay = false;

POINT currCursorPos;
LevelsTypes level = LevelsTypes::Level_1;
chrono::duration<long long, milli> creationSpeed = 2000ms;
int movementTimerOfAlien = 300;
int movementTimerOfAlienGun = 100;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID InitializeBuffer(HWND, int, int);
VOID FinalizeBuffer();

VOID MoveGameObject(HWND, LPRECT, double, double, GameObjectType);
VOID DrawGame(HWND);
VOID DrawWindow(HWND, const wchar_t*,const wchar_t*);

VOID InitGame(HWND);
VOID StartGame(HWND);
VOID DeleteGameObjects(HWND);

VOID CreatePlayerShip();
HWND CreateButton(HWND, const wchar_t*, int, int, int, int, int);
VOID CreateLevelButtons(HWND);
VOID DeleteLevelButtons();

AlienShip CreateAlienShip();
PlayerGun CreatePlayerGun();
AlienGun CreateAlienGun(AlienShip);


VOID CheckBorders(RECT*, double*, double*);

DWORD WINAPI CreationOfAliens(LPVOID);
VOID CreateAliens();
DWORD WINAPI DeletionOfAlienGuns(LPVOID);
VOID DeleteOfAlienGuns();
DWORD WINAPI DeletionOfAliens(LPVOID);
VOID DeleteAliens();
DWORD WINAPI DeletionOfAliensByPlayerGuns(LPVOID);
VOID DeleteAliensByPlayerGuns();
VOID DisableSpace();
DWORD WINAPI Disable(LPVOID);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPACEINVADERS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPACEINVADERS));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPACEINVADERS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPACEINVADERS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case BUTTON_LEVEL_1:
                level = LevelsTypes::Level_1;
                StartGame(hWnd);
                break;
            case BUTTON_LEVEL_2:
                level = LevelsTypes::Level_2;
                StartGame(hWnd);
                break;
            case BUTTON_LEVEL_3:
                level = LevelsTypes::Level_3;
                StartGame(hWnd);
                break;
            case BUTTON_LEVEL_4:
                level = LevelsTypes::Level_4;
                StartGame(hWnd);
                break;
            case BUTTON_ID_RETURN:
                DestroyWindow(buttonHwnd);
                isPlay = false;
                isGameOver = false;
                CreateLevelButtons(hWnd);
                InvalidateRect(hWnd, NULL, FALSE);                
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        {
            GetClientRect(hWnd, &windowSize);
            MoveWindow(hWnd, WINDOW_L, WINDOW_T, WINDOW_R, WINDOW_B, NULL);
            
            GetClientRect(hWnd, &windowSize);
            InitializeBuffer(hWnd, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top);

            InvalidateRect(hWnd, NULL, FALSE);
            CreateLevelButtons(hWnd);
        }
        break;
    case WM_TIMER:
        {
            switch (wParam)
            {
            case TIMER_1_ID:
                EnterCriticalSection(&csection);
                for (int i = 0; i < aliens.size(); i++)
                        MoveGameObject(hWnd, &aliens[i].objectCoordinates, 0, 5, aliens[i].getObjectType());   
                LeaveCriticalSection(&csection);
                break;
            case TIMER_2_ID:
                EnterCriticalSection(&csection);
                for (int i = 0; i < playerGuns.size(); i++)
                        MoveGameObject(hWnd, &playerGuns[i].objectCoordinates, 0, -playerGuns[i].getSpeed(), playerGuns[i].getObjectType());
                LeaveCriticalSection(&csection);
                break;
            case TIMER_3_ID:
                EnterCriticalSection(&csection);
                for (int i = 0; i < alienGuns.size(); i++) {
                    MoveGameObject(hWnd, &alienGuns[i].objectCoordinates, 0, 5, alienGuns[i].getObjectType());                     
                }
                LeaveCriticalSection(&csection);
                break;
            }            
        }
        break;
    case WM_PAINT:
        {
            if (isPlay) {
                if (!isGameOver)
                    DrawGame(hWnd);
                else {                 
                    DrawWindow(hWnd, L"images/gameOver.bmp", L"images/gameOverMask.bmp");
                    WaitForMultipleObjects(threads.size(), threads.data(), TRUE, INFINITE);
                    DeleteGameObjects(hWnd);
                    if (!isOnce) {
                        buttonHwnd = CreateButton(hWnd, L"Go to main", BUTTON_ID_RETURN, 550, windowSize.bottom - 100, 150, 30);
                        isOnce = true;
                    }
                }
            }
            else {
                DrawWindow(hWnd, L"images/welcom.bmp", L"images/welcomMask.bmp");
            }
        }
        break;
    case WM_KEYDOWN:
        {
            int speed = playerShipObject.getSpeed();
            switch (wParam) 
            {
            case VK_UP:
                is_UP_pressed = true;
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, 0, -speed, playerShipObject.getObjectType());
                break;
            case VK_DOWN:
                is_DOWN_pressed = true;
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, 0, speed, playerShipObject.getObjectType());
                break;
            case VK_LEFT:
                is_LEFT_pressed = true;
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, -speed, 0, playerShipObject.getObjectType());
                break;
            case VK_RIGHT:
                is_RIGHT_pressed = true;
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, speed, 0, playerShipObject.getObjectType());
                break;
            case VK_SPACE:     
                if (isEnableSpace) {
                    EnterCriticalSection(&csection);
                    if (playerGuns.size() <= 5) {
                        playerGuns.push_back(CreatePlayerGun());  
                        LeaveCriticalSection(&csection);
                    }
                    else {
                        LeaveCriticalSection(&csection);
                        isEnableSpace = false;
                        DisableSpace();
                    }
                }
                break;
            }
            if (is_UP_pressed && is_LEFT_pressed)
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, -speed/2, -speed/2, playerShipObject.getObjectType());
            if (is_UP_pressed && is_RIGHT_pressed)
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, speed/2, -speed/2, playerShipObject.getObjectType());
            if (is_DOWN_pressed && is_LEFT_pressed)
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, -speed/2, speed/2, playerShipObject.getObjectType());
            if (is_DOWN_pressed && is_RIGHT_pressed)
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, speed/2, speed/2, playerShipObject.getObjectType());
        }
        break;
    case WM_KEYUP:
        {
            switch (wParam)
            {
            case VK_UP:
                is_UP_pressed = false;
                break;
            case VK_DOWN:
                is_DOWN_pressed = false;
                break;
            case VK_LEFT:
                is_LEFT_pressed = false;
                break;
            case VK_RIGHT:
                is_RIGHT_pressed = false;
                break;            
            }            
        }
        break;
    case WM_LBUTTONDOWN:
    {
        currCursorPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        if ((currCursorPos.x >= playerShipObject.objectCoordinates.left && currCursorPos.x <= playerShipObject.objectCoordinates.right) &&
            (currCursorPos.y >= playerShipObject.objectCoordinates.top && currCursorPos.y <= playerShipObject.objectCoordinates.bottom))
        {
            isRectCaptured = TRUE;
        }
        SetCapture(hWnd);
    }
    break;
    case WM_MOUSEMOVE:
        {
            if (isRectCaptured)
            {
                POINT prevPoint = currCursorPos;
                // Возвращает положение курсора мыши в экранных координатах
                GetCursorPos(&currCursorPos);
                // Преобразует экранные координаты указанной точки на экране в координаты клиентской области
                // Извлекает дескриптор окна верхнего уровня, 
                // имя класса и имя окна которого соответствуют указанным строкам
                ScreenToClient(FindWindowA(NULL, "SpaceInvaders"), &currCursorPos);
                MoveGameObject(hWnd, &playerShipObject.objectCoordinates, currCursorPos.x - prevPoint.x, currCursorPos.y - prevPoint.y, playerShipObject.getObjectType());
            }
        }
    break;
    case WM_LBUTTONUP:
        {
            isRectCaptured = FALSE;

            ReleaseCapture();
        }
    break;
    case WM_DESTROY: 
        {
            isGameOver = true;
            WaitForMultipleObjects(threads.size(), threads.data(), TRUE, INFINITE);
            DeleteGameObjects(hWnd);
            FinalizeBuffer();
            PostQuitMessage(0);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

VOID InitializeBuffer(HWND hWnd, int width, int height)
{
    // Получает контекст устройства из дескриптора окна
    HDC hdcWindow = GetDC(hWnd);
    // Создает растровое изображение, совместимое с устройством, которое связано с указанным контекстом устройства
    handleBuffer = CreateCompatibleBitmap(hdcWindow, width, height);
    // Создает контекст устройства памяти, совместимый с указанным устройством
    hdcBuffer = CreateCompatibleDC(hdcWindow);
    // Сохраняет текущее состояние указанного контекста устройства путем копирования данных
    SaveDC(hdcBuffer);
    // Выбирает объект в указанном контексте устройства.
    // Новый объект заменяет предыдущий объект того же типа.
    SelectObject(hdcBuffer, handleBuffer);
    ReleaseDC(hWnd, hdcWindow);
}

VOID FinalizeBuffer()
{
    if (hdcBuffer)
    {
        // Восстанавливает контекст устройства (DC) до указанного состояния
        // -1 - последнее сохранённое
        RestoreDC(hdcBuffer, -1);
        DeleteObject(handleBuffer);
        DeleteObject(hdcBuffer);
    }
}

VOID StartGame(HWND hWnd) {
    InitGame(hWnd);
    
    DeleteLevelButtons();
    InvalidateRect(hWnd, NULL, FALSE);
}

VOID InitGame(HWND hWnd)
{
    isOnce = false;
    isPlay = true;
    isGameOver = false;

    InitializeCriticalSection(&csection);

    GameSettings gameSettings = GameSettings();
    gameSettings.setLevelSettings(level);
    creationSpeed = gameSettings.creationSpeed;
    movementTimerOfAlien = gameSettings.movementTimerOfAlien;
    movementTimerOfAlienGun = gameSettings.movementTimerOfAlienGun;

    CreatePlayerShip();
    CreateAliens();

    SetTimer(hWnd, TIMER_1_ID, movementTimerOfAlien, NULL);
    SetTimer(hWnd, TIMER_2_ID, 50, NULL);
    SetTimer(hWnd, TIMER_3_ID, movementTimerOfAlienGun, NULL);

    DeleteAliens();
    DeleteOfAlienGuns();
    DeleteAliensByPlayerGuns();    
}

VOID CreatePlayerShip()
{
    GameSettings gameSettings = GameSettings();
    playerShipObject = PlayerShip(gameSettings.playerShipPic, gameSettings.playerShipMask, gameSettings.playerShipSpeed);
    playerShipObject.loadImage();
    playerShipObject.setObjectCoordinates(gameSettings.playerShipStartX, gameSettings.playerShipStartY);
}

PlayerGun CreatePlayerGun()
{
    GameSettings gameSettings = GameSettings();
    PlayerGun playerGunObject = PlayerGun(gameSettings.playerGunPic, gameSettings.playerGunMask, gameSettings.playerGunSpeed);
    playerGunObject.loadImage();
    playerGunObject.setObjectCoordinates(playerShipObject.objectCoordinates.left + (playerShipObject.objectCoordinates.right - playerShipObject.objectCoordinates.left) / 2.4, playerShipObject.objectCoordinates.top);
    return playerGunObject;
}

AlienGun CreateAlienGun(AlienShip alienShip)
{
    GameSettings gameSettings = GameSettings();
    gameSettings.setLevelSettings(level);
    AlienGun alienGunObject = AlienGun(gameSettings.alienGunPic, gameSettings.alienGunMask, gameSettings.alienGunSpeed);
    alienGunObject.loadImage();
    alienGunObject.setObjectCoordinates(alienShip.objectCoordinates.left + (alienShip.objectCoordinates.right - alienShip.objectCoordinates.left) / 4, alienShip.objectCoordinates.bottom);
    return alienGunObject;
}

AlienShip CreateAlienShip()
{
    GameSettings gameSettings = GameSettings();
    gameSettings.setLevelSettings(level);
    AlienShip alienShipObject = AlienShip(gameSettings.alienShipPic, gameSettings.alienShipMask, gameSettings.alienShipSpeed);
    alienShipObject.loadImage();
    alienShipObject.setObjectCoordinates(rand() % (windowSize.right - windowSize.left - 50),10);
    return alienShipObject;
}

VOID CreateAliens()
{
    HANDLE thread = CreateThread(NULL, 0, &CreationOfAliens, NULL, CREATE_SUSPENDED, NULL);
    if (thread != 0) {
        threads.push_back(thread);
        ResumeThread(thread);
    }
}

DWORD WINAPI CreationOfAliens(LPVOID obj)
{
    while (!isGameOver) {
        EnterCriticalSection(&csection);
        if (isGameOver)
            break;
        aliens.push_back(CreateAlienShip());
        alienGuns.push_back(CreateAlienGun(aliens.back()));
        LeaveCriticalSection(&csection);
        this_thread::sleep_for(creationSpeed);
    }
    ExitThread(0);
}

VOID DeleteAliensByPlayerGuns()
{
    HANDLE thread = CreateThread(NULL, 0, &DeletionOfAliensByPlayerGuns, NULL, CREATE_SUSPENDED, NULL);
    if (thread != 0) {
        threads.push_back(thread);
        ResumeThread(thread);
    }
}

DWORD WINAPI DeletionOfAliensByPlayerGuns(LPVOID obj)
{
    while (!isGameOver) {
        bool isHit = false;
        EnterCriticalSection(&csection);
        for (int i = 0; i < aliens.size(); i++) {
            if (isGameOver)
                break;
            for (int j = 0; j < playerGuns.size(); j++) {
                if (isGameOver) {
                    isHit = true;
                    break;
                }
                if (playerGuns[j].objectCoordinates.top <= windowSize.top + 15) {
                    playerGuns.erase(playerGuns.begin() + j);
                    playerGuns.shrink_to_fit();
                    break;
                }
                int alienArea = 15;
                if (aliens[i].objectCoordinates.bottom + alienArea >= playerGuns[j].objectCoordinates.top
                    && aliens[i].objectCoordinates.top - alienArea <= playerGuns[j].objectCoordinates.top
                    && aliens[i].objectCoordinates.left - alienArea <= playerGuns[j].objectCoordinates.left
                    && aliens[i].objectCoordinates.right + alienArea >= playerGuns[j].objectCoordinates.right) {
                    isHit = true;
                    aliens.erase(aliens.begin() + i);
                    aliens.shrink_to_fit();

                    playerGuns.erase(playerGuns.begin() + j);
                    playerGuns.shrink_to_fit();
                    break;
                }
            }
            if (isHit)
                break;
        }
        LeaveCriticalSection(&csection);
    }
    ExitThread(0);
}

VOID DeleteAliens()
{
    HANDLE thread = CreateThread(NULL, 0, &DeletionOfAliens, NULL, CREATE_SUSPENDED, NULL);
    if (thread != 0) {
        threads.push_back(thread);
        ResumeThread(thread);
    }
}

DWORD WINAPI DeletionOfAliens(LPVOID obj)
{
    while (!isGameOver) {
        EnterCriticalSection(&csection);
        for (int i = 0; i < aliens.size(); i++) {
            if (isGameOver)
                break;
            if (aliens[i].objectCoordinates.top > windowSize.bottom) {
                aliens.erase(aliens.begin() + i);
                aliens.shrink_to_fit();
            }
            int wings = 15;
            if (level == LevelsTypes::Level_4)
                int wings = 5;
            if (aliens[i].objectCoordinates.bottom - 5 >= playerShipObject.objectCoordinates.top
                && aliens[i].objectCoordinates.left >= playerShipObject.objectCoordinates.left + wings
                && aliens[i].objectCoordinates.right <= playerShipObject.objectCoordinates.right - wings
                && aliens[i].objectCoordinates.bottom <= playerShipObject.objectCoordinates.bottom) {
                isGameOver = true;
                break;
            }
        }
        LeaveCriticalSection(&csection);
    }
    ExitThread(0);
}

VOID DeleteOfAlienGuns() {
    HANDLE thread = CreateThread(NULL, 0, &DeletionOfAlienGuns, NULL, CREATE_SUSPENDED, NULL);
    if (thread != 0) {
        threads.push_back(thread);
        ResumeThread(thread);
    }
}

DWORD WINAPI DeletionOfAlienGuns(LPVOID) {
    while (!isGameOver) {
        EnterCriticalSection(&csection);
        for (int i = 0; i < alienGuns.size(); i++) {
            if (isGameOver)
                break;
            int wings = 15;
            if (alienGuns[i].objectCoordinates.bottom >= playerShipObject.objectCoordinates.top
                && alienGuns[i].objectCoordinates.left >= playerShipObject.objectCoordinates.left + wings
                && alienGuns[i].objectCoordinates.right <= playerShipObject.objectCoordinates.right - wings
                && alienGuns[i].objectCoordinates.bottom <= playerShipObject.objectCoordinates.bottom) {
                isGameOver = true;
                break;
            }

            if (alienGuns[i].objectCoordinates.top > windowSize.bottom) {
                alienGuns.erase(alienGuns.begin() + i);
                alienGuns.shrink_to_fit();
            }
        }
        LeaveCriticalSection(&csection);
    }

    ExitThread(0);
}

VOID MoveGameObject(HWND hWnd, LPRECT objectCoordinates, double xChange, double yChange, GameObjectType type)
{
    if (type == GameObjectType::playerShip) {
        if (objectCoordinates->top + yChange <= windowSize.bottom - ((windowSize.bottom - windowSize.top) / 3 ))
            yChange = windowSize.bottom - ((windowSize.bottom - windowSize.top) / 3) - objectCoordinates->top;
        CheckBorders(objectCoordinates, &xChange, &yChange);
    }     
    OffsetRect(objectCoordinates, xChange, yChange);
    InvalidateRect(hWnd, NULL, FALSE);
}

VOID CheckBorders(RECT* rectangle, double* xChange, double* yChange)
{
    if (rectangle->top + *yChange <= windowSize.top)
        *yChange = windowSize.top - rectangle->top;


    if (rectangle->bottom + *yChange >= windowSize.bottom)
        *yChange = windowSize.bottom - rectangle->bottom;


    if (rectangle->left + *xChange <= windowSize.left)
        *xChange = windowSize.left - rectangle->left;


    if (rectangle->right + *xChange >= windowSize.right)
        *xChange = windowSize.right - rectangle->right;
}

VOID DrawGame(HWND hWnd)
{
    // Draw background
    HBRUSH brushWindow = CreateSolidBrush(RGB(10, 13, 104));
    FillRect(hdcBuffer, &windowSize, brushWindow);

    HDC hdcPicture = CreateCompatibleDC(hdcBuffer);

    // Draw game objects
    playerShipObject.drawObject(&hdcPicture, &hdcBuffer);

    EnterCriticalSection(&csection);

    for (int i = 0; i < playerGuns.size(); i++)
            playerGuns[i].drawObject(&hdcPicture, &hdcBuffer);

    for (int i = 0; i < aliens.size(); i++)
            aliens[i].drawObject(&hdcPicture, &hdcBuffer);

    for (int i = 0; i < alienGuns.size(); i++)
            alienGuns[i].drawObject(&hdcPicture, &hdcBuffer);
        
    LeaveCriticalSection(&csection);

    DeleteDC(hdcPicture);

    // Copy to the screen
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    BitBlt(hdc, 0, 0, windowSize.right, windowSize.bottom, hdcBuffer, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
}

VOID DrawWindow(HWND hWnd, const wchar_t* imagePath, const wchar_t* maskPath) {
    HBRUSH brushWindow = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hdcBuffer, &windowSize, brushWindow);

    HDC hdcPicture = CreateCompatibleDC(hdcBuffer);

    HANDLE windowHandler;
    HANDLE windowMaskHandler;
    BITMAP windowBmp;
    BITMAP windowMaskBmp;

    windowHandler = LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(windowHandler, sizeof(windowBmp), &windowBmp);
    windowMaskHandler = LoadImage(NULL, maskPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(windowMaskHandler, sizeof(windowMaskBmp), &windowMaskBmp);

    int x = windowSize.left + ((windowSize.right - windowSize.left) / 2);
    int xCenterBmp = windowMaskBmp.bmWidth / 2;

    int y = windowSize.top + ((windowSize.bottom - windowSize.top) / 2);
    int yCenterBmp = windowMaskBmp.bmHeight / 2;

    HBITMAP objectmaskBmp = (HBITMAP)SelectObject(hdcPicture, windowMaskHandler);
    BitBlt(hdcBuffer, x - xCenterBmp, y - yCenterBmp, windowMaskBmp.bmWidth, windowMaskBmp.bmHeight, hdcPicture, 0, 0, SRCPAINT);

    HBITMAP objectpictureBmp = (HBITMAP)SelectObject(hdcPicture, windowHandler);
    BitBlt(hdcBuffer, x - xCenterBmp, y - yCenterBmp, windowBmp.bmWidth, windowBmp.bmHeight, hdcPicture, 0, 0, SRCAND);


    DeleteDC(hdcPicture);

    // Copy to the screen
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    BitBlt(hdc, 0, 0, windowSize.right, windowSize.bottom, hdcBuffer, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
}

VOID DeleteGameObjects(HWND hWnd) {
    aliens.clear();
    aliens.shrink_to_fit();

    alienGuns.clear();
    alienGuns.shrink_to_fit();

    playerGuns.clear();
    playerGuns.shrink_to_fit();

    threads.clear();
    threads.shrink_to_fit();

    KillTimer(hWnd, TIMER_1_ID);
    KillTimer(hWnd, TIMER_2_ID);
    KillTimer(hWnd, TIMER_3_ID);
}

HWND CreateButton(HWND hWnd,const wchar_t* text, int id, int x, int y, int width, int height) {
    HWND buttonHwnd = CreateWindow(TEXT("button"), text, WS_VISIBLE | WS_CHILD, x, y, width, height, hWnd, (HMENU)id, NULL, NULL);
    return buttonHwnd;

}

VOID CreateLevelButtons(HWND hWnd) {
    int tmp = (windowSize.right - windowSize.left) / 5;
    level_1_Hwnd = CreateButton(hWnd, L"Lelvel 1", BUTTON_LEVEL_1, tmp - 50, windowSize.bottom - 150, 100, 30);
    level_2_Hwnd = CreateButton(hWnd, L"Lelvel 2", BUTTON_LEVEL_2, tmp * 2 - 50, windowSize.bottom - 150, 100, 30);
    level_3_Hwnd = CreateButton(hWnd, L"Lelvel 3", BUTTON_LEVEL_3, tmp * 3 - 50, windowSize.bottom - 150, 100, 30);
    level_4_Hwnd = CreateButton(hWnd, L"Lelvel 4", BUTTON_LEVEL_4, tmp * 4 - 50, windowSize.bottom - 150, 100, 30);
}

VOID DeleteLevelButtons() {
    DestroyWindow(level_1_Hwnd);
    DestroyWindow(level_2_Hwnd);
    DestroyWindow(level_3_Hwnd);
    DestroyWindow(level_4_Hwnd);
}

VOID DisableSpace()
{
    HANDLE thread = CreateThread(NULL, 0, &Disable, NULL, CREATE_SUSPENDED, NULL);
    if (thread != 0) {
        threads.push_back(thread);
        ResumeThread(thread);
    }
}

DWORD WINAPI Disable(LPVOID obj)
{
    this_thread::sleep_for(2000ms);
    isEnableSpace = true;
    ExitThread(0);
}

