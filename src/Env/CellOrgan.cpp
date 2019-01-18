#include "CellOrgan.hpp"
#include <Env/CellHandler.hpp>

CellOrgan::CellOrgan(CellHandler* strate)
	: substance(new Substance),
	  strate(strate) {}

CellOrgan::~CellOrgan()
{
	if (substance != nullptr) {
		delete substance;
	}
}

//----------------------------------------------------------------------

Substance CellOrgan::getSubstance() const {
	return (*substance);
}

void CellOrgan::setSubstance(const Substance& substance_) {
	*substance = substance_;
}

double CellOrgan::getGlucose() const {
	return (substance->getFractGlucose() * substance->getTotalConcentration());
}

double CellOrgan::getVGEF() const {
	return (substance->getFractVGEF() * substance->getTotalConcentration());
}

double CellOrgan::getInhibitor() const {
	return (substance->getFractInhibitor() * substance->getTotalConcentration());
}

CellCoord CellOrgan::getPosition() const {
	return strate->getPosition();
}

void CellOrgan::updateSubstance(const Substance& substance_) {
	*substance += substance_;
}

void CellOrgan::uptakeOnGradient(double fraction, CellOrgan* cellule, SubstanceId id) {
	Substance substance_tmp(cellule->getSubstance());
	
	substance->uptakeOnGradient(fraction, substance_tmp, id);
	
	cellule->setSubstance(substance_tmp);
}
