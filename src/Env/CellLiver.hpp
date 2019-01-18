#ifndef CELLLIVER_H
#define CELLLIVER_H

#include "CellOrgan.hpp"
#include <SFML/Graphics.hpp>

class CellLiver : public CellOrgan {
public:
	CellLiver(CellHandler* strate, double atp = 100.0);
	virtual ~CellLiver();
	
	double getATP() const;
	
	virtual double getFractUptake() const;
	
	virtual double getKrebsVmax() const;
	virtual double getKrebsKm() const;
	
	virtual double getFractGlu() const;
	
	void update(sf::Time dt) override;
	
	/*!
	 * @brief Transforme le glucose consommé en énergie par la glycolyse
	 * et le cycle de Krebs et ajout de cette énergie à l'énergie courante
	 */
	virtual void ATPSynthesis(sf::Time dt);
	
	/*!
	 * @brief Vérifie si la cellule hépatique peut se diviser ou non
	 */
	virtual bool canDivide() const;
	
	/*!
	 * @brief Permet au foie de s'accroître en faisant diviser la cellule
	 * hépatique
	 */
	virtual void expand();
	
	/*!
	 * @brief Calcule la quantité d'énergie produite au terme du cycle de Krebs
	 * par l'équation de Michaelis-Menten et l'ajoute à l'atp de la cellule
	 */
	virtual void Krebs(sf::Time dt);
	
	/*!
	 * @brief Calcule la quantité d'énergie produite au terme du cycle de
	 * Krebs par une variante de l'équation de Michaelis-Menten et l'ajoute
	 * à l'atp de la cellule
	 */
	virtual void Glycolyse(sf::Time dt);
	
	/*!
	 * @brief Renvoie le seuil minimal d'énergie nécessaire pour la division
	 */
	virtual double seuilEnergie() const;
	
	/*!
	 * @brief Renvoie le nombre minimal de cylces pour que la cellule puisse
	 * se diviser
	 */
	virtual int minNbCycles() const;
	
	/*!
	 * @brief Renvoie la gamme d'énergie entre laquel la cellule peut se
	 * diviser
	 */
	virtual int NbCyclesRange() const;
	
	/*!
	 * @brief Permet de mettre à jour les paramètres current_cycle et
	 * number_cylce pour la prochaine division
	 */
	virtual void finaliser_division();

protected:
	//! La quantité d'ATP de la cellule hépatique
	double atp;
	
	//! Indique le nombre actuel de cycles d'update parcourus
	int current_cycle;
	
	//! Indique le nombre de cycles d'update à faire pour que la cellule
	//! puisse se diviser
	int number_cycles;
};

#endif
