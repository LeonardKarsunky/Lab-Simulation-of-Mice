#include "CellLiverCancer.hpp"
#include "CellHandler.hpp"
#include <Application.hpp>

CellLiverCancer::CellLiverCancer(CellHandler* strate, double atp)
	: CellLiver(strate, atp) {}
	
CellLiverCancer::~CellLiverCancer() {}
	
//----------------------------------------------------------------------

void CellLiverCancer::ATPSynthesis(sf::Time dt) {
	Glycolyse(dt);
}

void CellLiverCancer::expand() {
	strate->expandCancer(getPosition());
}

double CellLiverCancer::getFractUptake() const {
	return getAppConfig().cancer_fract_uptake;
}

double CellLiverCancer::getKrebsVmax() const {
	return getAppConfig().cancer_km_glycolysis;
}

double CellLiverCancer::getKrebsKm() const {
	return getAppConfig().cancer_km_max_glycolysis;
}

double CellLiverCancer::getFractGlu() const {
	return getAppConfig().cancer_glucose_usage;
}

double CellLiverCancer::seuilEnergie() const {
	return getAppConfig().cancer_division_energy;
}

int CellLiverCancer::minNbCycles() const {
	return getAppConfig().cancer_time_next_division;
}

int CellLiverCancer::NbCyclesRange() const {
	return getAppConfig().cancer_range_next_division;
}
