#ifndef ANIMAL_H
#define ANIMAL_H

#include "SimulatedEntity.hpp"
#include <Utility/Vec2d.hpp>
#include <Types.hpp>
#include <iostream>

enum Etat {
	 FOOD_IN_SIGHT, //! nourriture en vue (il sera attiré par la nourriture)
	 FEEDING,       //! en train de manger (là en principe il arrête de se déplacer)
	 WANDERING,     //! déambule
	 IDLE,          //! au repos
};

class Lab;
class Organ;
class Animal : public SimulatedEntity {
public:
	Animal(const Vec2d& position, Quantity energie);
	virtual ~Animal();
	
	virtual double getMaxSpeed() const = 0;
	virtual double getEnergyLossFactor() const = 0;
	virtual double getMass() const = 0;
	virtual Quantity getBite() const = 0;
	
	Angle getNewRotation() const;
	Vec2d getHeading() const;
	Vec2d getSpeedVector() const;
	
	virtual double getViewRange() const = 0;
	virtual double getViewDistance() const = 0;
	
	double getSatietyMin() const;
	double getSatietyMax() const;
	double getMealRetention() const;
	
	SubstanceId getCurrentSubst() const;
	void setCurrentSubst(SubstanceId substance_);
	
	double getDeltaGlucose() const;
	double getDeltaVGEF() const;
	double getDeltaBromo() const;
	
	void setDeltaGlucose(double delta_glucose);
	void setDeltaBromo(double delta_bromo);
	void setDeltaVGEF(double delta_VGEF);
	
	void placeEntity(Box* box) override;
	
	/*!
	 * @brief Dessine l'animal dans le laboratoire (vue externe)
	 */
	void drawOn(sf::RenderTarget& target) const override;
	
	/*!
	 * @brief Dessine l'organe de l'animal (vue interne)
	 */
	void drawOrgan(sf::RenderTarget& target) const;
	
	/*!
	 * @brief Fait évoluer l'animal au cours du temps en fonction de son état
	 */
	void update(sf::Time dt) override;
	
	/*!
	 * @brief Fait évoluer l'état de l'animal au cours du temps
	 */
	virtual void updateState(sf::Time dt);
	
	/*!
	 * @brief Fait évoluer l'organe de l'animal au cours du temps
	 */
	void updateOrgan();
	
	/*!
	 * @brief Fait évoluer la position et l'orientation de l'animal au cours du temps
	 */
	virtual void move(sf::Time);
	virtual void move(const Vec2d& force, sf::Time dt);
	
	void resetBox() override;
	
	bool canBeTakenEnergy() const override;
	bool isSolitary() const override;
	
	/*!
	 * @brief Vérifie si une autre entité est dans le champ de vision de l'instance courante
	 */
	bool isTargetInSight(const Vec2d& position_other) const;
	
	/*!
	 * @brief Regarde si la position courante est sur l'un des murs et si
	 * oui change la celle-ci pour remettre l'animal complètement dans la boite
	 */
	void checkIfOnWall();
	
	/*!
	 * @brief Regarde si la prochaine position (après un pas de temps 'dt')
	 * sera sur l'un des murs et si oui change l'orientation de l'animal
	 * 
	 * @brief Change l'orientation de l'animal de telle sorte à ce qu'elle
	 * semble "rebondir" du mur dans la direction souhaitée
	 */
	void runningIntoWalls(sf::Time dt);
	
	/*!
	 * @brief Retourne "l'accélération" (un Vec2d) qu'a un animal s'il est
	 * attiré par une source de nourriture
	 */
	Vec2d getAttraction(const Vec2d& position_other) const;
	
	/*!
	 * @brief Permet de traquer l'animal en affectant une valeur (un bool)
	 * à l'attribut "tracker"
	 */
	void setTrack(bool track);
	
	/*!
	 * @brief Permet de voir si l'animal est traqué ou non
	 */
	bool isBeingTracked() const;
	
	/*!
	 * @brief Permet mettre une cellule cancéreuse dans l'organe de l'animal
	 * à la position physique pos
	 */
	void setCancerAt(const Vec2d& pos);

protected:
	//! L'état actuel de l'animal
	Etat etat;
	
	//! La vitesse (norme) de l'animal
	double velocite;
	
	//! Bool montrant si l'animal est rassasié ou non
	bool rassasie;
	
	//! L'organe de l'animal
	Organ* organ;
	
	/*!
	 * @brief Le traqueur de l'animal (pour voir si l'animal, dans la vue
	 *externe du lab, est traqué ou non)
	 */
	bool tracker;
	
	/*!
	 * @brief L'intervalle des angles pour déterminer la prochaine orientation,
	 * avec les probabilités respectives
	 */
	static Intervals angles;
	static Probs probabilites;
	
	/*!
	 * @brief Des compteurs utilisés pour la rotation, pour voir quelle
	 * bouchée prendre lorsqu'il mange, et pour fixer la durée de temps
	 * quand l'état de l'animal est 'IDLE'
	 */
	sf::Time rotation_timer;
	sf::Time bite_timer;
	sf::Time idle_timer;
	
	void setOrgan(Organ* organ_);
};

#endif
