#include <Windows.h>
#include "Direct3D.h"
#include "Quad.h"
#include "Camera.h"


//�萔�錾
const wchar_t* WIN_CLASS_NAME = L"SampleGame";
const wchar_t* APP_NAME = L"�V�����b�Z�[�W������܂�";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



//�G���g���[�|�C���g
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E�N���X�i�݌v�}�j���쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;            //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_INFORMATION);   //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�}�E�X�J�[�\��
	wc.lpszMenuName = NULL;                     //���j���[�i�Ȃ��j
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //�w�i�i���j
	RegisterClassEx(&wc); //�N���X��o�^

	//�E�B���h�E�T�C�Y�̌v�Z�i�\���̈��WINDOW_WIDTH�@�~�@WINDOW_HEIGHT�Ɏw�肷�邽�߂̌v�Z�j
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //�E�B���h�E��
	int winH = winRect.bottom - winRect.top;     //�E�B���h�E����

	//�E�B���h�E���쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //�E�B���h�E�N���X��
		APP_NAME,     //�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
		CW_USEDEFAULT,       //�\���ʒu���i���܂����j
		CW_USEDEFAULT,       //�\���ʒu��i���܂����j
		winW,                 //�E�B���h�E��
		winH,                 //�E�B���h�E����
		NULL,                //�e�E�C���h�E�i�Ȃ��j
		NULL,                //���j���[�i�Ȃ��j
		hInstance,           //�C���X�^���X
		NULL                 //�p�����[�^�i�Ȃ��j
	);

	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	//Direct3D������
	HRESULT hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr))
	{
		return 0;
	}

	

	Quad* q = new Quad;
	q->Initialize();

	Camera::Initialize();

	//���b�Z�[�W���[�v�i�����N����̂�҂j
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{

		//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		//���b�Z�[�W�Ȃ�
		else
		{
			//�Q�[���̏���
			Camera::Update();
			Direct3D::BeginDraw();
			//q->Draw();

			//�`�揈��
			/*static int rot = 0;
			rot += 1;

			XMMATRIX mat = XMMatrixRotationY(XMConvertToRadians(rot));

			static float f = 0;
			f += 0.05;
			float scale = 1.5 + (sin(f) * 0.1);
			XMMATRIX smat = XMMatrixScaling(scale, scale, scale);

			mat = smat * mat;*/
			static float f = 0;
			f += 1;

			XMMATRIX tmat = XMMatrixTranslation(cos(f), sin(f), 0.0f);
			XMMATRIX mat = XMMatrixIdentity();
			mat = tmat;
			q->Draw(mat);

			Direct3D::EndDraw();
		}
	}

	q->Release();
	SAFE_DELETE(q);
	Direct3D::Release();

	return 0;
}



//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //�v���O�����I��
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//SDK(�\�t�g�E�F�A�f�x���b�v�����g�L�b�g)