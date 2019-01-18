#ifndef LAB_H
#define LAB_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Types.hpp"
#include "Box.hpp"
#include "SimulatedEntity.hpp"
#include "Mouse.hpp"
#include "Cheese.hpp"

typedef std::vector<std::vector<Box*> > Lab_boxes;

/*!
 * @class Lab
 * 
 * @brief Un Lab est caractérisé par l'ensemble des boites qu'il contient,
 * et les entités simulées qui sont dedans
 */
class Lab {
public:
	Lab(Lab const& lab) = delete;
	
	Lab();
	virtual ~Lab();
	
	/*!
	 * @brief Permet de quadriller le laboratoire avec un nombre donné
	 * de boîtes par ligne
	 * 
	 * @param nbCagesPerRow le nombre de boîtes par ligne
	 */
	void makeBoxes(unsigned int nbCagesPerRow);
	
	/*!
	 * @brief Supprime toutes les boîtes du laboratoire
	 * (l'ensemble de boîtes devient vide)
	 */
	void destroyBoxes();
	
	/*!
	 * @brief Fait évoluer le contenu des boites au cours du temps
	 */	
	virtual void update(sf::Time dt);
	
	/*!
	 * @brief Fait évoluer l'organ de l'animal traqué
	 */
	void updateTrackedAnimal();
	
	/*!
	 * @brief Dessine le contenu du laboratoire
	 */
	virtual void drawOn(sf::RenderTarget& targetWindow) const;
	
	/*!
	 * @brief Dessine l'organe (en vue interne) de l'animal traqué
	 */
	void drawCurrentOrgan(sf::RenderTarget& target) const;
	
	/*!
	 * @brief Vide les boîtes de leur contenu
	 */
	virtual void reset();
	
	/*!
	 * @brief Ajoute une entité simulée dans le lab (c'est-à-dire dans
	 * une des boites du lab)
	 */
	virtual bool addEntity(SimulatedEntity* entite);
	
	/*!
	 * @brief Ajouter un animal dans le lab
	 */
	bool addAnimal(Animal* animal);
	
	/*!
	 * @brief Ajouter du fromage dans le lab
	 */
	bool addCheese(Cheese* cheese);
	
	/*!
	 * @brief Permet de trouver l'entité simulée la plus proche d'un
	 * animal qui est dans la même boite et qui est dans son champ de vision
	 */
	virtual SimulatedEntity* closestEntity(Animal const* entity);
	
	void trackAnimal(Animal* animal);
	void trackAnimal(const Vec2d& position_cursor);
	void stopTrackingAnyEntity();
	
	/*!
	 * @brief Permet à la simulation de basculer sur une vue donnée.
	 * Le basculement vers une vue autre que LAB ne doit pouvoir se faire
	 * que s'il existe un animal traqué
	 * 
	 * @param view la vue que l'on souhaite avoir
	 */
	void switchToView(View view);
	
	/*!
	 * @brief Permet de passer de façon cyclique d'une valeur à l'autre
	 * pour la substance courante
	 */
	void nextSubstance();
	
	/*!
	 * @brief Permet d'augmenter le delta associé à la substance courante
	 */
	void increaseCurrentSubst();
	
	/*!
	 * @brief Permet de diminuer le delta associé à la substance courante
	 */
	void decreaseCurrentSubst();
	
	/*!
	 * @brief Retourne la valeur courante du delta associé à la substance id
	 */
	double getDelta(SubstanceId id) const;
	
	/*!
	 * @brief Retourne l'identifiant (un SubstanceId) de la substance courante
	 */
	SubstanceId getCurrentSubst() const;
	
	/*!
	 * @brief Borne la valeur passée en paramètre
	 */
	double delta_borne(double deltaSubst);
	
	/*!
	 * @brief Permet mettre une cellule cancéreuse dans l'organe de l'animal
	 * traqué à la position physique pos
	 */
	void setCancerAt(const Vec2d& pos);

private:
	//! L'ensemble des boites du laboratoire, un vecteur de vecteur
	//! de pointeurs de boite (pointeurs à la C)
	Lab_boxes boites;
	
	//! La collection hétérogène des entités simulées dans le lab
	std::vector<SimulatedEntity*> lesEntites;
	
	//! La collection des animaux simulées dans le lab
	std::vector<Animal*> animals;
	
	//! La collection des bouts de fromage simulées dans le lab
	std::vector<Cheese*> cheeses;
	
	//! L'animal traqué
	Animal* animal_tracked;
};

#endif
