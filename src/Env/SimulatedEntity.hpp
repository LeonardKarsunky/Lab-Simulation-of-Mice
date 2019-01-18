#ifndef SIMULATEDENTITY_H
#define SIMULATEDENTITY_H

#include <SFML/Graphics.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Vec2d.hpp>
#include <iostream>
#include "Collider.hpp"
#include "Box.hpp"

class Box;
class Mouse;
class Cheese;
class SimulatedEntity : public Collider {
public:
	SimulatedEntity(const Vec2d& position, Quantity energie);
	virtual ~SimulatedEntity();
	
	Vec2d getCenter() const override;
	Vec2d getPosition() const;
	sf::Time getAge() const;
	Quantity getEnergy() const;
	virtual sf::Time getLongevity() const;
	virtual sf::Texture& getTexture() const = 0;
	
	void setCenter(const Vec2d& center) override;
	void setCenterX(double x) override;
	void setCenterY(double y) override;
	void setOrientation(Angle angle);
	
	/*!
	 * Permet de prendre de l'énergie de l'instance considérée si cette
	 * instance est capable de donner de l'énergie (on vérifie avec canBeTakenEnergy())
	 */
	Quantity provideEnergy(Quantity qte);
	
	/*!
	 * @brief Faire évoluer l'entité simulée au cours du temps
	 */	
	virtual void update(sf::Time dt);
	
	/*!
	 * @brief Dessiner l'entité simulée considérée
	 */
	virtual void drawOn(sf::RenderTarget& target) const;
	
	Box* getBox() const;
	void setBox(Box* box);
	
	/*!
	 * @brief Même chose que setBox
	 */
	void confine(Box* box);
	
	void removeBox();
	
	/*!
	 * @brief Affecte l'attribut de la boite 'animal_present' à false
	 */
	virtual void resetBox();
	
	/*!
	 * @brief Vérifie si l'instance courante peut être confinée dans la boite concernée
	 */
	virtual bool canBeConfinedIn(Box* box) const;
	
	/*!
	 * @brief Place l'instance courante dans la boite concernée
	 */
	virtual void placeEntity(Box* box);
	
	/*!
	 * @brief Vérifie si l'entité mis en paramètre peut être mangée par
	 * l'instance courante
	 */
	virtual bool eatable(SimulatedEntity const* entity) const = 0;
	virtual bool eatableBy(Mouse const* mouse) const = 0;
	virtual bool eatableBy(Cheese const* cheese) const = 0;
	
	/*!
	 * @brief Vérifie si l'instance courante est capable de donner de l'énergie
	 */
	virtual bool canBeTakenEnergy() const = 0;
	
	/*!
	 * @brief Vérifie si l'instance courante doit être seule dans une boite
	 * (e.g. la souris)
	 */
	virtual bool isSolitary() const = 0;
	
	/*!
	 * @brief Calcule la distance entre la position de l'instance courante
	 * et le Vec2d mis en paramètre
	 */
	double getDistance(const Vec2d& position_other) const;

protected:
	//! Permet de changer l'orientation
	virtual void setRotation(const Angle& angle);
	
	Vec2d position;
	Angle orientation;
	Box* boite;
	sf::Time age;
	Quantity energie;
};

#endif
