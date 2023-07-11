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

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // 텍스처의 UV 좌표 값을 FLOAT형 3개로 표현하겠다는 매크로(괄호안의 숫자 0의 의미는 본래 버텍스에 텍스쳐 UV값이 여러개가 올 수 있는데 그중 0번째 값을 지정하겠다는 의미)

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
		_uint iMaxHp = 0;
		_uint iHp = 0;
		_uint iAttack = 0;
	}MONSTERSTAT;

	typedef struct tMaterial
	{
		void	Set_Material(D3DMATERIAL9& _material, D3DCOLORVALUE _color, _float _fPower = 100.f)
		{
			_material.Ambient = _color;
			_material.Diffuse = _color;
			_material.Specular = _color;
			_material.Emissive = _color;
			_material.Power = _fPower;
		}

		const D3DMATERIAL9 Get_Meretial(D3DCOLORVALUE _color, _float _fPower = 100.f)
		{
			D3DMATERIAL9 mat;

			mat.Ambient = _color;
			mat.Diffuse = _color;
			mat.Specular = _color;
			mat.Emissive = _color;
			mat.Power = _fPower;

			return mat;
		}

		D3DMATERIAL9 material;

	} CUSTOM_MATERIAL;
	static CUSTOM_MATERIAL MATERIAL;

}


#endif // Engine_Struct_h__
