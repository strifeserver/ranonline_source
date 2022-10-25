#pragma	once
//rq
#include "../EngineUiLib/GUInterface/UIGroup.h"
#include "DxSkinChar.h"
#include "../RanClientLib/G-Logic/GLItem.h"
#include "../RanClientLib/G-Logic/GLItemMan.h"

class	CCharacterWindowPetRender : public CUIGroup
{

private:
	enum
	{
	};

public:
	CCharacterWindowPetRender ();
	virtual	~CCharacterWindowPetRender ();

public:
	void	CreateSubControl ();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );

public:
	DxSkinChar*			m_pSkinChar;
	D3DXMATRIX			m_matTrans;
	WORD				m_wLastCOLOR;
	WORD				m_wLastSTYLE;
	float				m_fDelay;
	D3DXVECTOR3			m_vRot;
	PETTYPE				m_emPETTYPE;
	SITEMCUSTOM			m_PutOnItems[PET_ACCETYPE_SIZE];

public:
	void	CreateModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void	RenderModel( LPDIRECT3DDEVICEQ pd3dDevice );
	void	UpdateSuit( LPDIRECT3DDEVICEQ pd3dDevice );
	void	ClearRender();
	void	CalculatePetMatrix();

};