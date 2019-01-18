#ifndef CELLHANDLER_H
#define CELLHANDLER_H

#include "CellOrgan.hpp"
#include "CellECM.hpp"
#include "CellLiver.hpp"
#include "CellBlood.hpp"
#include "Substance.hpp"
#include <Utility/Utility.hpp>
#include <SFML/Graphics.hpp>

class Organ;
class CellHandler {
public:
	CellHandler(CellCoord position, Organ* organ);
	virtual ~CellHandler();
	
	int getOrganNbCells() const;
	
	double getDeltaGlucose() const;
	double getDeltaBromo() const;
	double getDeltaVGEF() const;
	
	/*!
	 * @brief Vérifie si une case est en dehors de la grille ou non
	 * 
	 * @param coord la position d'une case (un CellCoord)
	 * @return true si la case est en dehors de la grille et false sinon
	 */
	bool isOut(const CellCoord& coord) const;
		
	bool hasECM() const;
	bool hasLiver() const;
	bool hasBlood() const;
		
	void setECM();
	void setLiver();
	void setBlood(TypeBloodCell type);
	
	/*!
	 * @brief Fait évoluer chaque cellule présente sur la case de l'instance
	 * courante
	 */
	virtual void update(sf::Time dt);
	
	/*!
	 * @brief Permet de mettre des substances sur le niveau "ECM". Cette
	 * méthode ajoutera une substance à l'objet Substance liée au niveau ECM du CellHandler.
	 * 
	 * @param substance une Substance
	 */
	void updateSubstance(const Substance& substance);
	
	/*!
	 * @brief Permet d'ajouter diffusedSubst au niveau ECM du CellHandler
	 * à la position logique pos
	 */
	void updateCellHandlerAt(const CellCoord& pos, const Substance& diffusedSubst);
	
	virtual double getQuantity(CellOrgan* cellule, SubstanceId id) const;
	double getECMQuantity(SubstanceId id) const;
	double getLiverQuantity(SubstanceId id) const;
	double getBloodQuantity(SubstanceId id) const;
	
	CellCoord getPosition() const;
	
	/*!
	 * @brief Permet au niveau «ECM» du CellHandler de céder au niveau
	 * «foie» une fraction de la substance identifié par id
	 */
	void liverTakeFromEcm(SubstanceId id, double fraction);
	
	/*!
	 * @brief Permet de mettre une cellule cancéreuse à la position logique
	 * de l'instance courante
	 */
	void setCancer();
	
	/*!
	 * @brief Permet de voir si la cellule hépatique du CellHandler est
	 * cancéreuse ou non
	 */
	bool hasCancer() const;
	
	/*!
	 * @brief Permet de faire accroître le foie en faisant diviser ses cellules
	 */
	void expandLiver(const CellCoord& current_position);
	
	/*!
	 * @brief Permet de faire accroître la tumeur en faisant diviser les
	 * cellules cancéreuses
	 */
	void expandCancer(const CellCoord& current_position);
		
private:
	//! La position logique dans la grille
	CellCoord position;
	
	//! L'organe (de l'animal) auquel il appartient
	Organ* organ;
	
	//! La cellule de l'ECM occupant cette position logique
	CellECM* cellule_ECM;
	
	//! La cellule du foie occupant cette position logique
	CellLiver* cellule_foie;
	
	//! La cellule du réseau sanguin occupant cette position logique
	CellBlood* cellule_sang;
	
	//! Un bool qui indique si la cellule hépatique du CellHandler est
	//! cancéreuse ou non
	bool cancer;
};

#endif
