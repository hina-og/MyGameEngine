#include "Quad.h"
#include "Camera.h"


Quad::Quad()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
}

Quad::~Quad()
{
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

HRESULT Quad::Initialize()
{
	// 頂点情報
	XMVECTOR vertices[] =
	{
		//XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),	// 四角形の頂点（左上）
		//XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	// 四角形の頂点（右上）
		//XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	// 四角形の頂点（右下）
		//XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),	// 四角形の頂点（左下）
		//XMVectorSet(0.5f, -1.4f, 0.0f, 0.0f),//五角形の頂点（中央上）
		//XMVectorSet(-0.5f, -1.4f, 0.0f, 0.0f),//六角形の頂点（どっか）

		XMVectorSet(-1.0f,  0.0f, 1.0f, 0.0f),//四角錐の頂点
		XMVectorSet(1.0f,  0.0f, 1.0f, 0.0f),//四角錐の頂点
		XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f),//四角錐の頂点
		XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f),//四角錐の頂点
		XMVectorSet(0.0f, 1.3, 0.0f, 0.0f),//四角錐の頂点
	};

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	if (FAILED(Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_)))
	{
		MessageBox(NULL, L"頂点バッファの作成に失敗", NULL, MB_OK);
		return E_FAIL;
	}
	Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	//インデックス情報
	int index[] = { 0,2,3, 0,1,2,/* 3,2,4,*/ /*3,4,5*/ 0,1,4, 1,2,4, 4,3,2, 0,4,3 };

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_)))
	{
		MessageBox(NULL, L"インデックスバッファの作成に失敗", NULL, MB_OK);
		return E_FAIL;
	}

	Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);

	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	if (Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", NULL, MB_OK);
		return E_FAIL;
	}
	Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	return S_OK;
}

void Quad::Draw()
{
	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	//頂点バッファ
	UINT stride = sizeof(XMVECTOR);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(18, 0, 0);
}

void Quad::Draw(XMMATRIX& worldMatrix)
{
	//コンスタントバッファに渡す情報
	
	//XMVECTOR position = { 0, 3, -10, 0 };	//カメラの位置
	//XMVECTOR target = { 0, 0, 0, 0 };	//カメラの焦点
	//XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));	//ビュー行列
	//XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列
	
	
	

	//D3D11_MAPPED_SUBRESOURCE pdata;
	//CONSTANT_BUFFER cb;
	//cb.matWVP = XMMatrixTranspose(Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	//Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	//memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データの値を送る
	//Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);

	//頂点バッファ
	UINT stride = sizeof(XMVECTOR);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(18, 0, 0);
}

void Quad::Release()
{
	Direct3D::pContext->Release();
	Direct3D::pDevice->Release();
}