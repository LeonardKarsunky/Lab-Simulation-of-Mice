#ifndef MOUSE_H
#define MOUSE_H

#include "SimulatedEntity.hpp"
#include "Animal.hpp"
#include <iostream>
#include <Utility/Vec2d.hpp>

class Mouse : public Animal {
public:
	Mouse(const Vec2d& position);
	
	double getRadius() const override;
	double getInitialRadius() const override;
	double getMaxSpeed() const override;
	
	double getEnergyLossFactor() const override;
	double getMass() const override;
	Quantity getBite() const override;
	
	sf::Time getLongevity() const override;
	sf::Texture& getTexture() const override;
	
	bool eatable(SimulatedEntity const* entity) const override;
	bool eatableBy(Mouse const* mouse) const override;
	bool eatableBy(Cheese const* cheese) const override;
	
	double getViewRange() const override;
	double getViewDistance() const override;
};

#endif
