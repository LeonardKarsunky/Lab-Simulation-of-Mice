#include "Collider.hpp"
#include <iostream>
#include <Utility/Utility.hpp>

bool Collider::isColliding (const Collider& other) const {
	if (distance(getCenter(), other.getCenter()) <= (getRadius() + other.getRadius())) {
		return true;
	}
	return false;
}

bool Collider::isColliding(const Vec2d& centre, double radius) const {
	if (distance(getCenter(), centre) <= (getRadius() + radius)) {
		return true;
	}
	return false;
}

bool Collider::isPointInside (const Vec2d& point) const {
	if (distance(point, getCenter()) <= getRadius()) {
		return true;
	}
	return false;
}
void Collider::drawOn(sf::RenderTarget& target) const {
	auto circle(buildCircle(getCenter(), getRadius(), sf::Color(20,150,20,30)));
	target.draw(circle);
}

//----------------------------------------------------------------------

bool operator|(const Collider& body1, const Collider& body2) {
	if (body1.isColliding(body2)) {
		return true;
	}
	return false;
}

bool operator>(const Collider& body, const Vec2d& point) {
	if (body.isPointInside(point)) {
		return true;
	}
	return false;
}

