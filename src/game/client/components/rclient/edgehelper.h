#ifndef GAME_CLIENT_COMPONENTS_RCLIENT_EDGEHELPER_H
#define GAME_CLIENT_COMPONENTS_RCLIENT_EDGEHELPER_H

#include <game/client/component.h>

#include <engine/console.h>
class CEdgeHelper : public CComponent
{
	bool m_Active = false;

	static void ConToggleEdgeHelper(IConsole::IResult *pResult, void *pUserData);

	CUIRect m_Rect;

	void RIReset()
	{
	}

	void RenderEdgeHelper();

public:
	CEdgeHelper();
	int Sizeof() const override { return sizeof(*this); }

	void SetActive(bool Active);

	void OnReset() override;
	void OnRender() override;
	void OnConsoleInit() override;
	void OnRelease() override;

	bool IsActive() const;
};

#endif
