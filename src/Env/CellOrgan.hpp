#ifndef CELLORGAN_H
#define CELLORGAN_H

#include "Substance.hpp"
#include <SFML/Graphics.hpp>
#include <Utility/Utility.hpp>

class CellHandler;
class CellOrgan {
public:
	CellOrgan(CellHandler* strate);
	virtual ~CellOrgan();
	
	double getGlucose() const;
	double getVGEF() const;
	double getInhibitor() const;
	
	CellCoord getPosition() const;
	
	/*!
	 * @brief Fait évoluer une cellule de l'organe
	 * 
	 * @brief Cette méthode est virtuelle pure car on ne peut pas faire
	 * évoluer (a priori) une cellule quelconque
	 */
	virtual void update(sf::Time dt) = 0;
	
	/*!
	 * @brief Permet d'ajouter une quantité de substance à la substance
	 * de l'instance courante
	 */
	void updateSubstance(const Substance& substance_);
	
	/*!
	 * @brief A la même rôle que uptakeOnGradient de la classe Substance
	 */
	void uptakeOnGradient(double fraction, CellOrgan* cellule, SubstanceId id);
		
protected:
	Substance getSubstance() const;
	void setSubstance(const Substance& substance_);
	
	//! La substance véhiculée dans la cellule
	Substance* substance;
	
	//! Le CellHandler associé à cette position
	CellHandler* strate;
};

#endif
