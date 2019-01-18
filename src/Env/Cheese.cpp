#include "Cheese.hpp"
#include <Utility/Utility.hpp>
#include <Application.hpp>

Cheese::Cheese(const Vec2d& position)
	: SimulatedEntity(position, getAppConfig().cheese_initial_energy) {}

Cheese::Cheese()
	: SimulatedEntity({0.0, 0.0}, getAppConfig().cheese_initial_energy) {}

//----------------------------------------------------------------------

double Cheese::getRadius() const {
	return (energie/2);
}

double Cheese::getInitialRadius() const {
	return (getAppConfig().cheese_initial_energy/2);
}

sf::Texture& Cheese::getTexture() const {
	return getAppTexture(getAppConfig().cheese_texture);
}

bool Cheese::canBeTakenEnergy() const {
	return true;
}

bool Cheese::isSolitary() const {
	return false;
}

bool Cheese::eatable(SimulatedEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Cheese::eatableBy(Mouse const* mouse) const {
	return true;
}

bool Cheese::eatableBy(Cheese const* cheese) const {
	return false;
}
