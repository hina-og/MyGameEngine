#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include"Texture.h"
#include"Transform.h"
#include<vector>

using namespace DirectX;

//コンスタントバッファー


class Quad
{
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //ノーマル追加(法線ベクトル)
	};

protected:
	int vertexNum_; //頂点数
	std::vector<VERTEX> vertices_; //頂点情報
	int indexNum_; //インデックス数
	std::vector<int> indices_; //インデックス情報
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;
public:
	Quad();
	virtual ~Quad();
	HRESULT Initialize();
	void Draw(Transform& _transform);
	void Release();
private:
	virtual void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer(); //頂点バッファを作成
	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture();
	//-------------Drawから呼ばれる関数-------------//
	void PassDataToCB(Transform& _transform);
	void SetBufferToPipeline();
};

