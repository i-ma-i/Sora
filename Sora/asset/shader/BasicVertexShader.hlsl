// ���_�\����
struct VertexPositionNormalTexture
{
    float3 Position : POSITION; // ���_�̈ʒu
    float3 Normal : NORMAL; // �@���x�N�g��
    float2 TexCoord : TEXCOORD; // �e�N�X�`�����W
};

// �o�͍\����
struct VS_OUTPUT
{
    float4 Position : SV_POSITION; // �ϊ����ꂽ���_�ʒu
    float2 TexCoord : TEXCOORD; // �e�N�X�`�����W
};

// �萔�o�b�t�@
cbuffer ConstantBuffer : register(b0)
{
    matrix World; // ���[���h�ϊ��s��
    matrix View; // �r���[�ϊ��s��
    matrix Projection; // �v���W�F�N�V�����ϊ��s��
};

// ���_�V�F�[�_�[�֐�
VS_OUTPUT main(VertexPositionNormalTexture input)
{
    VS_OUTPUT output;

    // ���[���h�A�r���[�A�v���W�F�N�V�����s���K�p���Ē��_�ʒu��ϊ�
    float4 worldPosition = mul(float4(input.Position, 1.0f), World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

    // �e�N�X�`�����W���o��
    output.TexCoord = input.TexCoord;

    return output;
}
