#ifndef CHEESE_H
#define CHEESE_H

#include "SimulatedEntity.hpp"
#include <iostream>
#include <Utility/Vec2d.hpp>

class Cheese : public SimulatedEntity {
public:
	Cheese(const Vec2d& position);
	Cheese();
	
	double getRadius() const override;
	double getInitialRadius() const override;
	sf::Texture& getTexture() const override;
	
	bool canBeTakenEnergy() const override;
	bool isSolitary() const override;
	
	bool eatable(SimulatedEntity const* entity) const override;
	bool eatableBy(Mouse const* mouse) const override;
	bool eatableBy(Cheese const* cheese) const override;
};

#endif
