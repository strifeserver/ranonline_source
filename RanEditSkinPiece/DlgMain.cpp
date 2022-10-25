// DlgMain.cpp : implementation file
//

#include "stdafx.h"
#include "RanEditSkinPiece.h"
#include "DlgMain.h"
#include "EtcFunction.h"
#include ".\dlgmain.h"
#include "RanEditSkinPieceView.h"
#include "DxSkinMeshMan.h"
#include "DxEffChar.h"
#include "ToolTab.h"

// CDlgMain dialog

IMPLEMENT_DYNAMIC(CDlgMain, CPropertyPage)
CDlgMain::CDlgMain( LOGFONT logfont )
	: CPropertyPage(CDlgMain::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, m_nMaterialSelected( -1 )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgMain::~CDlgMain()
{
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_TRACE, m_list_Strace );
	DDX_Control( pDX, IDC_LIST_EFF, m_list_CharEff );
	DDX_Control( pDX, IDC_LIST_MESH, m_list_Mesh );
	DDX_Control( pDX, IDC_LIST_MATERIAL, m_list_Material );
}


BEGIN_MESSAGE_MAP(CDlgMain, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_FINDSKIN, OnBnClickedButtonFindskin)
	ON_BN_CLICKED(IDC_BUTTON_FINDSKELETON, OnBnClickedButtonFindskeleton)
	ON_BN_CLICKED(IDC_BUTTON_SETSKINSKELETON, OnBnClickedButtonSetskinskeleton)
	ON_BN_CLICKED(IDC_BUTTON_SETMESH, OnBnClickedButtonSetmesh)
	ON_BN_CLICKED(IDC_BUTTON_TRACE_ADD, OnBnClickedButtonTraceAdd)
	ON_BN_CLICKED(IDC_BUTTON_TRACE_DEL, OnBnClickedButtonTraceDel)
	ON_BN_CLICKED(IDC_BUTTON_EFF_ADD, OnBnClickedButtonEffAdd)
	ON_BN_CLICKED(IDC_BUTTON_EFF_DEL, OnBnClickedButtonEffDel)
	ON_BN_CLICKED(IDC_BUTTON_EFF_EDIT, OnBnClickedButtonEffEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MATERIAL, OnNMClickListMaterial)
	ON_BN_CLICKED(IDC_BUTTON_MATERIAL_SIZE, OnBnClickedButtonMaterialSize)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TRACE, OnNMClickListTrace)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRACE, OnNMDblclkListTrace)
	ON_BN_CLICKED(IDC_BUTTON_EFF_OK, OnBnClickedButtonEffOk)
	ON_CBN_SELCHANGE(IDC_COMBO_PIECE_TYPE, OnCbnSelchangeComboPieceType)
	ON_CBN_SELCHANGE(IDC_COMBO_WEAPONWHEREBACK, OnCbnSelchangeComboWeaponwhereback)
END_MESSAGE_MAP()


// CDlgMain message handlers
BOOL CDlgMain::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;


	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Strace.SetExtendedStyle ( m_list_Strace.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Strace.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 2;	
		char* szColumnName1[nColumnCount] = { "Trace", "TracePos" };
		int nColumnWidthPercent[nColumnCount] = { 35, 65 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Strace.InsertColumn (i, &lvColumn );
		}
	}

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_CharEff.SetExtendedStyle ( m_list_CharEff.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_CharEff.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "DxEffChar" };
		int nColumnWidthPercent[nColumnCount] = { 100 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_CharEff.InsertColumn (i, &lvColumn );
		}
	}

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Mesh.SetExtendedStyle ( m_list_Mesh.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Mesh.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "Mesh" };
		int nColumnWidthPercent[nColumnCount] = { 100 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Mesh.InsertColumn (i, &lvColumn );
		}
	}

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Material.SetExtendedStyle ( m_list_Material.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Material.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 2;	
		char* szColumnName1[nColumnCount] = { "Material", "Mask" };
		int nColumnWidthPercent[nColumnCount] = { 50,50 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Material.InsertColumn (i, &lvColumn );
		}
	}

	std::string strWBT[EMPEACE_WEAPON_SIZE];
	for( int i=0; i<EMPEACE_WEAPON_SIZE; ++i )
	{
		strWBT[i] = SKINTYPE_STING::m_szWHEREBACKTYPE[i];
	}

	std::string strPT[PIECE_SIZE];
	for( int i=0; i<PIECE_SIZE; ++i )
	{
		strPT[i] = DxSkinPiece::m_szPIECETYPE[i];
	}

	SetWin_Combo_Init ( this, IDC_COMBO_PIECE_TYPE, strPT, PIECE_SIZE );
	SetWin_Combo_Init ( this, IDC_COMBO_WEAPONWHEREBACK, strWBT, EMPEACE_WEAPON_SIZE );

	std::string strBias[12] = 
	{
		"0 zBias",
		"1 zBias",
		"2 zBias",
		"3 zBias",
		"4 zBias",
		"5 zBias",
		"6 zBias",
		"7 zBias",
		"8 zBias",
		"9 zBias",
		"10 zBias",
		"11 zBias",
	};

	SetWin_Combo_Init ( this, IDC_COMBO_MATERIAL_BIAS, strBias, 12 );

	{
		CComboBox *pCombo = (CComboBox*) ( GetDlgItem(IDC_COMBO_EFF) );
		if ( pCombo )
		{
			DxEffCharType* pType = DxEffCharMan::GetInstance().GetEffectList();
			while( pType )
			{
				int nIndex = pCombo->AddString ( pType->NAME );
				pCombo->SetItemDataPtr ( nIndex, pType );
				pType = pType->pNext;
			}

			pCombo->SetCurSel( 0 );
		}
	}


	return TRUE;
}

void CDlgMain::SetData( DxSkinPiece*	pData )
{
	if ( pData ) 
	{
		m_pPiece = pData;

		if ( m_pPiece )
		{
			D3DXMATRIX mat;
			CRanEditSkinPieceView::GetView()->bRenderSkeleton = FALSE;
			CRanEditSkinPieceView::GetView()->bMeshDetect = TRUE;
			CRanEditSkinPieceView::GetView()->SetEditMatrix( mat, TRUE );

			ShowData();
		}
	}
}

void CDlgMain::ShowData()
{
	if ( m_pPiece )
	{
		SetWin_Text( this, IDC_EDIT_FILENAME, m_pPiece->m_szFileName );
		SetWin_Text( this, IDC_EDIT_SKIN, m_pPiece->m_szXFileName );
		SetWin_Text( this, IDC_EDIT_SKELETON, m_pPiece->m_szSkeleton );
		SetWin_Text( this, IDC_EDIT_MESHNAME, m_pPiece->m_szMeshName );

		SetWin_Combo_Sel ( this, IDC_COMBO_PIECE_TYPE, m_pPiece->m_emType );
		SetWin_Combo_Sel ( this, IDC_COMBO_WEAPONWHEREBACK, m_pPiece->m_emWeaponWhereBack );

		char szTempChar[512];
		int i = 0;

		m_list_Strace.DeleteAllItems();
		m_list_CharEff.DeleteAllItems();
		m_list_Mesh.DeleteAllItems();
		m_list_Material.DeleteAllItems();

		{ //mesh
			if ( m_pPiece->m_pSkinMesh )
			{
				i = 0;
				SMeshContainer* pMesh = m_pPiece->m_pSkinMesh->GetMeshClobalList ();
				while ( pMesh )
				{
					sprintf( szTempChar, "%s", pMesh->Name );
					m_list_Mesh.InsertItem( i, szTempChar );
					i ++;
					pMesh = pMesh->pGlobalNext;
				}
			}
		}

		{ //effchar
			i = 0;
			for ( size_t n=0; n<m_pPiece->m_vecEFFECT.size(); ++n )
			{
				sprintf( szTempChar, "%d-%s", m_pPiece->m_vecEFFECT[n]->GetTypeID(), m_pPiece->m_vecEFFECT[n]->GetName() );
				m_list_CharEff.InsertItem( i, szTempChar );
				m_list_CharEff.SetItemData( i, (DWORD_PTR) m_pPiece->m_vecEFFECT[n] );
				i ++;
			}
		}

		{// strace
			i = 0;
			for ( int nstrace = 0; nstrace < MAX_TRACE; nstrace ++ )
			{
				sprintf( szTempChar, "%s", szTRACE_NAME[nstrace] );
				m_list_Strace.InsertItem( i, szTempChar );

				SVERTEXINFLU* sTrace = m_pPiece->GetTracePos ( szTRACE_NAME[nstrace] );
				if ( sTrace )
				{
					sprintf( szTempChar, "%d~%4.2f~%4.2f~%4.2f", sTrace->m_dwIndex, sTrace->m_vVector.x , sTrace->m_vVector.y, sTrace->m_vVector.z );
					m_list_Strace.SetItemText( i, 1, szTempChar );
				}
				i ++;
			}
		}


		{ //material
			i = 0;
			for ( DWORD dw = 0; dw<m_pPiece->m_dwMaterialNum; ++ dw )
			{
				sprintf( szTempChar, "%s", m_pPiece->m_pmcMesh->pMaterials[dw].pTextureFilename );
				m_list_Material.InsertItem( i, szTempChar );
				sprintf( szTempChar, "%s", m_pPiece->m_pMaterialPiece[dw].m_strTexture.c_str() );
				m_list_Material.SetItemText( i, 1, szTempChar );
				i ++;
			}
		}
		

		SetWin_Enable( this, IDC_EDIT_MATERIAL_TEXTURE, FALSE );
		SetWin_Enable( this, IDC_COMBO_MATERIAL_BIAS, FALSE );
		SetWin_Enable( this, IDC_BUTTON_MATERIAL_SIZE, FALSE );

		SetWin_Enable( this, IDC_COMBO_EFF, FALSE );
		SetWin_Enable( this, IDC_BUTTON_EFF_OK, FALSE );

		m_nMaterialSelected = -1;
	}
}

BOOL CDlgMain::SaveData()
{
	if ( m_pPiece )
	{
		m_pPiece->m_emType = (EMPIECECHAR)GetWin_Combo_Sel ( this, IDC_COMBO_PIECE_TYPE );
		m_pPiece->m_emWeaponWhereBack = (EMPEACEZONEWEAPON)GetWin_Combo_Sel ( this, IDC_COMBO_WEAPONWHEREBACK );
		return TRUE;
	}

	return FALSE;
}

void CDlgMain::OnBnClickedButtonFindskin()
{
	if ( m_pPiece )
	{
		CString szFilterInput = "Skin (*.x)|*.x;|";
		CFileDialog dlgInput(true,"*.X*",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilterInput,this);
		dlgInput.m_ofn.lpstrInitialDir = DxSkinMeshMan::GetInstance().GetPath();
		if ( dlgInput.DoModal() != IDOK ) return;

		SetWin_Text( this, IDC_EDIT_SKIN, dlgInput.GetFileName() );
	}
}

void CDlgMain::OnBnClickedButtonFindskeleton()
{
	if ( m_pPiece )
	{
		CString szFilterInput = "Skeleton (*.x)|*.x;|";
		CFileDialog dlgInput(true,"*.X*",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilterInput,this);
		dlgInput.m_ofn.lpstrInitialDir = DxBoneCollector::GetInstance().GetPath();
		if ( dlgInput.DoModal() != IDOK ) return;

		SetWin_Text( this, IDC_EDIT_SKELETON, dlgInput.GetFileName() );
	}
}

void CDlgMain::OnBnClickedButtonSetskinskeleton()
{
	if ( m_pPiece )
	{
		std::string strSkin = GetWin_Text( this, IDC_EDIT_SKIN );
		std::string strSkeleton = GetWin_Text( this, IDC_EDIT_SKELETON );
		m_pPiece->ClearAll();
		HRESULT hr = m_pPiece->SetSkinMesh( strSkin.c_str(), strSkeleton.c_str(), CRanEditSkinPieceView::GetView()->Get3DDevice(), false );
		if ( FAILED( hr ) )
		{
			CDebugSet::MsgBox( "Failed Setting Skin Mesh Skin:%s Skeleton:%s", strSkin.c_str(), strSkeleton.c_str() );
			return;
		}
		ShowData();
	}
}

void CDlgMain::OnBnClickedButtonSetmesh()
{
	int nSelect = m_list_Mesh.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		std::string str = m_list_Mesh.GetItemText( nSelect, 0 );

		SMeshContainer* pMesh = m_pPiece->m_pSkinMesh->FindMeshContainer( (char*)str.c_str() );
		if ( pMesh )
		{
			m_pPiece->SelectMesh( pMesh );
			ShowData();
		}
	}
}

void CDlgMain::OnBnClickedButtonTraceAdd()
{
	int nSelect = m_list_Strace.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		std::string str = m_list_Strace.GetItemText( nSelect, 0 );
		SVERTEXINFLU sVertex;
		D3DXVECTOR3 vPOS = CRanEditSkinPieceView::GetView()->m_vCOL;
		DWORD dwCOLID = CRanEditSkinPieceView::GetView()->m_dwCOL;

		D3DXVECTOR3 vVALID = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		if ( vPOS != vVALID && dwCOLID != COL_ERR )
		{
			//sVertex.m_vVector = vPOS;
			//sVertex.m_dwIndex = dwCOLID;

			m_pPiece->DelTracePos( str.c_str() );
			m_pPiece->SetTracePos( str.c_str(), sVertex  );

			SVERTEXINFLU* pVtx = m_pPiece->GetTracePos( str.c_str() );
			if ( pVtx )
			{
				m_pPiece->m_pmcMesh->GetVertexInfluences( dwCOLID, pVtx );
				ShowData();
			}
			else
			{
				CDebugSet::MsgBox( "Error Reading newly added SVERTEXINFLU, coordinates may have been wrong" );
			}
		}
	}
}

void CDlgMain::OnBnClickedButtonTraceDel()
{
	int nSelect = m_list_Strace.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		std::string str = m_list_Strace.GetItemText( nSelect, 0 );
		m_pPiece->DelTracePos( str.c_str() );
		ShowData();
	}
}

void CDlgMain::OnBnClickedButtonEffAdd()
{
	if ( m_pPiece )
	{
		SetWin_Enable( this, IDC_COMBO_EFF, TRUE );
		SetWin_Enable( this, IDC_BUTTON_EFF_OK, TRUE );
	}
}

void CDlgMain::OnBnClickedButtonEffDel()
{
	int nSelect = m_list_CharEff.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		DxEffChar* pEff = ( DxEffChar* )m_list_CharEff.GetItemData( nSelect );
		if ( pEff )
		{
			m_pPiece->DelEffList( pEff );
			ShowData();
		}
	}
}

void CDlgMain::OnBnClickedButtonEffEdit()
{
	int nSelect = m_list_CharEff.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		DxEffChar* pEff = ( DxEffChar* )m_list_CharEff.GetItemData( nSelect );
		if ( pEff )
		{
			pEff->SetLinkObj ( NULL, m_pPiece );
			m_pToolTab->EffCharEdit( m_pPiece, pEff );
		}
	}
}

void CDlgMain::OnNMClickListMaterial(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Material.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		SetWin_Enable( this, IDC_EDIT_MATERIAL_TEXTURE, TRUE );
		SetWin_Enable( this, IDC_COMBO_MATERIAL_BIAS, TRUE );
		SetWin_Enable( this, IDC_BUTTON_MATERIAL_SIZE, TRUE );

		SetWin_Text( this, IDC_EDIT_MATERIAL_TEXTURE, m_pPiece->m_pMaterialPiece[nSelect].m_strTexture.c_str() );
		SetWin_Combo_Sel( this, IDC_COMBO_MATERIAL_BIAS, m_pPiece->m_pMaterialPiece[nSelect].m_dwZBias );
		m_nMaterialSelected = nSelect;
	}
}

void CDlgMain::OnBnClickedButtonMaterialSize() //save not size lol :P
{
	if ( m_pPiece )
	{
		if( m_nMaterialSelected == -1 || m_nMaterialSelected >= m_pPiece->m_dwMaterialNum ) return;
		m_pPiece->m_pMaterialPiece[m_nMaterialSelected].m_strTexture = GetWin_Text( this, IDC_EDIT_MATERIAL_TEXTURE );
		m_pPiece->m_pMaterialPiece[m_nMaterialSelected].m_dwZBias = GetWin_Combo_Sel( this, IDC_COMBO_MATERIAL_BIAS );
		m_pPiece->m_pMaterialPiece[m_nMaterialSelected].LoadTexture( CRanEditSkinPieceView::GetView()->Get3DDevice() );
		ShowData();
	}
}

void CDlgMain::OnNMClickListTrace(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;	
}

void CDlgMain::OnNMDblclkListTrace(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Strace.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece )
	{
		std::string str = m_list_Strace.GetItemText( nSelect, 0 );
		SVERTEXINFLU* pVertext = m_pPiece->GetTracePos( str.c_str() );
		if ( pVertext )
		{
			D3DXVECTOR3 vVecOutput;
			D3DXVECTOR3 vNorOutput;
			m_pPiece->m_pmcMesh->CalculateVertexInfluences( pVertext, vVecOutput, vNorOutput, NULL, TRUE );

			CRanEditSkinPieceView::GetView()->m_vCOL = vVecOutput;
			CRanEditSkinPieceView::GetView()->m_dwCOL = pVertext->m_dwIndex;
		}
	}
}

void CDlgMain::OnBnClickedButtonEffOk()
{
	if ( m_pPiece )
	{
		CComboBox *pCombo = (CComboBox*) ( GetDlgItem(IDC_COMBO_EFF) );
		if ( pCombo )
		{
			int nIndex = pCombo->GetCurSel ();
			DxEffCharType* pType = (DxEffCharType*)pCombo->GetItemData ( nIndex );
			if ( pType )
			{
				DxEffChar*	pEFF = DxEffCharMan::GetInstance().CreateEffInstance( pType->TYPEID );
				if ( pEFF )
				{
					pEFF->SetLinkObj ( NULL, m_pPiece );
					m_pToolTab->EffCharEdit( m_pPiece, pEFF, TRUE );

					SetWin_Enable( this, IDC_COMBO_EFF, FALSE );
					SetWin_Enable( this, IDC_BUTTON_EFF_OK, FALSE );
				}
			}
		}
	}
}

void CDlgMain::OnCbnSelchangeComboPieceType()
{
	SaveData();
}

void CDlgMain::OnCbnSelchangeComboWeaponwhereback()
{
	SaveData();
}
