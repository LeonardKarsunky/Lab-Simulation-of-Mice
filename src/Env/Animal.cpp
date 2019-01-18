#include <Env/Lab.hpp>
#include "Animal.hpp"
#include <Env/Organ.hpp>
#include <SFML/Graphics.hpp>
#include <Application.hpp>
#include <Utility/Constants.hpp>
#include <Random/Random.hpp>
#include <Utility/Arc.hpp>
#include <Utility/Utility.hpp>
#include <cmath>
#include <algorithm>
#include "Types.hpp"

Intervals Animal::angles({ -180, -100, -55, -25, -10, 0, 10, 25, 55, 100, 180});
Probs Animal::probabilites({0.0000,0.0000,0.0005,0.0010,0.0050,0.9870,0.0050,0.0010,0.0005,0.0000,0.0000});

Animal::Animal(const Vec2d& position, Quantity energie)
	: SimulatedEntity(position, energie),
	  etat(WANDERING),
	  velocite(0.0),
	  rassasie(false),
	  organ(new Organ(true)),
	  tracker(false),
	  rotation_timer(sf::Time::Zero),
	  bite_timer(sf::Time::Zero),
	  idle_timer(sf::Time::Zero) {}

Animal::~Animal() 
	{
		if (organ != nullptr) {
			delete organ;
			organ = nullptr;
		}
	}

//----------------------------------------------------------------------

Angle Animal::getNewRotation() const {
	return (DEG_TO_RAD * piecewise_linear(angles, probabilites));
}

Vec2d Animal::getHeading() const {
	return Vec2d::fromAngle(orientation);
}

Vec2d Animal::getSpeedVector() const {
	return (velocite * getHeading());
}

double Animal::getSatietyMin() const {
	return getAppConfig().animal_satiety_min;
}

double Animal::getSatietyMax() const {
	return getAppConfig().animal_satiety_max;
}

double Animal::getMealRetention() const {
	return getAppConfig().animal_meal_retention;
}

SubstanceId Animal::getCurrentSubst() const {
	return organ->getCurrentSubst();
}

void Animal::setCurrentSubst(SubstanceId substance_) {
	organ->setCurrentSubst(substance_);
}

double Animal::getDeltaGlucose() const {
	return organ->getDeltaGlucose();
}

double Animal::getDeltaVGEF() const {
	return organ->getDeltaVGEF();
}

double Animal::getDeltaBromo() const {
	return organ->getDeltaBromo();
}

void Animal::setDeltaGlucose(double delta_glucose) {
	organ->setDeltaGlucose(delta_glucose);
}

void Animal::setDeltaBromo(double delta_bromo) {
	organ->setDeltaBromo(delta_bromo);
}

void Animal::setDeltaVGEF(double delta_VGEF) {
	organ->setDeltaVGEF(delta_VGEF);
}

void Animal::placeEntity(Box* box) {
	SimulatedEntity::placeEntity(box);
	boite->addOccupant();
}

void Animal::drawOn(sf::RenderTarget& target) const {
	SimulatedEntity::drawOn(target);
	
	if (isDebugOn() and (energie > 0.0)) {
		double rotation(getHeading().angle());
		double arc_radius(getViewDistance());
		
		sf::Color color(sf::Color::Black);
		color.a = 16; //! light, transparent grey
		Arc arcgraphics((rotation - (getViewRange()/2))/DEG_TO_RAD, 
						(rotation + (getViewRange()/2))/DEG_TO_RAD, 
						arc_radius, color, arc_radius);
		arcgraphics.setOrigin(arc_radius, arc_radius);
		arcgraphics.setPosition(getCenter());
		target.draw(arcgraphics);
		
		std::string state;
		switch (etat) {
			case WANDERING:
				state = "WANDERING";
				break;
			case IDLE:
				state = "IDLE";
				break;
			case FOOD_IN_SIGHT:
				state = "FOOD_IN_SIGHT";
				break;
			case FEEDING:
				state = "FEEDING";
				break;
		}
		
		Vec2d text_center(getCenter());
		Vec2d heading(Vec2d::fromAngle(orientation - 0.55));
		text_center.x += heading.x * 225;
		text_center.y += heading.y * 225;
		
		auto text = buildText(state,
							  text_center,
							  getAppFont(),
							  getAppConfig().default_debug_text_size,
							  getAppConfig().debug_text_color);
		text.setRotation(orientation / DEG_TO_RAD + 90);
		target.draw(text);
	}
	
	if (isBeingTracked()) {
		Vec2d track_center(getCenter());
		Vec2d heading(Vec2d::fromAngle(orientation - 0.5));
		track_center.x -= heading.x * 65;
		track_center.y -= heading.y * 65;
		
		sf::Sprite entitySprite(buildSprite(track_center, (getRadius()/2), getAppTexture(getAppConfig().entity_texture_tracked)));
		entitySprite.setRotation(orientation / DEG_TO_RAD); 
		target.draw(entitySprite);
	}
}

void Animal::setOrgan(Organ* organ_) {
	if (organ_ != nullptr) {
		delete organ;
		organ = organ_;
	}
}

void Animal::drawOrgan(sf::RenderTarget& target) const {
	organ->drawOn(target);
}

void Animal::update(sf::Time dt) {
	SimulatedEntity::update(dt);
	
	double interval_time(dt.asSeconds());
	double perte_energie(getAppConfig().animal_base_energy_consumption + 
						(velocite * getAppConfig().mouse_energy_loss_factor * interval_time));
	if (energie > perte_energie) {
		energie -= perte_energie;
	} else {
		energie = 0.0;
	}
	
	SimulatedEntity* entite_tmp(getAppEnv().closestEntity(this));
	
	switch (etat)
	{
		case WANDERING:
			velocite = getMaxSpeed();
			move(dt);
			
			updateState(dt);
			break;			
		case IDLE:
			velocite = 0.0;
			idle_timer += dt;
			
			if (idle_timer > sf::seconds(2.0)) { //! On peut ajuster le temps que chaque phase IDLE va durer
				updateState(dt);
				idle_timer = sf::Time::Zero;
			}
			break;			
		case FOOD_IN_SIGHT:
			velocite = getMaxSpeed();		
			if (entite_tmp != nullptr) {
				move(getAttraction(entite_tmp->getCenter()), dt);
			} else {
				move(dt);
			}
			
			updateState(dt);
			break;
		case FEEDING:
			velocite = 0.0;
			bite_timer += dt;
			
			if (entite_tmp != nullptr) {
				if (bite_timer > sf::seconds(0.1)) {
					entite_tmp->provideEnergy(getBite());
					energie += getBite() * getMealRetention();
					bite_timer = sf::Time::Zero;
				}
			}
			
			if ((energie > getSatietyMax()) or (entite_tmp == nullptr)
				or (not(this->isColliding(entite_tmp->getCenter(), entite_tmp->getInitialRadius())))) {
				updateState(dt);
			}
			break;
	}
	
	entite_tmp = nullptr;
}

void Animal::updateState(sf::Time dt) {
	SimulatedEntity* entite_tmp(getAppEnv().closestEntity(this));
	
	if ((entite_tmp != nullptr) and (energie < getSatietyMin())) {
		if (entite_tmp->isColliding(*this)) {
			etat = FEEDING;
		} else {
			etat = FOOD_IN_SIGHT;
		}
	} else {
		if (bernoulli(0.001)) { //! On prend '0.001' car on ne veut pas avoir de probabilité d'être en état IDLE
			etat = IDLE;
		} else {
			etat = WANDERING;
		}
	}
	
	entite_tmp = nullptr;
}

void Animal::updateOrgan() {
	if (organ != nullptr) {
		organ->update();
	}
}

void Animal::move(sf::Time dt) {
	sf::Time temps_entre_deux_rotations(sf::seconds(0.08));
	rotation_timer += dt;
	
	if ((rotation_timer > temps_entre_deux_rotations) and (etat != FOOD_IN_SIGHT)) {
		setOrientation(getHeading().angle() + getNewRotation());
		rotation_timer = sf::Time::Zero;
	}
	
	runningIntoWalls(dt);
	checkIfOnWall();
}

void Animal::move(const Vec2d& force, sf::Time dt) {
	Vec2d acceleration(force/getMass());
	double time(dt.asSeconds());
	Vec2d vitesse(getHeading() * velocite);
	
	vitesse += acceleration * time;
	orientation = (vitesse.normalised()).angle();
	velocite = sqrt(vitesse.lengthSquared());
	
	if (velocite > getMaxSpeed()) {
		vitesse = (vitesse.normalised()) * getMaxSpeed();
	}
	
	setCenter(getCenter() + (vitesse * time));
	checkIfOnWall();		
}

void Animal::resetBox() {
	SimulatedEntity::removeBox();
	removeBox();
}

bool Animal::canBeTakenEnergy() const {
	return false;
}

bool Animal::isSolitary() const {
	return true;
}

bool Animal::isTargetInSight(const Vec2d& position_other) const {
	Vec2d distance(position_other - getCenter());
	Vec2d normalised_distance(distance.normalised());
		
	if (isEqual(sqrt(distance.lengthSquared()), 0.0, 0.001)) {
		return true;
	}
		
	if (boite->isPositionInside(position_other) and (getDistance(distance) <= getViewDistance())
		and (normalised_distance.dot(getHeading()) >= cos((getViewRange() + 0.001)/2))) {
		return true;
	}
	
	return false;
}

void Animal::checkIfOnWall() {
	auto topWall(boite->getTopLimit(true));
	auto bottomWall(boite->getBottomLimit(true));
	auto leftWall(boite->getLeftLimit(true));
	auto rightWall(boite->getRightLimit(true));
	double radius(getRadius());
	
	if (getCenter().y - radius < topWall) {
		setCenterY(topWall + radius);
	}
	if (getCenter().y + radius > bottomWall) {
		setCenterY(bottomWall - radius);				
	}
	if (getCenter().x - radius < leftWall) {
		setCenterX(leftWall + radius);
	}
	if (getCenter().x + radius > rightWall) {
		setCenterX(rightWall - radius);
	}
}

void Animal::runningIntoWalls(sf::Time dt) {
	double time(dt.asSeconds());
	Vec2d next_position(getCenter() + (getSpeedVector() * time));
	
	if (boite->isColliderInside(next_position, getRadius())) {
		setCenter(next_position);
	} else {
		if (boite->whichWall(next_position, getRadius()) == boite->getWallTop()) {
			if (orientation < 0.0) {
				setOrientation(-getHeading().angle());
			}
			
		} else if (boite->whichWall(next_position, getRadius()) == boite->getWallBottom()) {
			if (orientation > 0.0) {
				setOrientation(-getHeading().angle());
			}
			
		} else if (boite->whichWall(next_position, getRadius()) == boite->getWallRight()) {
			if ((orientation > (-PI/2)) and (orientation < (PI/2))) {
				setOrientation(PI - getHeading().angle());
			}
			
		} else if (boite->whichWall(next_position, getRadius()) == boite->getWallLeft()) {
			if ((orientation < (-PI/2)) or (orientation > (PI/2))) {
				setOrientation(PI - getHeading().angle());
			}
		}
	}
}

Vec2d Animal::getAttraction(const Vec2d& position_other) const {
	Vec2d toTarget(position_other - getCenter());
	double toTarget_norme(sqrt(toTarget.lengthSquared()));
	
	double speed(std::min((toTarget_norme/0.3), getMaxSpeed()));
	Vec2d v_target(toTarget*(speed/toTarget_norme));
	
	Vec2d force(v_target - (getHeading() * velocite));
	
	return force;
}

void Animal::setTrack(bool track) {
	tracker = track;
}

bool Animal::isBeingTracked() const {
	return tracker;
}

void Animal::setCancerAt(const Vec2d& pos) {
	if (organ != nullptr) {
		organ->setCancerAt(pos);
	}
}
