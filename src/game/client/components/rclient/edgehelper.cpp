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
struct SPopupProperties
{
	static constexpr float ms_FontSize = 8.0f;
	static constexpr float ms_IconFontSize = 11.0f;
	static constexpr float ms_Padding = 6.0f;
	static constexpr float ms_Rounding = 3.0f;

	static constexpr float ms_ItemSpacing = 2.0f;
	static constexpr float ms_GroupSpacing = 5.0f;

	static constexpr float ms_ButtonHeight = 12.0f;

	static constexpr float ms_HeadlineFontSize = 8.0f;

	static ColorRGBA WindowColor() { return ColorRGBA(0.451f, 0.451f, 0.451f, 0.9f); };
	static ColorRGBA WindowColorDark() { return ColorRGBA(0.2f, 0.2f, 0.2f, 0.9f); };
	static ColorRGBA GeneralButtonColor() { return ColorRGBA(0.541f, 0.561f, 0.48f, 0.8f); };
	static ColorRGBA GeneralActiveButtonColor() { return ColorRGBA(0.53f, 0.78f, 0.53f, 0.8f); };

	static ColorRGBA ActionGeneralButtonColor() { return ColorRGBA(0.541f, 0.561f, 0.48f, 0.8f); };
	static ColorRGBA ActionActiveButtonColor() { return ColorRGBA(0.53f, 0.78f, 0.53f, 0.8f); };
	static ColorRGBA ActionAltActiveButtonColor() { return ColorRGBA(1.0f, 0.42f, 0.42f, 0.8f); };

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
	CUIRect Base, Label;

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

	Base.Draw(SPopupProperties::WindowColorDark(), IGraphics::CORNER_ALL, SPopupProperties::ms_Rounding);
	Base.Margin(SPopupProperties::ms_Padding, &Base);

	Base.HSplitTop(SPopupProperties::ms_HeadlineFontSize, &Label, &Base);
	char aBuf[64];
	float OriginalValue = GameClient()->m_Camera.m_Center.x/32.0f;
	float FractionalPart = OriginalValue - floorf(OriginalValue);
	float ShiftedValue = FractionalPart * 100.0f;
	float RoundedValue = ceilf(ShiftedValue);
	str_format(aBuf, sizeof(aBuf), "Pos: %i", static_cast<int>(floorf(RoundedValue)));
	Ui()->DoLabel(&Label, aBuf, 12, TEXTALIGN_MC);
	Base.HSplitTop(5, nullptr, &Base);
}

bool CEdgeHelper::IsActive() const
{
	if(m_Active)
		return true;

	return false;
}
