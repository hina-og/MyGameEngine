#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include"Texture.h"
#include"Transform.h"
#include<vector>

using std::vector;

using namespace DirectX;

class Sprite
{
	//コンスタントバッファー
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};

private:
	uint64_t vertexNum_; //頂点数
	vector<VERTEX> vertices_; //頂点情報
	uint64_t indexNum_; //インデックス数
	vector<int> indices_; //インデックス情報
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;
public:
	Sprite();
	 ~Sprite();
    HRESULT Load(std::string _fileName);
	void Draw(Transform& _transform); //描画関数 const + & で引数を変更しない
	void Release();
private:
	//----------Initializeから呼ばれる関数----------//
	void InitVertexData(); //頂点情報の準備
	HRESULT CreateVertexBuffer(); //頂点バッファを作成

	void InitIndexData();
	HRESULT CreateIndexBuffer();

	HRESULT CreateConstantBuffer();

	HRESULT LoadTexture(std::string _fileName);
	//-------------Drawから呼ばれる関数-------------//
	void PassDataToCB(DirectX::XMMATRIX worldMatirx);
	void SetBufferToPipeline();
};

