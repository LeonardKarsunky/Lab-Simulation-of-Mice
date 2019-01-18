#include "CellLiver.hpp"
#include "CellHandler.hpp"
#include <Application.hpp>
#include <Random/Random.hpp>
#include <cmath>

CellLiver::CellLiver(CellHandler* strate, double atp)
	: CellOrgan(strate),
	  atp(atp),
	  current_cycle(0),
	  number_cycles(uniform(minNbCycles(), (minNbCycles() + NbCyclesRange())))
	  {}

CellLiver::~CellLiver() {}

//----------------------------------------------------------------------

double CellLiver::getATP() const {
	return atp;
}

double CellLiver::getFractUptake() const {
	return getAppConfig().liver_fract_uptake;
}

double CellLiver::getKrebsKm() const {
	return getAppConfig().liver_km_glycolysis;
}

double CellLiver::getKrebsVmax() const {
	return getAppConfig().liver_km_max_glycolysis;
}

double CellLiver::getFractGlu() const {
	return getAppConfig().liver_glucose_usage;
}

void CellLiver::update(sf::Time dt) {
	++current_cycle;
	
	if (atp > 0.0) {
		atp *= 1 - exp(-getAppConfig().liver_decay_atp * (dt.asSeconds()));
		atp -= gamma(getAppConfig().base_atp_usage, (getAppConfig().base_atp_usage + getAppConfig().range_atp_usage));
	}
	
	strate->liverTakeFromEcm(GLUCOSE, getFractUptake());
	strate->liverTakeFromEcm(BROMOPYRUVATE, getFractUptake());
	
	ATPSynthesis(dt);
	
	if (atp < 0.0) {
		atp = 0.0;
	}
	
	if (canDivide()) {
		atp -= getAppConfig().liver_division_cost;
		expand();
		finaliser_division();
	}
}

void CellLiver::ATPSynthesis(sf::Time dt) {
	Krebs(dt);
	Glycolyse(dt);
}

bool CellLiver::canDivide() const {
	return ((atp >= seuilEnergie()) and (current_cycle >= number_cycles));
}

void CellLiver::expand() {
	strate->expandLiver(getPosition());
}

void CellLiver::Krebs(sf::Time dt) {
	double S(getKrebsVmax() * (strate->getLiverQuantity(GLUCOSE) * 0.8));
	
	atp += dt.asSeconds() * ((getKrebsVmax() * S)/(S + getKrebsKm()));
}

void CellLiver::Glycolyse(sf::Time dt) {
	double S(strate->getLiverQuantity(GLUCOSE) * getFractGlu() * 0.8);
	double I(strate->getLiverQuantity(BROMOPYRUVATE));
	double k(0.6);
	
	atp += (0.1 * dt.asMilliseconds()) * ((getKrebsVmax() * S)/(S + (getKrebsKm() * (1 + I/k))));
}

double CellLiver::seuilEnergie() const {
	return getAppConfig().liver_division_energy;
}

int CellLiver::minNbCycles() const {
	return getAppConfig().liver_time_next_division;
}

int CellLiver::NbCyclesRange() const {
	return getAppConfig().liver_range_next_division;
}

void CellLiver::finaliser_division() {
	current_cycle = 0;
	number_cycles = uniform(minNbCycles(), (minNbCycles() + NbCyclesRange()));
}
