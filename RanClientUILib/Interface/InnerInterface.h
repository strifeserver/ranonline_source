#pragma	once

#include "../EngineUIlib/GUInterface/UIMan.h"
#include "../RanClientLib/G-Logic/GLCharClient.h"
#include "./InnerInterfaceGuid.h"
#include "../RanClientLib/G-Logic/GLogicEx.h"
class	CBasicProgressBar;
class	CInventoryViewCharItemWindow;
class	CInventoryPageWearView;
class	CTowerFinalDisplay;
class	CTowerCapturedDisplay;
class	CItemMallIcon;
class	CItemShopIconMan;
class	CItemShopWindow;

//class	CTowerCapturedDisplayFacility;
class	CCTFInfoDisplay;
class	CTowerDownDisplay;
class	CTowerHPDisplay;
class	CAcademyConftDisplay;	
class	CAdminMessageDisplay;	
class	CBasicButton;	
class	CBasicChat;	
class	CBasicGameMenu;	
class   CQBoxButton;
class	CBasicInfoView;	
class	CBasicPotionTray;	
class	CBasicQuickSkillSlot;	

class	CBasicSkillTray;
class	CBasicSkillTrayNew;
class	CBasicVarTextBox;	
class	CBlockProgramAlarm;	
class	CBusWindow;	
class	CCharacterWindow;	
class	CCharacterWindowCharAdditionalInfo; //add addtional info by CNDev
class	CCharacterViewPropertiesWindow;//add charinfoview
class	CChatMacroWindow;
class	CChatOptionWindow;	
class	CClubMake;	
class	CClubStorageWindow;	
class	CClubWindow;	
class	CConftConfirm;	
class	CConftDisplayMan;	
class	CConftModalWindow;	
class	CCountMsg;	
class	CDamageDisplayMan;	
class	CDialogueWindow;	
class	CFriendWindow;	
class	CGambleAgainBox;	
class	CGambleAnimationBox;	
class   CPetSkinMixImage;
class	CGambleBox;	
class	CGambleResultEvenBox;	
class	CGambleResultOddBox;	
class	CGambleSelectBox;	
class	CHeadChatDisplayMan;	
class	CInventoryWindow;	
class	CItemBankWindow;	
class	CVNGainSysInventory;
class   CVNGainSysGauge;
class	CItemMove;	
class	CItemRebuild;	// ITEMREBUILD_MARK
class	CKeySettingWindow;	
class	CLargeMapWindow;	
class	CMapMoveDisplay;	
class	CMarketWindow;	
class	CMiniMap;	
class	CMiniPartyWindow;	
class	CModalWindow;	
class	CNameDisplayMan;	
class	CPartyModalWindow;	
class	CPartyWindow;	
class	CPetRebirthDialogue;// Monster7j	
class	CPetWindow;	
class	CPrivateMarketMake;	
class	CPrivateMarketSellWindow;	
class	CPrivateMarketShowMan;	
class	CPrivateMarketWindow;	
class	CPtoPWindow;	
class	CQuestAlarm;	
class	CQuestionItemDisplay;	
class	CQuestWindow;	
class	CRebirthDialogue;	
class	CRebuildInventoryWindow;	
class	CReceiveNoteWindow;	
class	CSimpleHP;	
class	CSimpleMessageMan;	
class	CSkillTrayTab;	
class	CSkillTrayTabNew;
class	CSkillWindowToTray;	
class	CSMSSendWindow;	
class	CStorageChargeCard;	
class	CStorageWindow;	
class	CSubMasterSet;	
class	CSystemMessageWindow;	
class	CTargetInfoDisplay;	
class	CTargetInfoDisplayNpc;
class	CTargetInfoDisplayPlayer;
class	CTradeInventoryWindow;	
class	CTradeWindow;	
class	CUILeftTopGroup;	
class	CWaitServerDialogue;	
class	CWriteNoteWindow;	
struct	GLCHARLOGIC;	
class	GLCLUB;	
class	GLMapAxisInfo;	
struct	GLSKILL;	
struct	SCHARSKILL;	
struct	SITEMCUSTOM;	
struct	SNpcTalk;	
class	CBonusTimeGauge;
class	CBonusTimeDisplay;
class	CQuestHelper;
class	CThaiCCafeMark;
class	CItemGarbage;
class	CGarbageInventoryWindow;
class	CShopItemSearchWindow;
class	CItemSearchResultWindow;
class	CSummonWindow;
class	CSummonPosionDisplay;
class	CAttendanceBookWindow;
class	CClubBattleModal;
class	CMapRequireCheck;
class	CTaxiWindow;
class	CItemMixWindow;
class	CItemMixInvenWindow;
class	CGatherGauge;
class	CCdmRankingDisplay;
class	CQBoxTimeUnit;			//add qbox

class	CPvpWindowDisplay;
class	CPetTimeUnit;			//add petimage
class	CVehicleTimeUnit;		//add vehicleimage
class	CPetDisplay;			//add petimage
class	CVehicleDisplay;		//add vehicleimage
class	CNotifyPost;			//add notify
class	CNotifyRecord;			//add notify
class	CNotifyCompete;			//add notify
class	CItemCompound;
class	CItemPreview;	//add itempreview
class	CPlayerKillStreakDisplay;	//add pk streak
class	CBasicQuickSkillWindow;
class	CBasicQuickSkillWindowNew;
class	CBoxItemInfo;
class	CItemPreviewWindow;
class	CPartyDisplay;
class	CPartyDisplayBuff;
class	CBasicVarTextBoxEx;
class	CFriendWindowNew;

class	CSwRankingDisplay;		//add school wars
class	CSwPRankingDisplay;
//add sw buff
class	CSwBonusMark;
class	CCrowTargetInfo;
class	CCrowTargetInfoNpc;
class	CCrowTargetInfoPlayer;

class	CSchoolWarWindow;
class	CRRRankingDisplay;		//add school wars
class	CRRPRankingDisplay;

class	CNotifyRequestIcon;
class	CNotifyRequestWindow;

class	CTargetInfoCtfDisplay;
class	CForceRebirth;		
class	CCtfRankingDisplay;
class	CCtfInfoDisplay;
class	CProgressDisplayCtf;
//	NOTE
//		��ýÿ� Ű����
enum
{
	CONFT_WIN_INDEX = 0,
	CONFT_LOSE_INDEX,
	CONFT_DRAW_INDEX
};

//	NOTE
//		�б�
enum
{	
	SUNGMOON = 0,
	HYUNAM,
	BONGHWANG,
	INIT_SCHOOL = 100,
};

const float fDEFAULT_WAITTIME_LEFT = 5.0f;

class	CInnerInterface : public CUIMan
{
private:
	//	���� ǥ�� Ÿ��
	enum ET_INFOTYPE
	{		
		ET_ITEM_INFO,
		ET_SKILL_INFO,
		ET_GENERAL_INFO,
		ET_ITEM_INFO_LINK,
	};

public:
	static const int nOUTOFRANGE;
	static const float fMENU_LIFTUP;
	//CPartyDisplay*				m_pPartyDisplay;  
private: //	��ϵǴ� ��� ��Ʈ��
	
	//CInventoryViewCharItemWindow*				m_pInventoryViewCharItemWindow;
	CInventoryPageWearView*				m_pInventoryWindowView;

	CAcademyConftDisplay*		m_pAcademyConftDisplay;				
	CAdminMessageDisplay*		m_pAdminMessageDisplay;				
	CBasicChat*					m_pChat;				
	CBasicGameMenu*				m_pGameMenu;		
	CQBoxButton*				m_pQBoxButton;
	CQBoxTimeUnit*				m_pQBoxTimeUnit;//add q box
	CPetTimeUnit*				m_pPetTimeUnit;//add petimage
	CVehicleTimeUnit*			m_pVehicleTimeUnit;//add vehicleimage
	CTowerHPDisplay*			m_pTowerHPDisplay;
	CTowerFinalDisplay*			m_pTowerFinalDisplay;
	CBasicInfoView*				m_pBasicInfoView;
	CBasicQuickSkillSlot*		m_pBasicQuickSkillSlot;				
	CBasicVarTextBox*			m_pInfoDisplay;			
	CBasicVarTextBox*			m_pInfoDisplayEx;	
	CBasicVarTextBoxEx*			m_pInfoDisplayLink;	
	CBlockProgramAlarm*			m_pBlockProgramAlarm;				
	CBusWindow*					m_pBusWindow;				
	CCharacterWindow*			m_pCharacterWindow;			
	CCharacterWindowCharAdditionalInfo*		m_pCharacterWindowAdditionalInfo;
	CCharacterViewPropertiesWindow*			m_pCharacterViewPropertiesWindow;	//add charinfoview
	CChatMacroWindow*			m_pChatMacroWindow;			
	CChatOptionWindow*			m_pChatOptionWindow;
	CClubMake*					m_pClubMake;	
	CClubStorageWindow*			m_pClubStorageWindow;			
	CClubWindow*				m_pClubWindow;		
	CConftConfirm*				m_pConftConfirm;		
	CConftDisplayMan*			m_pConftDisplayMan;			
	CConftModalWindow*			m_pConftModalWindow;			
	CCountMsg*					m_pCountMsgWindow;	
	CDamageDisplayMan*			m_pDamageDisplayMan;				
	CDialogueWindow*			m_pDialogueWindow;				
	CFriendWindow*				m_pFriendWindow;				
	CGambleAgainBox*			m_pGambleAgainBox;			
	CGambleAnimationBox*		m_pGambleAnimationBox;				
	CPetSkinMixImage*			m_pPetSkinMixImage;
	CGambleBox*					m_pGambleBox;	
	CGambleResultEvenBox*		m_pGambleResultEvenBox;				
	CGambleResultOddBox*		m_pGambleResultOddBox;				
	CGambleSelectBox*			m_pGambleSelectBox;			
	CHeadChatDisplayMan*		m_pHeadChatDisplayMan;				
	CInventoryWindow*			m_pInventoryWindow;			
	CItemShopWindow*			m_pItemShopWindow;		
	//CItemBankWindow*			m_pItemBankWindow;				
	CVNGainSysInventory*		m_pVNGainSysInventory;				
	CVNGainSysGauge*			m_pVNGainSysGauge;
	CItemMove*					m_pItemMove;		
	CItemRebuild*				m_pItemRebuildWindow;	// ITEMREBUILD_MARK	
	CKeySettingWindow*			m_pKeySettingWindow;			
	CLargeMapWindow*			m_pLargeMapWindow;			
	CMapMoveDisplay*			m_pMapMoveDisplay;			
	CMarketWindow*				m_pMarketWindow;			
	CMiniMap*					m_pMiniMap;				
	CModalWindow*				m_pModalWindow;		
	CNameDisplayMan*			m_pNameDisplayMan;				
	CPartyModalWindow*			m_pPartyModalWindow;			
	CPartyWindow*				m_pPartyWindow;			
	CPetRebirthDialogue*		m_pPetRebirthDialogue;// Monster7j				
	CPetWindow*					m_pPetWindow;	
	CPrivateMarketMake*			m_pPrivateMarketMake;			
	CPrivateMarketSellWindow*	m_pPrivateMarketSellWindow;				
	CPrivateMarketShowMan*		m_pPrivateMarketShowMan;				
	CPrivateMarketWindow*		m_pPrivateMarketWindow;				
	CPtoPWindow*				m_pPtoPWindow;			
	CQuestAlarm*				m_pQuestAlarm;				
	CQuestionItemDisplay*		m_pQuestionItemDisplay;				
	CTowerDownDisplay*			m_pTowerDownDisplay;
	CTowerCapturedDisplay*		m_pTowerCapturedDisplay;
//	CTowerCapturedDisplayFacility*	m_pTowerCapturedDisplayFacility;
	CCTFInfoDisplay*			m_pCTFInfoDisplay;
	CQuestWindow*				m_pQuestWindow;				
	CRebirthDialogue*			m_pRebirthDialogue;			
	CRebuildInventoryWindow*	m_pRebuildInventoryWindow;				
	CSimpleHP*					m_pSimpleHP;				
	CSimpleHP*					m_pSummonHP;				
	CSkillTrayTab*				m_pSkillTrayTab;	
	CSkillTrayTabNew*			m_pSkillTrayTabNew;
	CSkillWindowToTray*			m_pSkillWindowToTray;				
	CStorageChargeCard*			m_pStorageChargeCard;			
	CStorageWindow*				m_pStorageWindow;			
	CSubMasterSet*				m_pSubMasterSet;		// �θ����� ���� â
	CSystemMessageWindow*		m_pSystemMessageWindow;				
	CTargetInfoDisplay*			m_pTargetInfoDisplay;			
	CTargetInfoDisplayNpc*		m_pTargetInfoDisplayNpc;
	CTargetInfoDisplayPlayer*	m_pTargetInfoDisplayPlayer;	
	CTradeInventoryWindow*		m_pTradeInventoryWindow;				
	CTradeWindow*				m_pTradeWindow;			
	CUILeftTopGroup*			m_pUILeftTopGroup;			
	CWaitServerDialogue*		m_pWaitServerDisplay;
	CBonusTimeGauge*			m_pBonusTimeGauge;
	CBonusTimeDisplay*			m_pBonusTimeDisplay;
	CQuestHelper*				m_pQuestHelper;
//	CVehicleWindow*				m_pVehicleWindow;
	CThaiCCafeMark*				m_pThaiCCafeMark;
	//add sw buff
	CSwBonusMark*				m_pSwBonusMark;
	CItemMallIcon*				m_pItemMallIcon;
	CItemGarbage*				m_pItemGarbageWindow;
	CGarbageInventoryWindow*	m_pGarbageInventoryWindow;
	CItemShopIconMan*			m_pItemShopIconMan;
	CShopItemSearchWindow*		m_pShopItemSearchWindow;
	CItemSearchResultWindow*	m_pItemSearchResultWindow;
	CSummonWindow*				m_pSummonWindow;
	CAttendanceBookWindow*		m_pAttendanceBookWindow;
	CClubBattleModal*			m_pClubBattleModalWindow;
	CTaxiWindow*				m_pTaxiWindow;
	CMapRequireCheck*			m_pMapRequireCheckWindow;
	CItemMixWindow*				m_pItemMixWindow;
	CItemMixInvenWindow*		m_pItemMixInvenWindow;
	CGatherGauge*				m_pGatherGauge;
	CCdmRankingDisplay*			m_pCdmRankingDisplay;
	CPetDisplay*				pPetDisplay; //add petimage
	CVehicleDisplay*			pVehicleDisplay; //add vehicleimage
	CItemPreview*				m_pItemPreview; //add itempreview
	CPlayerKillStreakDisplay*	m_pPlayerKillDisplay; //add pk streak
	CBasicButton* 				pButton;
	CBasicQuickSkillWindow*		m_pBasicQuickSkillWindow;
	CBasicQuickSkillWindowNew*	m_pBasicQuickSkillWindowNew;
	CItemPreviewWindow*			m_pItemPreviewWindow;
	CPartyDisplay*				m_pPartyDisplay;
	CPartyDisplayBuff*				m_pPartyDisplayBuff;
	CCrowTargetInfo*			m_pCrowTargetInfo; //Add new Interface
	CCrowTargetInfoNpc*			m_pCrowTargetInfoNpc; //Add new Interface
	CCrowTargetInfoPlayer*		m_pCrowTargetInfoPlayer; //Add new Interface
	CFriendWindowNew*			m_pFriendWindowNew;
	CPvpWindowDisplay*			m_pPvpWindowDisplay;
	CSwRankingDisplay*			m_pSwRankingDisplay;		//add school wars
	CBoxItemInfo*				m_pBoxInfo; 
	CSwPRankingDisplay*			m_pSwPRankingDisplay;
	CSchoolWarWindow*			m_pSchoolWarWindow;

	CRRRankingDisplay*			m_pRrRankingDisplay;		//add school wars
	CRRPRankingDisplay*			m_pRrPRankingDisplay;

	CNotifyRequestIcon*			m_pNotifyRequest;
	CNotifyRequestWindow*		m_pNotifyRequestWindow;
	CTargetInfoCtfDisplay*		m_pTargetInfoCtfDisplay;
	CForceRebirth*				m_pForceRebirth;
	CCtfRankingDisplay*			m_pCtfRankingDisplay;
	CCtfInfoDisplay*			m_pCtfInfoDisplay;
	CProgressDisplayCtf*		m_pProgressCtfDisplay;
private:
	CUIControl*	m_pSystemMessageWindowDummy;
	CUIControl*	m_pMapMoveDisplayDummy;
	CUIControl*	m_pBlockProgramAlarmDummy;
	CUIControl*	m_pLeftTopGroupDummy;
	CUIControl*	m_pBasicInfoViewDummy;
	CUIControl* m_pBasicPotionTrayDummy;
	CUIControl* m_pQuestAlarmDummy;

	void ResetControl();

private:
	bool m_bFirstVNGainSysCall;

public:
	CInnerInterface ();
	virtual	~CInnerInterface ();

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT OneTimeSceneInit ();
	//virtual	HRESULT FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fElapsedTime );
	virtual	HRESULT FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT FinalCleanup ();

public:
	void SetMiniMapInfo ( GLMapAxisInfo &sInfo, CString strMapName );
	void SetFirstVNGainSysCall ( bool bFirstCall ) { m_bFirstVNGainSysCall = bFirstCall; }

public:
void CInnerInterface::SchoolWarStarted( NET_MSG_GENERIC* nmg );
	void CInnerInterface::SchoolWarEnded( NET_MSG_GENERIC* nmg );
	CMiniMap*				GetMiniMap()					{ return m_pMiniMap; }
	CCTFInfoDisplay*		GetCTFInfoDisplay()				{ return m_pCTFInfoDisplay; }
	CBasicQuickSkillSlot*	GetBasicQuickSkillSlot()		{ return m_pBasicQuickSkillSlot; }
	CInventoryPageWearView*			GetInventoryViewWindow()				{ return m_pInventoryWindowView; }
	void CInnerInterface::SetInventoryViewWindowOpen ( DWORD dwTargetID );
	void CInnerInterface::SetInventoryViewWindowItem ( int nIndex , SITEMCUSTOM& sItemCustom );
	void CInnerInterface::CloseItemViewWindow();
	CItemShopWindow*		GetItemShop()					{ return m_pItemShopWindow; }
	CPvpWindowDisplay*		GetPvpWindow()					{ return m_pPvpWindowDisplay; }
	CSkillWindowToTray*		GetSkillWindowToTray()			{ return m_pSkillWindowToTray; }
	CSkillTrayTab*		    GetSkillTrayTab()				{ return m_pSkillTrayTab; }
	CSkillTrayTabNew*		GetSkillTrayTabNew()			{ return m_pSkillTrayTabNew; }
	CPartyWindow*			GetPartyWindow()				{ return m_pPartyWindow; }
	CBasicChat*				GetChat()						{ return m_pChat; }
	CNameDisplayMan*		GetDispName()					{ return m_pNameDisplayMan; }
	CPrivateMarketShowMan*	GetPMarketShow()				{ return m_pPrivateMarketShowMan; }
	CTradeWindow*			GetTradeWindow()				{ return m_pTradeWindow; }
	CTradeInventoryWindow*	GetTradeInventoryWindow()		{ return m_pTradeInventoryWindow; }
	CPtoPWindow*			GetPtoPWindow()					{ return m_pPtoPWindow; }
	CInventoryWindow*		GetInventoryWindow()			{ return m_pInventoryWindow; }
	CItemMove*				GetItemMove()					{ return m_pItemMove; }
	CDialogueWindow*		GetDialogueWindow()				{ return m_pDialogueWindow; }	
	CMarketWindow*			GetMarketWindow()				{ return m_pMarketWindow; }
	CHeadChatDisplayMan*	GetHeadChatDisplayMan()			{ return m_pHeadChatDisplayMan; }
	CConftModalWindow*		GetConftModalWindow()			{ return m_pConftModalWindow; }
	CPartyModalWindow*		GetPartyModalWindow()			{ return m_pPartyModalWindow; }
	CConftDisplayMan*		GetConftDisplayMan()			{ return m_pConftDisplayMan; }
	CQuestWindow*			GetQuestWindow()				{ return m_pQuestWindow; }
	CFriendWindow*			GetFriendWindow()				{ return m_pFriendWindow; }
	CBasicGameMenu*			GetGameMenu()					{ return m_pGameMenu; }
	CQBoxButton*			GetQBoxButton()					{ return m_pQBoxButton; }
	CPetWindow*				GetPetWindow()					{ return m_pPetWindow; }
	CCharacterWindow*		GetCharacterWindow()			{ return m_pCharacterWindow; }
	CLargeMapWindow*		GetLargeMapWindow()				{ return m_pLargeMapWindow; }
	CCharacterWindowCharAdditionalInfo*	GetCharacterWindowAdditionalInfo()			{ return m_pCharacterWindowAdditionalInfo; }
	CCharacterViewPropertiesWindow*		GetCharacterViewPropertiesWindow()			{ return m_pCharacterViewPropertiesWindow; }//add charinfoview
	CGambleBox*				GetGambleBox()					{ return m_pGambleBox; }
	CGambleSelectBox*		GetGambleSelectBox()			{ return m_pGambleSelectBox; }
	CGambleAgainBox*		GetGambleAgainBox()				{ return m_pGambleAgainBox; }
	CGambleAnimationBox*	GetGambleAnimationBox()			{ return m_pGambleAnimationBox; }
	CPetSkinMixImage*		GetPetSkinMixImage()			{ return m_pPetSkinMixImage; }
	CGambleResultEvenBox*	GetGambleResultEvenBox()		{ return m_pGambleResultEvenBox; }
	CGambleResultOddBox*	GetGambleResultOddBox()			{ return m_pGambleResultOddBox; }
	CModalWindow*			GetModalWindow()				{ return m_pModalWindow; }
	CKeySettingWindow*		GetKeySettingWindow()			{ return m_pKeySettingWindow; }
	CChatMacroWindow*		GetChatMacroWindow()			{ return m_pChatMacroWindow; }
	CChatOptionWindow*		GetChatOptionWindow()			{ return m_pChatOptionWindow; }
	CItemShopIconMan*		GetItemShopIconMan()			{ return m_pItemShopIconMan; }
	CShopItemSearchWindow*	GetShopItemSearchWindow()		{ return m_pShopItemSearchWindow; }
	CItemSearchResultWindow* GetItemSearchResultWindow()	{ return m_pItemSearchResultWindow; }
	CSummonWindow*			GetSummonWindow()				{ return m_pSummonWindow; }
	CMapRequireCheck*		GetRequireCheck()				{ return m_pMapRequireCheckWindow; }
	CItemPreview*			GetItemPreview()				{ return m_pItemPreview; } //add itempreview
	CBasicButton*			GetpButton()					{ return pButton; }

	CBasicQuickSkillWindow*	GetBasicQuickSkillWindow()		{ return m_pBasicQuickSkillWindow; }
	CBasicQuickSkillWindowNew*	GetBasicQuickSkillWindowNew()	{ return m_pBasicQuickSkillWindowNew; }
	CFriendWindowNew*			GetFriendWindowNew()					{ return m_pFriendWindowNew; }

	CNotifyRequestIcon*		GetNotifyRequest()				{ return m_pNotifyRequest; }
	CNotifyRequestWindow*	GetNotifyRequestWindow()		{ return m_pNotifyRequestWindow; }

public:
	void	CloseAllWindow ();

public:
	BOOL	PrintMsgText ( D3DCOLOR dwColor, const char* szFormat, ... );
	BOOL	PrintConsoleText ( const char* szFormat, ... );

	BOOL	PrintMsgTextDlg ( D3DCOLOR dwColor, const char* szFormat, ... );
	BOOL	PrintConsoleTextDlg ( const char* szFormat, ... );

	BOOL	PrintMsgDlg ( D3DCOLOR dwColor, const char* szFormat, ... );

public:
	const char* MakeString ( const char* szFormat, ... );

	int		state;
//	void	SetInventoryPage();
	void	Setstate(int num)	{ state = num;}

	BOOL	SkillTabClose;
	BOOL	SkillTabOpen;

	BOOL	SkillTabCloseNew;
	BOOL	SkillTabOpenNew;

	BOOL	RotateSkillTab;
	BOOL	RotateSkillTabNew;
public:
	BOOL IsGateOpen()							{ return m_bGateOpen; }
	void SetGateOpen( BOOL bGateOpen )			{ m_bGateOpen = bGateOpen; }
	void SetDamage( D3DXVECTOR3 vPos, int nDamage, DWORD dwDamageFlag, BOOL bAttack );
	void SetTargetInfo( STARGETID sTargetID, DWORD dwBarColor, int wSchool );
	void SetTargetInfoNpc( STARGETID sTargetID );
	void SetTargetInfoPlayer( STARGETID sTargetID );
	void ResetTargetInfo()						{ HideGroup ( TARGETINFO_DISPLAY ); }
	void ResetTargetInfoNpc()						{ HideGroup ( TARGETINFO_DISPLAY_NPC ); }
	void ResetTargetInfoPlayer()						{ HideGroup ( TARGETINFO_DISPLAY_PLAYER ); }
	void SetTargetInfoCrow ( STARGETID sTargetID );
	void SetTargetInfoCrowNpc ( STARGETID sTargetID );
	void SetTargetInfoCrowPlayer ( STARGETID sTargetID );
	void ResetTargetInfoCrow()						{ HideGroup ( CROW_TARGET_INFO ); }
	void ResetTargetInfoCrowNpc()						{ HideGroup ( CROW_TARGET_INFO_NPC ); }
	void ResetTargetInfoCrowPlayer()						{ HideGroup ( CROW_TARGET_INFO_PLAYER ); }
	void ResetTargetCtfInfo()						{ HideGroup ( TARGETINFO_CTF_DISPLAY ); }

	void SetFightBegin ( const int nIndex );
	void SetFightEnd ( const int nResult );

	void SetAcademyFightBegin ( const int& nLMARK, const int& nLNUMBER, const int& nRMARK, const int& nRNUMBER );
	void SetAcademyUpdateNumber ( const int& nLNUMBER, const int& nRNUMBER );
	void SetAcademyFightEnd ();

	void SetBlockProgramFound( bool bFOUND )	{ m_bBlockProgramFound = bFOUND; }
	bool IsBlockProgramFound()					{ return m_bBlockProgramFound; }

	void SetBlockProgramAlarm( bool bAlarm )	{ m_bBlockProgramAlarm = bAlarm; }
	bool IsBlockProgramAlarm()					{ return m_bBlockProgramAlarm; }

public:	
	void	SetDialogueWindowOpen ( STARGETID sCrowID, GLCHARLOGIC* pCHAR );
	void	SetPtoPWindowOpen ( PGLCHARCLIENT pCharClient, DWORD dwGaeaID, GLCLUB & sClub );
	void	SetQuestWindowOpen ( DWORD dwQuestID );
	void	SetBusWindowOpen ( const DWORD dwGaeaID, SNpcTalk* pNpcTalk );

	void	TowerDown ( NET_MSG_GENERIC* nmg );
	void	TowerFinalSG ();
	void	TowerFinalMP ();
	void	TowerFinalPHX ();
	void	SetTradeWindowOpen ( CString szPlayerName, DWORD dwTargetID );
	void	SetTradeWindowClose ();
	void	SetStorageWindowOpen ( DWORD dwNPCID );
	void	SetStorageWindowClose ();
	void	SetClubStorageWindowOpen ();
	void	SetClubStorageWindowClose ();
	void	SetMarketWindowOpen ( SNATIVEID sNativeID );
	void	SetMarketWindowClose ();
	void	SetStorageChargeOpen ( const WORD& wPosX, const WORD& wPosY );
	void	SetItemBankWindowOpen ();
	void	SetItemShopInfo ();
	void	SetItemShopWindowOpen ();
	void	SetItemBankInfo ();
	void	SetVNGainSysWindowOpen ();
	void	SetVNGainSysInfo ();
	void	SetDefaultPosInterface(UIGUID ControlID);

	//Item Link Render 
	void	ADDITEM_RENDER_ITEMLINK ( SNATIVEID sICONINDEX, const char* szTexture );
	void	RESETITEM_RENDER_ITEMLINK ();
	void	OpenItemRebuildWindow();	// ITEMREBUILD_MARK
	void	CloseItemRebuildWindow();

	void	OpenItemGarbageWindow();	// ������
	void	CloseItemGarbageWindow();

	void	SetPetDisplay();	//add petimage
	void	ResetPetDisplay();	//add petimage

	void	SetVehicleDisplay();	//add vehicleimage
	void	ResetVehicleDisplay(); //add vehicleimage

	void	ItemPreview(int nType ,SNATIVEID sItemId ); //add itempreview

	void	OpenItemMixWindow( DWORD dwNpcID );		// ������ ����
	void	CloseItemMixWindow();
	void	SetItemMixResult( CString strMsg, bool bSuccess = false, bool bFail = false );

	void	OPEN_TAXI_WINDOW( WORD wPosX, WORD wPosY );			// �ý� ī��

	//jdev help
	void	CreateHelp();

	void	SetPrivateMarketMake ();
	void	SetPrivateMarketOpen ( const bool& bOPENER, const DWORD& dwGaeaID );
	void	GetPrivateMarketInfo ( bool& bOPENER, DWORD& dwGaeaID );
	void	SetPrivateMarketClose ();	

	void	MODAL_PRIVATE_MARKET_SELLITEM ( const bool& bCOUNTABLE, const WORD& wPosX, const WORD& wPosY );

	void	SetClubMake ( const DWORD& dwNpcID );

	void	SetShotcutText ( DWORD nID, CString& strTemp );
	void	GetShotCutKey();
	void	GetChatMacro();
	void	AddChatMacro(int nIndex);
	void	ChatLog( bool bChatLogt, int nChatLogType );
	CString GET_RECORD_CHAT();
	CString GetdwKeyToString(int dwKey);

	void	SetArmSwap( BOOL bSwap );
	
public:
	void	SetLottoOpen ( const DWORD& dwGaeaID );

public:
	void	DOMODAL_ADD_FRIEND_REQ( const CString& strName );
	void	DOMODAL_CLUB_JOIN_ASK( const DWORD& dwMasterID, const CString& strClubName, const CString& strClubMaster );
	void	DOMODAL_UNION_JOIN_ASK( const DWORD& dwMasterID, const CString& strClubMaster );
	void	DOMODAL_CLUB_BATTLE_ASK( const DWORD& dwMasterID, const CString& strClubMaster, DWORD dwBattleTime, bool bAlliance = false );
	void	DOMODAL_CLUB_BATTLE_ARMISTICE_ASK( const DWORD& dwClubID, const CString& strClubName, bool bAlliance = false );
	void	DOMODAL_CLUB_AUTHORITY_ASK( const CString& strClubName );

	void	OPEN_MODAL ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID = NO_ID, BOOL bHide = FALSE );
	void	OPEN_MODALESS ( const CString& strText, int nModalTitle, int nModalType, UIGUID CallerID = NO_ID, BOOL bHide = FALSE );
	void	CLOSE_MODAL ( const UIGUID cID, bool bMakeMsg );

public:
	DWORD GetEventQuestID()							{ return m_dwEventQuestID; }
	void SetEventQuestID( DWORD dwQuestID )			{ m_dwEventQuestID = dwQuestID; }

public:
	void	WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer = fDEFAULT_WAITTIME_LEFT );
	void	WAITSERVER_DIALOGUE_CLOSE();

public:
	void	ClearNameList ();

public:
	void	SetTradeWindowCloseReq ();

private:
	HRESULT	TEXTURE_PRE_LOAD ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT	TEXTURE_PRE_UNLOAD ();

public:
	BOOL IsInventoryWindowOpen()			{ return IsVisibleGroup( INVENTORY_WINDOW ); }
	BOOL IsStorageWindowOpen()				{ return IsVisibleGroup( STORAGE_WINDOW ); }
	BOOL IsClubStorageWindowOpen()			{ return IsVisibleGroup( CLUB_STORAGE_WINDOW ); }
	BOOL IsMarketWindowOpen()				{ return IsVisibleGroup( MARKET_WINDOW ); }
	BOOL IsDialogueWindowOpen()				{ return IsVisibleGroup( DIALOGUE_WINDOW ); }

public:
	// �������̽� ���� ĳ������ �������� ����
	BOOL IsCharMoveBlock()							{ return m_bCharMoveBlock; }
	void SetCharMoveBlock()							{ m_bCharMoveBlock = TRUE; }
	void ResetCharMoveBlock()						{ m_bCharMoveBlock = FALSE; }
	BOOL IsOpenWindowToMoveBlock();

	BOOL IsSnapItem()								{ return m_bSnapItem; }
	void SetSnapItem()								{ m_bSnapItem = TRUE; }
	void ResetSnapItem()							{ m_bSnapItem = FALSE; }

	void SetFirstItemSlot()							{ m_bFirstItemSlot = false;	}
	void ResetFirstItemSlot()						{ m_bFirstItemSlot = true; }
	bool IsFirstItemSlot()							{ return m_bFirstItemSlot; }

	void SetFriendName( const CString& strName )	{ m_strMoveFriendName = strName; }
	const CString& GetFriendName() const			{ return m_strMoveFriendName; }

	void SetThaiCCafeClass( DWORD dwClass );
	void SetMyCCafeClass( int nClass );				// �����̽þ� PC�� �̺�Ʈ
	//add sw buff
	void SetSwBonus( bool isWinner );

public:
	bool	SET_QUESTION_ITEM_ID ( int nID );
	bool	SET_PLAYERKILL_STREAK_ID ( int nID );	///add pk streak
	bool	SET_PROGRESSCTFDISPLAY_ID ( int nID );

	//add qbox
	void	SetQBoxTime ( float fTime );
	void	UpdateQBoxTime ( float fRTime );

	bool	SET_KEEP_QUESTION_ITEM_ID ( int nID );
	void	RESET_KEEP_QUESTION_ITEM ();

	void	BONUS_TIME_EVENT_START( bool bCharging ); // �̺�Ʈ ����
	void	BONUS_TIME_EVENT_END(); // �̺�Ʈ ����
	void	BONUS_TIME_BUSTER_START(); // ����ġ ����
	void	BONUS_TIME_BUSTER_END(); // ����ġ ����

	// ��Ʈ�� Ž�� ���� �ý��� ������
	void	SET_VNGAINTYPE_GAUGE( int nPos, int nLimit );

	void	SET_QUEST_HELPER( DWORD dwQuestID );
	void	RESET_QUEST_HELPER( DWORD dwQuestID );

public:
	void	SET_CONFT_CONFIRM_VISIBLE ( bool bVisible );
	void	SET_CONFT_CONFIRM_PERCENT ( float fPercent );

public:
	void SetSkillUpID( const DWORD& dwID )			{ m_dwSkillUpID = dwID; }
	DWORD GetSkillUpID()							{ return m_dwSkillUpID; }
//	BOOL ISMASTER_SKILL( SNATIVEID skill_id );

private:
	void MoveBasicInfoWindow ();
	void BasicInfoViewDoubleClick();

private:
	BOOL	IsPartyMember ( const char* szName );

public:
	void	REFRESH_QUEST_WINDOW ();

	void	REFRESH_FRIEND_LIST ();
	void	REFRESH_FRIEND_STATE();

	void	REFRESH_CLUB_LIST ();
	void	REFRESH_CLUB_STATE ();
	void	REFRESH_CLUB_NOTICE();
	void	REFRESH_CLUB_ALLIANCE_LIST();
	void	REFRESH_CLUB_BATTLE_LIST();

	void	REFRESH_ITEMBANK ();
	void	REFRESH_VNGAINSYSTEM ();

	void	REFRESH_ATTENDBOOK();

	void	SET_GATHER_GAUGE( DWORD dwGaeaID, float fTime );

public:
	void	SetInventorySlotViewSize ( const int& nSLOT_VIEW_SIZE );
	int GetInventorySlotViewSize () const;

public:
	void ADD_FRIEND_NAME_TO_EDITBOX( const CString& strName );
	void ADD_FRIEND( const CString& strName );
	void FRIEND_LIST( CONST CString & strName, bool bOnline );

	const CString& GetPrivateMarketTitle ();

	void DoBattleModal( const CString strClubName, bool bAlliance = false );
	BOOL ReqClubBattle( DWORD dwTime, bool bAlliance );

private:
	ET_INFOTYPE		m_etInfoType;

public:
	void	RESET_INFO ();
	void	RESET_INFO_LINK ();
	void	CLEAR_INFO_LINK ();
	void	SET_INFO_LINK ( SITEMCUSTOM sITEM );
	void	SHOW_ITEM_INFO_LINK ( SITEMCUSTOM &sItemCustom );	

	void	SHOW_ITEM_INFO_SIMPLE ( SITEMCUSTOM &sItemCustom );
	void	SHOW_ITEM_INFO ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID = SNATIVEID() );	
	void	SHOW_SKILL_INFO ( SNATIVEID sNativeID, const BOOL bNextLevel );
	void	SHOW_COMMON_LINEINFO ( const CString& strText, D3DCOLOR dwColor );
	void	SHOW_COMMON_LINEINFO_SPLIT ( const CString& strText, D3DCOLOR dwColor );

	bool	BEGIN_COMMON_LINEINFO_MULTI ();
	bool	ADD_COMMON_LINEINFO_MULTI ( const CString& strText, D3DCOLOR dwColor );
	void	END_COMMON_LINEINFO_MULTI ();

	void	WARNING_MSG_ON()			{ ShowGroupBottom( WARNING_MSG_WINDOW ); }
	void	WARNING_MSG_OFF()			{ HideGroup( WARNING_MSG_WINDOW ); }

	void	SET_COUNT_MSG( INT nCount );

public:
	void	CLEAR_TEXT ();
	void	REPOS_CONTROL ( int x, int y );
	int		ADDTEXT_NOSPLIT ( const CString& strText, const D3DCOLOR& dwColor );
	int		ADDTEXT_NOSPLIT_BIG ( const CString& strText, const D3DCOLOR& dwColor );
	void	ADDTEXT_MULTICOLOR_SPLIT (int nIndex, const CString& strText, const D3DCOLOR& dwColor );
	int		ADDTEXT ( const CString& strText, const D3DCOLOR& dwColor );
	void	ADDSTRING ( int nIndex, const CString& strText, const D3DCOLOR& dwColor );
//	void	ADDTEXT_NOSPLIT7 ();												//IconResistant by BlackCatYB
	void	ADDTEXT_LONGESTLINE_SPLIT ( const CString& strText, const D3DCOLOR& dwColor );

public:
	void	ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg );	
	void	MsgProcess ( LPVOID msgBuffer );

public:
	const	D3DXVECTOR3&	GetCharDir () const				{ return m_vCharDir; }

public:
	void	SetAniPic(int nPic);
	void	SetOddEven(BOOL bOdd);

public:
	void	DisplayChatMessage ( int nType, const char *szName, const char *szMsg );
	void	DisplayChatMessage ( int nType, const char *szName, const char *szMsg, SITEMLINK sLINK );
	void	UpdateClubBattleTime( float fClubBattleTime );
	void	UpdateClubBattleTime2( float fClubBattleTime2, bool bTW );
	void	UpdateSchoolWarBattleTime( float fSchoolWarsTime );
	void	UpdateRoyalRumbleBattleTime( float fRoyalRumbleTime );
	void	ClearItemBank();
	void	ClearItemShop ();
	void	ClearVNGainSys();
	void	DisableMinimapTarget();

private:
	void	UpdateStatus ();
	void	UpdateShortcutBefore ();
	void	UpdateShortcutAfter ();

private:
	void	UpdateStateQuestAlarm ();
	void	UpdateStateSimpleHP ();
	void	UpdateSimpleMessage ();
public:
	void	ReqToggleRun ();
	bool	ItemShopAuth ();		// �Ϻ� Ŀ���� ������ ���� ��� //	ItemShopAuth
	void	ItemShopVisible();		// �Ϻ� Ŀ���� ������ �ε� ��� //	ItemShopAuth


	void    ViewpropertiseNetTo(NET_MSG_GENERIC* nmg );//add charinfoview
	void	VisibleCDMRanking( bool bVisible );
	void	RefreashCDMRanking();
	//add school wars
	void	VisibleSWRanking( bool bVisible );
	void	VisibleSWPRanking( bool bVisible );
	void	RefreshSWRanking();
	void	RefreshSWPRanking();

	void	VisibleRRPRanking( bool bVisible );
	void	RefreshRRPRanking();

	void	VisibleCTFRanking( bool bVisible );
	void	VisibleCtfInfoDisplay( bool bVisible );
	void	RefreshCTFRanking();
	void	RefreshCtfIcon( bool bVecSw );
//#ifdef CH_PARAM // �߱� �������̽� ����
//public:
//	void UpdatePotionTrayPosition();
//#endif

public:	//	ETC Func.
	SCONFT_OPTION*	GetConftOption ()			{ return &m_sReqConflictOption; }
	SPARTY_OPT*		GetPartyOption ()			{ return &m_sReqPartyOption; }

	void			SetPartyOption( SPARTY_OPT sOpt )			{ m_sReqPartyOption = sOpt; }
	void			SetFriendName2( CString strName )			{ m_strFriendName = strName; }
	void			SetClubMasterID( DWORD dwID )				{ m_dwClubMasterID = dwID; }

public:
	CItemRebuild* 		GetItemRebuild() 	{ return m_pItemRebuildWindow; 	} //sealed card NaJDeV

public:
	//SITEMCUSTOM		m_sItemInfoTemp;
	SITEMCLIENT		m_sItemInfoTemp2;
	void SetItemInfoTemp2( const SITEMCLIENT& sID )		{ m_sItemInfoTemp2 = sID; }
	SITEMCLIENT GetItemInfoTemp2()						{ return m_sItemInfoTemp2; }
	void SetItemInfoTemp( const SITEMCUSTOM& sID )		{ m_sItemInfoTemp = sID; }
	SITEMCUSTOM GetItemInfoTemp()						{ return m_sItemInfoTemp; }

public:
	bool IsCHANNEL() { return m_bCHANNEL; }
	bool IsCHAT_BEGIN();

private:
	BOOL			m_bGateOpen;
	BOOL			m_bCharMoveBlock;
	bool			m_bBlockProgramFound;
	bool			m_bBlockProgramAlarm;
	bool			m_bITEM_INFO_EX_DISPLAY_MODE;
	bool			m_bITEM_INFO_LINK_DISPLAY_MODE;
	DWORD			m_dwEventQuestStep;
	DWORD			m_dwEventQuestID;
	EMCONFT_TYPE	m_emConflictReqType;
	DWORD			m_ConflictReqID;
	SCONFT_OPTION	m_sReqConflictOption;
	SPARTY_OPT		m_sReqPartyOption;
	BOOL			m_bSnapItem;
	BOOL			m_bPartyStateBack;
	CString			m_strFriendName;
	D3DXVECTOR3		m_vCharDir;
	DWORD			m_dwSkillUpID;
	SITEMCUSTOM		m_sItemInfoTemp;
	SITEMCUSTOM		m_sItemInfoLinkTemp;
	bool			m_bUSING_INFO_DISPLAY;
	bool			m_bACADEMY_FIGHT_READY;
	CString			m_strGeneralInfoBack;
	float			m_fMoveWindowDistBack;
	bool			m_bFirstItemSlot;
	DWORD			m_dwClubMasterID;
	CString			m_strMoveFriendName;
	bool			m_bCHANNEL;
	bool			m_bTabReserve;
	float			m_fVehicleDelay;
	float			m_fItemBankDelay;
	float			m_fItemShopDelay;
	bool			m_bItemShopLoad;	//	ItemShopAuth

public:
	static CInnerInterface& GetInstance();

public:
	void	OpenCharacterPage();
	void	OpenVehiclePage();
	void	RefreshVehiclePage();
	void	OpenPetPage();
	void	HidePetPage();
public:
	bool	PreviewCheckSimple( SNATIVEID sID );
	bool	PreviewCheckWear( SNATIVEID sID );
	void	PreviewItem( SITEMCUSTOM sItem );
	void	LinkItem( SITEMCUSTOM* pItem );

	SNATIVEID		m_sSKILLID;
	SNATIVEID		GetSKEFFID() { return m_sSKILLID; }
	void			SetSKEFFID(SNATIVEID sID) { m_sSKILLID = sID; }

	void ADDITEM_RENDER ( SNATIVEID sICONINDEX, const char* szTexture ); 
	void RESETITEM_RENDER (); 
public:
	BOOL	m_bDispMember;
	BOOL	m_bDispBuff;

	void	DisplayMember(BOOL bDisp)	{ m_bDispMember = bDisp;	}
	void	DisplayBuff(BOOL bDisp)		{ m_bDispBuff = bDisp;		}

	BOOL	IsMemberDisp()				{ return m_bDispMember;		}
	BOOL	IsBuffDisp()				{ return m_bDispBuff;		}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Box Item Image Render by CNDev
	SITEMCUSTOM sBOXINFOCUSTOM[ITEM::SBOX::ITEM_SIZE];
	SITEMCUSTOM sRANDOMBOXINFOCUSTOM[30];

	void	ADDNAME_RENDER_BOXINFO ( const CString& strText, const D3DCOLOR& dwColor );
	void	ADDITEM_RENDER_BOXINFO ( SNATIVEID sICONINDEX, const char* szTexture );
	void	RESETITEM_RENDER_BOXINFO ();
	void	ADDITEMBOX_RENDER_BOXINFO ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	RESETITEMBOX_RENDER_BOXINFO ();
	void	SETRESIICON_BOXINFO();
	void	RESETRESIICON_BOXINFO();
	void	ADDITEMBOXRANDOM_RENDER_BOXINFO ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	RESETITEMBOXRANDOM_RENDER_BOXINFO ();
	void	RESET_INFO_BOXINFO();
	void	SHOW_BOXINFO ( SITEMCUSTOM &sItemCustom, BOOL bShopOpen, BOOL bInMarket, BOOL bInPrivateMarket, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID = SNATIVEID() );	
	void	ADDTEXT_LONGESTLINE_SPLIT_BOXINFO ( const CString& strText, const D3DCOLOR& dwColor );
	void	ADDTEXT_NOSPLIT_BOXINFO ( const CString& strText, const D3DCOLOR& dwColor );
	//End of Box Item Image Render by CNDev
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Box Item Render through hover by CNDev
	void	ADDITEMBOX_RENDER_INFO ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	RESETITEMBOX_RENDER_INFO ();
	void	ADDITEMBOXRANDOM_RENDER_INFO ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void	RESETITEMBOXRANDOM_RENDER_INFO ();
	/////////////////////
	//Multi Color
	void ADDTEXT_MULTICOLORSPLIT ( const CString& strText, const D3DCOLOR& dwColor, const CString& strText2 ,const D3DCOLOR& dwColor2 );
	void ADDTEXT_MULTICOLORSPLIT2 ( const CString& strText, const D3DCOLOR& dwColor , const CString& strText2 , const D3DCOLOR& dwColor2 , const CString& strText3, const D3DCOLOR& dwColor3 , const CString& strText4 , const D3DCOLOR& dwColor4 );
	void ADDTEXT_MULTICOLORSPLIT3 ( const CString& strText, const D3DCOLOR& dwColor , const CString& strText2 , const D3DCOLOR& dwColor2 , const CString& strText3, const D3DCOLOR& dwColor3 , const CString& strText4 , const D3DCOLOR& dwColor4 , const CString& strText5 , const D3DCOLOR& dwColor5 );
	void ADDTEXT_MULTICOLORSPLIT4 ( const CString& strText, const D3DCOLOR& dwColor , const CString& strText2 , const D3DCOLOR& dwColor2 , const CString& strText3, const D3DCOLOR& dwColor3 , const CString& strText4 , const D3DCOLOR& dwColor4 );
	void	ADDTEXT_MULTICOLORSPLIT5( const CString& strText, const D3DCOLOR& dwColor , const CString& strText2 , const D3DCOLOR& dwColor2 , const CString& strText3 , const D3DCOLOR& dwColor3 , const CString& strText4 , const D3DCOLOR& dwColor4 , const CString& strText5 , const D3DCOLOR& dwColor5 );
	void	ADDTEXT_MULTICOLORSPLIT6( const CString& strText, const D3DCOLOR& dwColor , const CString& strText2 , const D3DCOLOR& dwColor2 , const CString& strText3 , const D3DCOLOR& dwColor3 , const CString& strText4 , const D3DCOLOR& dwColor4 , const CString& strText5 , const D3DCOLOR& dwColor5, const CString& strText6 , const D3DCOLOR& dwColor6);
	void	ADDTEXT_MULTICOLORSPLIT33 ( const CString& strText, const D3DCOLOR& dwColor , const CString& strText2 , const D3DCOLOR& dwColor2 , const CString& strText3, const D3DCOLOR& dwColor3 );

};
