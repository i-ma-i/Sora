// 頂点構造体
struct VertexPositionNormalTexture
{
    float3 Position : POSITION; // 頂点の位置
    float3 Normal : NORMAL; // 法線ベクトル
    float2 TexCoord : TEXCOORD; // テクスチャ座標
};

// 出力構造体
struct VS_OUTPUT
{
    float4 Position : SV_POSITION; // 変換された頂点位置
    float2 TexCoord : TEXCOORD; // テクスチャ座標
};

// 定数バッファ
cbuffer ConstantBuffer : register(b0)
{
    matrix World; // ワールド変換行列
    matrix View; // ビュー変換行列
    matrix Projection; // プロジェクション変換行列
};

// 頂点シェーダー関数
VS_OUTPUT main(VertexPositionNormalTexture input)
{
    VS_OUTPUT output;

    // ワールド、ビュー、プロジェクション行列を適用して頂点位置を変換
    float4 worldPosition = mul(float4(input.Position, 1.0f), World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

    // テクスチャ座標を出力
    output.TexCoord = input.TexCoord;

    return output;
}
