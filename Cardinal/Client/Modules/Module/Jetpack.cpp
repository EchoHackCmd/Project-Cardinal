#include "Jetpack.h"
#include "../../Client.h"

void Jetpack::onLoop() {
	if (key == NULL) key = 0x46;
	this->isEnabled = client->keyMap[key];

	if (!everythingIsValid() || !instance->minecraftGame()->canUseKeys) this->isEnabled = false;
}

void Jetpack::onGmTick(GameMode* GM) {
	if (!everythingIsValid())
		return;

	if (!instance->minecraftGame()->canUseKeys)
		return;

	auto rot = *player->bodyRot();
	Vec2 angles = Vec2((rot.x) * -(PI / 180.f), (rot.y + 90.0f) * (PI / 180.f));
	*player->velocity() = Vec3(cos(angles.y) * cos(angles.x) * speed, sin(angles.x) * speed, sin(angles.y) * cos(angles.x) * speed);
}