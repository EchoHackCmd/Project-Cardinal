#pragma once
#include "../../Client.h"
#include "../Module.h"

class TabGui : public Module {
public:
	TabGui(class Client* i, class Category* c) : Module(i, c, "TabGui", VK_TAB) {
		this->isEnabled = true;
	};

	void onRender(class Renderer*);
	void renderLogo(class Renderer*);
	void updateAlpha();
	void onKey(uint64_t, bool, bool*);

	int sCIndex; /* Selected Category Index */
	int sMIndex; /* Selected Module Index */

	bool sCat = false;
	bool sMod = false;

	float alpha = 0.f;
};