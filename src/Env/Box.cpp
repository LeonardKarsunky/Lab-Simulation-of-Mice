#include "Box.hpp"
#include <Application.hpp>
#include <Utility/Utility.hpp>

Box::Box() {}

Box::Box (const Vec2d& centre, double largeur, double hauteur, double epaisseur) 
	: centre(centre),
	  largeur(largeur),
	  hauteur(hauteur),
	  epaisseur(epaisseur),
	  wall_top({{largeur - epaisseur, epaisseur}, {0.0, 0.0}}),
	  wall_bottom({{largeur, hauteur}, {epaisseur, hauteur - epaisseur}}),
	  wall_left({{epaisseur, hauteur}, {0.0, epaisseur}}),
	  wall_right({{largeur, hauteur - epaisseur}, {largeur - epaisseur, 0.0}}),
	  animal_present(false) {}

Box::~Box() {}

//----------------------------------------------------------------------

Vec2d Box::getCenter() const {
	return centre;
}
double Box::getWidth() const {
	return largeur;
}
double Box::getHeight() const {
	return hauteur;
}
double Box::getWallWidth() const {
	return epaisseur;
}

void Box::setWidth(double Width) {
	largeur = Width;
}

void Box::setHeight(double Height) {
	hauteur = Height;
}

void Box::setWallWidth(double WallWidth) {
	epaisseur = WallWidth;
}

void Box::setCenter(const Vec2d& Center) {
	centre = Center;
}

void Box::setWalls() {
	Wall wall_top_tmp({{centre.x + (largeur/2) - epaisseur, centre.y - (hauteur/2) + epaisseur},
					   {centre.x - (largeur/2), centre.y - (hauteur/2)}});
	Wall wall_bottom_tmp({{centre.x + (largeur/2), centre.y + (hauteur/2)},
						  {centre.x - (largeur/2) + epaisseur, centre.y + (hauteur/2) - epaisseur}});
	Wall wall_left_tmp({{centre.x - (largeur/2) + epaisseur, centre.y + (hauteur/2)},
					    {centre.x - (largeur/2), centre.y - (hauteur/2) + epaisseur}});
	Wall wall_right_tmp({{centre.x + (largeur/2), centre.y + (hauteur/2) - epaisseur},
						 {centre.x + (largeur/2) - epaisseur, centre.y - (hauteur/2)}});

	wall_top = wall_top_tmp;
	wall_bottom = wall_bottom_tmp;
	wall_left = wall_left_tmp;
	wall_right = wall_right_tmp;
}

double Box::getLeftLimit(bool intern) const {
	if (intern == true) {
		return (getCenter().x - largeur/2 + epaisseur);
	} else {
        return (getCenter().x - largeur/2);
	}
}

double Box::getRightLimit(bool intern) const {
	if (intern == true) {
		return (getCenter().x + largeur/2 - epaisseur);
	} else {
		return (getCenter().x + largeur/2);
	}
}

double Box::getTopLimit(bool intern) const {
	if (intern == true) {
		return (getCenter().y - hauteur/2 + epaisseur);
	} else {
		return (getCenter().y - hauteur/2);
	}
}

double Box::getBottomLimit(bool intern) const {
	if (intern == true) {
        return (getCenter().y + hauteur/2 - epaisseur);
	} else {
		return (getCenter().y + hauteur/2);
	}
}

Wall Box::getWallTop() const {
	return wall_top;
}

Wall Box::getWallBottom() const {
	return wall_bottom;
}

Wall Box::getWallRight() const {
	return wall_right;
}

Wall Box::getWallLeft() const {
	return wall_left;
}

bool Box::isPositionInside(const Vec2d& position) const {
	if ((position.x < getRightLimit(true)) and (position.x > getLeftLimit(true))
	    and (position.y < getBottomLimit(true)) and (position.y > getTopLimit(true))) {
		return true;
	} else {
		return false;
	}
}

bool Box::isPositionOnWall(const Vec2d& position) const {
    if (not(isPositionInside(position)) and (position.x >= getLeftLimit(false))
		and (position.x <= getRightLimit(false)) and (position.y >= getTopLimit(false))
		and (position.y <= getBottomLimit(false))) {
		return true;
	} else {
		return false;
	}
}

bool Box::isColliderInside(const Vec2d& position, double radius) const {
	if ((position.x + radius < getRightLimit(true)) and (position.x - radius > getLeftLimit(true))
	    and (position.y + radius < getBottomLimit(true)) and (position.y - radius > getTopLimit(true))) {
		return true;
	} else {
		return false;
	}
}

Wall Box::whichWall(const Vec2d& position, double radius) const {
	if ((position.x + radius >= getRightLimit(true)) and (position.y + radius < getBottomLimit(true))) {
		return wall_right;
	} else if ((position.x - radius <= getLeftLimit(true)) and (position.y - radius > getTopLimit(true))) {
		return wall_left;
	} else if ((position.x - radius > getLeftLimit(true)) and (position.y + radius >= getBottomLimit(true))) {
		return wall_bottom;
	} else if ((position.x + radius < getRightLimit(true)) and (position.y - radius <= getTopLimit(true))) {
		return wall_top;
	}
}

void Box::drawOn(sf::RenderTarget& target) const {
	drawWall(wall_top.first, wall_top.second, target);
	drawWall(wall_bottom.first, wall_bottom.second, target);
	drawWall(wall_left.first, wall_left.second, target);
	drawWall(wall_right.first, wall_right.second, target);
}

void Box::drawWall(Vec2d point1, Vec2d point2, sf::RenderTarget& target) const {
	sf::RectangleShape rectangle(buildRectangle(point1, point2, 
	    &getAppTexture(getAppConfig().simulation_lab_fence)));
	target.draw(rectangle);
}

void Box::reset() {
	if (animal_present == true) {
		animal_present = false;
	}
}

bool Box::isEmpty() const {
	if (animal_present == false) {
		return true;
	}
	return false;
}

void Box::addOccupant() {
	animal_present = true;
}
