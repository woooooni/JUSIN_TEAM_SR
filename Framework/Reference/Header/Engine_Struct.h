#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3		vPosition;		
		_ulong		dwColor;

	}VTXCOL;
	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;


	typedef struct tagVertexTexture
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;

	}VTXTEX;
	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTXCUBE;
	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)

	typedef struct tagVertexCubeCol
	{
		_vec3		vPosition;
		_ulong		dwColor;

	}VTXCUBE_COL;
	const _ulong	FVF_CUBECOL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagIndex16
	{
		_ushort  _0;
		_ushort  _1;
		_ushort  _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;

	}INDEX32;

	typedef struct tagObjectInfo
	{
		bool m_bIsPushable = false;
		bool m_bIsGrabbable = false;
		bool m_bIsAttackable = false;
		bool m_bIsBreakable = false;
	}OBJ_INFO;
	
	typedef struct tagMonsterstats
	{
		_int iMaxHp = 0;
		_int iHp = 0;
		_int iAttack = 0;
	}MONSTERSTAT;


	typedef struct tUI_INFO
	{
		_float fX;
		_float fY;
		_float fCX;
		_float fCY;
	}UI_INFO;

	
}


#endif // Engine_Struct_h__
