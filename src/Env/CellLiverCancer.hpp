#ifndef CELLLIVERCANCER_H
#define CELLLIVERCANCER_H

#include "CellLiver.hpp"

class CellLiverCancer : public CellLiver {
public:
	CellLiverCancer(CellHandler* strate, double atp = 100.0);
	~CellLiverCancer();
	
	double getKrebsVmax() const override;
	double getKrebsKm() const override;
	
	double getFractGlu() const override;
	
	/*!
	 * @brief Transforme le glucose consommé en énergie par la glycolyse
	 * et ajout de cette énergie à l'énergie courante
	 */
	void ATPSynthesis(sf::Time dt) override;
	
	double getFractUptake() const override;
	
	void expand() override;
	
	double seuilEnergie() const override;
	
	int minNbCycles() const override;
	int NbCyclesRange() const override;
};

#endif
