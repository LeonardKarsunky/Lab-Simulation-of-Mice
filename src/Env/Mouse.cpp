#include "Mouse.hpp"
#include <Utility/Utility.hpp>
#include <Application.hpp>

Mouse::Mouse(const Vec2d& position)
	: Animal(position, getAppConfig().mouse_energy_initial) {}

//----------------------------------------------------------------------

double Mouse::getRadius() const {
	return ((getAppConfig().mouse_size)/2);
}

double Mouse::getInitialRadius() const {
	return ((getAppConfig().mouse_size)/2);
}

double Mouse::getMaxSpeed() const {
	if (energie < getAppConfig().animal_min_energy) {
		return (getAppConfig().mouse_max_speed/2);
	} else {
		return getAppConfig().mouse_max_speed;
	}
}

double Mouse::getEnergyLossFactor() const {
	return getAppConfig().mouse_energy_loss_factor;
}

double Mouse::getMass() const {
	return getAppConfig().mouse_mass;
}

Quantity Mouse::getBite() const {
	return getAppConfig().mouse_energy_bite;
}

sf::Time Mouse::getLongevity() const {
	return getAppConfig().mouse_longevity;
}

sf::Texture& Mouse::getTexture() const {
	return getAppTexture(getAppConfig().mouse_texture_white);
}

bool Mouse::eatable(SimulatedEntity const* entity) const {
	return entity->eatableBy(this);
}

bool Mouse::eatableBy(Mouse const* mouse) const {
	return false;
}

bool Mouse::eatableBy(Cheese const* cheese) const {
	return false;
}

double Mouse::getViewRange() const {
	return getAppConfig().mouse_view_range;
}

double Mouse::getViewDistance() const {
	return getAppConfig().mouse_view_distance;
}
