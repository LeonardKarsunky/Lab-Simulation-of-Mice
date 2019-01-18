#include "SimulatedEntity.hpp"
#include <Random/Random.hpp>
#include <cmath>
#include <Config.hpp>
#include <Application.hpp>
#include <Env/Box.hpp>
#include <Env/Mouse.hpp>
#include <Env/Cheese.hpp>
#include <Utility/Constants.hpp>

SimulatedEntity::SimulatedEntity(const Vec2d& position, Quantity energie)
	: position(position),
	  orientation(uniform(0.0, TAU)),
	  boite(nullptr),
	  age(sf::Time::Zero),
	  energie(energie) {}

SimulatedEntity::~SimulatedEntity() {}

//----------------------------------------------------------------------

Vec2d SimulatedEntity::getCenter() const {
	return position;
}

Vec2d SimulatedEntity::getPosition() const {
	return position;
}

sf::Time SimulatedEntity::getAge() const {
	return age;
}

Quantity SimulatedEntity::getEnergy() const {
	return energie;
}

sf::Time SimulatedEntity::getLongevity() const {
	sf::Time longevite = sf::seconds(1E+9);
	return longevite;
}

void SimulatedEntity::setCenter(const Vec2d& center) {
	position.x = center.x;
	position.y = center.y;
}

void SimulatedEntity::setCenterX(double x) {
	position.x = x;
}

void SimulatedEntity::setCenterY(double y) {
	position.y = y;
}

void SimulatedEntity::setOrientation(Angle angle) {
	orientation = angle;
}

void SimulatedEntity::setRotation(const Angle& angle) {
	orientation = angle;
}

Quantity SimulatedEntity::provideEnergy(Quantity qte) {
	if (canBeTakenEnergy()) {
		if (energie <= qte) {
			energie = 0.0;
			return energie;
		} else {
			energie -= qte;
			return energie;
		}
	}
}

void SimulatedEntity::update(sf::Time dt) {
	if ((age + dt) <= getLongevity()) {
		age += dt;
	} else {
		energie = 0.0;
	}
}

void SimulatedEntity::drawOn(sf::RenderTarget& target) const {
	sf::Sprite entitySprite(buildSprite(getCenter(), (getRadius() * 2), getTexture()));
	entitySprite.setRotation(orientation / DEG_TO_RAD); 
	target.draw(entitySprite);
	
	if (isDebugOn()) {
		Collider::drawOn(target);
			
		/*! Si l'on veut afficher l'énergie plus en avant :
		Vec2d text_center(getCenter());
		Vec2d heading(Vec2d::fromAngle(orientation));
		text_center.x += heading.x * 130;
		text_center.y += heading.y * 130;*/
		auto text = buildText(to_nice_string(getEnergy()),
							  getCenter(),
							  getAppFont(),
							  getAppConfig().default_debug_text_size,
							  getAppConfig().debug_text_color);
		text.setRotation(orientation / DEG_TO_RAD + 90);
		target.draw(text);
	}
}

Box* SimulatedEntity::getBox() const {
	return boite;
}

void SimulatedEntity::setBox(Box* box) {
	boite = box;
}

void SimulatedEntity::confine(Box* box) {
	boite = box;
}

void SimulatedEntity::removeBox() {
	if (boite != nullptr) {
		boite = nullptr;
	}
}

void SimulatedEntity::resetBox() {
	boite->reset();
}

bool SimulatedEntity::canBeConfinedIn(Box* box) const {
	if (box->isPositionInside(getCenter())) {
		if ((box->isEmpty() and (isSolitary())) or (not(isSolitary()))) {
			return true;
		}
	} else { 
		return false;
	}
	
	return false;
}

void SimulatedEntity::placeEntity(Box* box) {
	auto topWall(box->getTopLimit(true));
	auto bottomWall(box->getBottomLimit(true));
	auto leftWall(box->getLeftLimit(true));
	auto rightWall(box->getRightLimit(true));
	double radius(getRadius());
	
	if (getCenter().y - radius < topWall) {
		setCenterY(topWall + radius * 1.25);
	}
	if (getCenter().y + radius > bottomWall) {
		setCenterY(bottomWall - radius * 1.25);				
	}
	if (getCenter().x - radius < leftWall) {
		setCenterX(leftWall + radius * 1.25);
	}
	if (getCenter().x + radius > rightWall) {
		setCenterX(rightWall - radius * 1.25);
	}
	
	setBox(box);
}

double SimulatedEntity::getDistance(const Vec2d& position_other) const {
	return (sqrt(position_other.lengthSquared()));
}
