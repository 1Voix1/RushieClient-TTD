#include <engine/graphics.h>
#include <engine/shared/config.h>
#include <game/client/gameclient.h>
#include <game/client/render.h>
#include <game/client/ui.h>
#include <generated/protocol.h>

#include <game/client/components/camera.h>
#include <game/client/components/chat.h>
#include <game/client/components/console.h>
#include <game/client/components/controls.h>
#include <game/client/components/menus.h>

#include "edgehelper.h"
#include <game/client/components/emoticon.h>

#include "game/localization.h"
struct SEdgeHelperProperties
{
	static constexpr float ms_FontSize = 8.0f;
	static constexpr float ms_IconFontSize = 11.0f;
	static constexpr float ms_Padding = 3.0f;
	static constexpr float ms_Rounding = 3.0f;

	static constexpr float ms_ItemSpacing = 2.0f;
	static constexpr float ms_GroupSpacing = 5.0f;

	static constexpr float ms_CubeSize = 24.0f;
	static constexpr float ms_WallWidth = 3.0f;
	static constexpr float ms_CircleRadius = 10.0f;

	static constexpr float ms_HeadlineFontSize = 8.0f;

	static ColorRGBA WindowColor() { return ColorRGBA(0.451f, 0.451f, 0.451f, 0.9f); };
	static ColorRGBA WindowColorDark() { return ColorRGBA(0.2f, 0.2f, 0.2f, 0.9f); };
	static ColorRGBA GeneralButtonColor() { return ColorRGBA(0.541f, 0.561f, 0.48f, 0.8f); };
	static ColorRGBA GeneralActiveButtonColor() { return ColorRGBA(0.53f, 0.78f, 0.53f, 0.8f); };

	static ColorRGBA ActionGeneralButtonColor() { return ColorRGBA(0.541f, 0.561f, 0.48f, 0.8f); };
	static ColorRGBA ActionActiveButtonColor() { return ColorRGBA(0.53f, 0.78f, 0.53f, 0.8f); };
	static ColorRGBA ActionAltActiveButtonColor() { return ColorRGBA(1.0f, 0.42f, 0.42f, 0.8f); };
	static ColorRGBA BlueSteelButtonColor() { return ColorRGBA(0.2f, 0.4f, 0.65f, 0.8f); };

	static ColorRGBA TeamsGeneralButtonColor() { return ColorRGBA(0.32f, 0.32f, 0.72f, 0.8f); };
	static ColorRGBA TeamsActiveButtonColor() { return ColorRGBA(0.31f, 0.52f, 0.78f, 0.8f); };

	static ColorRGBA ActionSpecButtonColor() { return ColorRGBA(0.2f, 1.0f, 0.2f, 0.8f); }; // Bright green color for spec

	static ColorRGBA ActionBanAltButtonColor() { return ColorRGBA(0.7f, 0.1f, 0.1f, 0.8f); };
	static ColorRGBA ActionBanButtonColor() { return ColorRGBA(1.0f, 0.24f, 0.24f, 0.8f); };
	static ColorRGBA ActionKillAltButtonColor() { return ColorRGBA(0.5f, 0.45f, 0.0f, 0.8f); };
	static ColorRGBA ActionKillButtonColor() { return ColorRGBA(1.0f, 0.902f, 0.0f, 0.8f); };
	static ColorRGBA ActionKickAltButtonColor() { return ColorRGBA(0.5f, 0.27f, 0.0f, 0.8f); };
	static ColorRGBA ActionKickButtonColor() { return ColorRGBA(1.0f, 0.549f, 0.0f, 0.8f); };
	static ColorRGBA ActionMuteAltButtonColor() { return ColorRGBA(0.25f, 0.0f, 0.25f, 0.8f); };
	static ColorRGBA ActionMuteButtonColor() { return ColorRGBA(0.502f, 0.0f, 0.502f, 0.8f); };
};

CEdgeHelper::CEdgeHelper()
{
	OnReset();
}

void CEdgeHelper::OnConsoleInit()
{
	Console()->Register("toggle_edgehelper", "", CFGFLAG_CLIENT, ConToggleEdgeHelper, this, "Toggle admin panel");
}

void CEdgeHelper::ConToggleEdgeHelper(IConsole::IResult *pResult, void *pUserData)
{
	CEdgeHelper *pSelf = (CEdgeHelper *)pUserData;
	pSelf->SetActive(!pSelf->IsActive());
}

void CEdgeHelper::SetActive(bool Active)
{
	if(m_Active == Active)
		return;

	m_Active = Active;
	if(m_Active)
	{
	}
	else
	{
		OnReset();
	}
}

void CEdgeHelper::OnReset()
{
	RIReset();
	SetActive(false);
}

void CEdgeHelper::OnRelease()
{
	RIReset();
	SetActive(false);
}

void CEdgeHelper::OnRender()
{
	if(!IsActive())
		return;

	RenderEdgeHelper();
}

void CEdgeHelper::RenderEdgeHelper()
{
	CUIRect Base, Label, EdgeInfo, JumpInfo;

	Base.h = 100.0f * 3.0f / 6;
	Base.w = 100.0f * 3.0f * Graphics()->ScreenAspect() / 5;
	Base.x = 100.0f * 3.0f * Graphics()->ScreenAspect() / 2 - Base.w / 2;
	Base.y = (100.0f * 3.0f) / 2 + 16.0f;

	vec2 ScreenTL, ScreenBR;
	Graphics()->GetScreen(&ScreenTL.x, &ScreenTL.y, &ScreenBR.x, &ScreenBR.y);

	if(Base.y + Base.h > ScreenBR.y)
	{
		Base.y -= Base.y + Base.h - ScreenBR.y;
	}
	if(Base.x + Base.w > ScreenBR.x)
	{
		Base.x -= Base.x + Base.w - ScreenBR.x;
	}

	m_Rect = Base;

	Base.Draw(SEdgeHelperProperties::WindowColorDark(), IGraphics::CORNER_ALL, SEdgeHelperProperties::ms_Rounding);
	Base.Margin(SEdgeHelperProperties::ms_Padding, &Base);
	const int ClientId = GameClient()->m_Snap.m_SpecInfo.m_Active ? GameClient()->m_Snap.m_SpecInfo.m_SpectatorId : GameClient()->m_Snap.m_LocalClientId;
	m_Pos_x = GetPositionEdgeHelper(ClientId, g_Config.m_ClDummy);

	Base.HSplitMid(&EdgeInfo, &JumpInfo);
	RenderEdgeHelperEdgeInfo(&EdgeInfo);
	RenderEdgeHelperJumpInfo(&JumpInfo);
}

float CEdgeHelper::GetPositionEdgeHelper(int ClientId, int Conn)
{
	float ValuePos;
	if(ClientId == SPEC_FREEVIEW)
	{
		ValuePos = GameClient()->m_Camera.m_Center.x / 32.0f;
	}
	else if(GameClient()->m_aClients[ClientId].m_SpecCharPresent)
	{
		ValuePos = GameClient()->m_aClients[ClientId].m_SpecChar.x / 32.0f;
	}
	else
	{
		const CNetObj_Character *pPrevChar = &GameClient()->m_Snap.m_aCharacters[ClientId].m_Prev;
		const CNetObj_Character *pCurChar = &GameClient()->m_Snap.m_aCharacters[ClientId].m_Cur;
		const float IntraTick = Client()->IntraGameTick(Conn);
		ValuePos = mix(vec2(pPrevChar->m_X, pPrevChar->m_Y), vec2(pCurChar->m_X, pCurChar->m_Y), IntraTick).x / 32.0f;
	}

	ValuePos = std::round(ValuePos * 100.0f) / 100.0f;
	float temp = std::round(ValuePos * 100.0f); // temp = 3263.0f
	int result = static_cast<int>(temp) % 100; // result = 3263 % 100 = 63
	return result;
}

void CEdgeHelper::RenderEdgeHelperEdgeInfo(CUIRect *pBase)
{
	CUIRect Label, LeftZone, RightZone, CenterZone;
	pBase->HSplitBottom(SEdgeHelperProperties::ms_ItemSpacing, pBase, nullptr);
	float ActionSpacing = (pBase->w - (2 * (SEdgeHelperProperties::ms_WallWidth + SEdgeHelperProperties::ms_CircleRadius + SEdgeHelperProperties::ms_CubeSize))) / 4;
	pBase->VSplitLeft(SEdgeHelperProperties::ms_CubeSize + ActionSpacing, &LeftZone, &CenterZone);
	CenterZone.VSplitRight(SEdgeHelperProperties::ms_CubeSize + ActionSpacing, &CenterZone, &RightZone);
	LeftZone.VSplitRight(ActionSpacing + 2, &LeftZone, nullptr);
	RightZone.VSplitLeft(ActionSpacing + 2, nullptr, &RightZone);
	LeftZone.Margin(SEdgeHelperProperties::ms_ItemSpacing, &LeftZone);
	RightZone.Margin(SEdgeHelperProperties::ms_ItemSpacing, &RightZone);
	LeftZone.Draw(m_Pos_x >= 44 ? SEdgeHelperProperties::BlueSteelButtonColor() : m_Pos_x >= 28 ? SEdgeHelperProperties::ActionActiveButtonColor() : SEdgeHelperProperties::ActionAltActiveButtonColor(), IGraphics::CORNER_NONE, 0);
	RightZone.Draw(m_Pos_x <= 53 ? SEdgeHelperProperties::BlueSteelButtonColor() : m_Pos_x <= 69 ? SEdgeHelperProperties::ActionActiveButtonColor() : SEdgeHelperProperties::ActionAltActiveButtonColor(), IGraphics::CORNER_NONE, 0);
	CenterZone.VSplitLeft(SEdgeHelperProperties::ms_WallWidth + ActionSpacing, &LeftZone, &CenterZone);
	CenterZone.VSplitRight(SEdgeHelperProperties::ms_WallWidth + ActionSpacing, &CenterZone, &RightZone);
	LeftZone.VSplitRight(ActionSpacing - 3, &LeftZone, nullptr);
	LeftZone.VSplitLeft(3, nullptr, &LeftZone);
	RightZone.VSplitLeft(ActionSpacing - 3, nullptr, &RightZone);
	RightZone.VSplitRight(3, &RightZone, nullptr);
	LeftZone.Draw(SEdgeHelperProperties::WindowColor(), IGraphics::CORNER_NONE, 0);
	RightZone.Draw(SEdgeHelperProperties::WindowColor(), IGraphics::CORNER_NONE, 0);
	CenterZone.Margin(SEdgeHelperProperties::ms_ItemSpacing, &CenterZone);
	CenterZone.Draw(SEdgeHelperProperties::WindowColor(), IGraphics::CORNER_ALL, CenterZone.w/2);
}

void CEdgeHelper::RenderEdgeHelperJumpInfo(CUIRect *pBase)
{
	CUIRect Label, Line;
	pBase->HSplitTop(1, &Line, pBase);
	Line.Draw(SEdgeHelperProperties::WindowColor(), IGraphics::CORNER_NONE, 0);
	pBase->HSplitTop(SEdgeHelperProperties::ms_ItemSpacing, nullptr, pBase);
	pBase->HSplitTop(SEdgeHelperProperties::ms_HeadlineFontSize, &Label, pBase);
	char aBuf[64];
	str_format(aBuf, sizeof(aBuf), "Pos: %02i", m_Pos_x);
	Ui()->DoLabel(&Label, aBuf, 12, TEXTALIGN_MC);
	pBase->HSplitTop(5, nullptr, pBase);
}

bool CEdgeHelper::IsActive() const
{
	if(m_Active)
		return true;

	return false;
}
