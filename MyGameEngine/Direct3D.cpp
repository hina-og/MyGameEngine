#include <d3dcompiler.h>
#include "Direct3D.h"

//�ϐ�
namespace Direct3D
{
	ID3D11Device* pDevice = nullptr;		//�f�o�C�X
	ID3D11DeviceContext* pContext = nullptr;		//�f�o�C�X�R���e�L�X�g
	IDXGISwapChain* pSwapChain = nullptr;		//�X���b�v�`�F�C��
	ID3D11RenderTargetView* pRenderTargetView = nullptr;	//�����_�[�^�[�Q�b�g�r���[

	ID3D11VertexShader* pVertexShader = nullptr;	//���_�V�F�[�_�[
	ID3D11PixelShader* pPixelShader = nullptr;		//�s�N�Z���V�F�[�_�[
	ID3D11InputLayout* pVertexLayout = nullptr;	//���_�C���v�b�g���C�A�E�g
	ID3D11RasterizerState* pRasterizerState = nullptr;	//���X�^���C�U�[
}

//������
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr;
	///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
	DXGI_SWAP_CHAIN_DESC scDesc;

	//�Ƃ肠�����S��0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = 800;		//��ʕ�
	scDesc.BufferDesc.Height = 600;	//��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

	//FPS�i1/60�b��1��j
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
	scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
	scDesc.SampleDesc.Quality = 0;		//�@�V

	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,				// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
		nullptr,				// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		0,					// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
		nullptr,				// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
		0,					// ��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION,			// SDK�̃o�[�W�����B�K�����̒l
		&scDesc,				// ��ł��낢��ݒ肵���\����
		&pSwapChain,				// ������������SwapChain�̃A�h���X���Ԃ��Ă���
		&pDevice,				// ������������Device�A�h���X���Ԃ��Ă���
		&level,					// ������������Device�AContext�̃��x�����Ԃ��Ă���
		&pContext);				// ������������Context�̃A�h���X���Ԃ��Ă���

	if (FAILED(hr))
	{
		return hr;
	}

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	if (FAILED(hr))
	{
		return hr;
	}

	//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
	pBackBuffer->Release();

	///////////////////////////�r���[�|�[�g�i�`��͈́j�ݒ�///////////////////////////////
	//�����_�����O���ʂ�\������͈�
	D3D11_VIEWPORT vp;
	vp.Width = 800;	//��
	vp.Height = 600;//����
	vp.MinDepth = 0.0f;	//��O
	vp.MaxDepth = 1.0f;	//��
	vp.TopLeftX = 0;	//��
	vp.TopLeftY = 0;	//��

	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // �f�[�^�̓��͎�ނ��w��
	pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);            // �`����ݒ�
	pContext->RSSetViewports(1, &vp);

	//�V�F�[�_�[����
	InitShader();
	return S_OK;
}

HRESULT Direct3D::InitShader()
{
	HRESULT hr;

	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);

	if (FAILED(hr))
	{
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�ʒu
	};
	pDevice->CreateInputLayout(layout, 1, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	pCompileVS->Release();

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompileVS != nullptr);
	pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
	pCompilePS->Release();


	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;//���p�`�̗����͕`�悵�Ȃ��i�J�����O�j
	rdc.FillMode = D3D11_FILL_WIREFRAME;//���p�`�̓�����h��Ԃ�
	rdc.FrontCounterClockwise = TRUE;//�����v����\�ɂ��邩�ǂ����i��false�Ȃ̂Ŏ��v��肪�\�j
	pDevice->CreateRasterizerState(&rdc, &pRasterizerState);

	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	pContext->VSSetShader(pVertexShader, NULL, 0);	//���_�V�F�[�_�[
	pContext->PSSetShader(pPixelShader, NULL, 0);	//�s�N�Z���V�F�[�_�[
	pContext->IASetInputLayout(pVertexLayout);	//���_�C���v�b�g���C�A�E�g
	pContext->RSSetState(pRasterizerState);		//���X�^���C�U�[

	return S_OK;
}

//�`��J�n
void Direct3D::BeginDraw()
{
	Sleep(1);

	//�w�i�̐F
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	//��ʂ��N���A
	pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
}

//�`��I��
void Direct3D::EndDraw()
{
	//�X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
	pSwapChain->Present(0, 0);
}

//�������
void Direct3D::Release()
{
	pRasterizerState->Release();
	pVertexLayout->Release();
	pPixelShader->Release();
	pVertexShader->Release();


	//�������
	pRenderTargetView->Release();
	pSwapChain->Release();
	pContext->Release();
	pDevice->Release();
}