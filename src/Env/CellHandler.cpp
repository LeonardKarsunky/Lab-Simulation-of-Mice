#include "CellHandler.hpp"
#include "CellLiverCancer.hpp"
#include <Env/Organ.hpp>
#include <Application.hpp>

CellHandler::CellHandler(CellCoord position, Organ* organ)
	: position(position),
	  organ(organ),
	  cellule_ECM(new CellECM(this)),
	  cellule_foie(nullptr),
	  cellule_sang(nullptr),
	  cancer(false) {}

CellHandler::~CellHandler()
{
	if (cellule_ECM != nullptr) {
		delete cellule_ECM;
	}
	if (cellule_foie != nullptr) {
		delete cellule_foie;
	}
	if(cellule_sang != nullptr) {
		delete cellule_sang;
	}
}

//----------------------------------------------------------------------

int CellHandler::getOrganNbCells() const {
	return organ->getNbCells();
}

double CellHandler::getDeltaGlucose() const {
	return organ->getDeltaGlucose();
}

double CellHandler::getDeltaBromo() const {
	return organ->getDeltaBromo();
}

double CellHandler::getDeltaVGEF() const {
	return organ->getDeltaVGEF();
}
	
bool CellHandler::isOut(const CellCoord& coord) const {
	return (organ->isOut(coord));
}

bool CellHandler::hasECM() const {
	return (cellule_ECM != nullptr);
}

bool CellHandler::hasLiver() const {
	return (cellule_foie != nullptr);
}

bool CellHandler::hasBlood() const {
	return (cellule_sang != nullptr);
}

void CellHandler::setECM() {
	if (cellule_ECM == nullptr) {
		cellule_ECM = new CellECM(this);
	}
}

void CellHandler::setLiver() {
	if (cellule_foie == nullptr) {
		cellule_foie = new CellLiver(this);
		organ->updateRepresentationAt(getPosition());
	}
}

void CellHandler::setBlood(TypeBloodCell type) {
	if (cellule_sang == nullptr) {
		cellule_sang = new CellBlood(this, type);
		organ->updateRepresentationAt(getPosition());
	}
}

void CellHandler::update(sf::Time dt) {
	cellule_ECM->update(dt);
	if (cellule_foie != nullptr) {
		cellule_foie->update(dt);
		if (cellule_foie->getATP() <= 0.0) {
			delete cellule_foie;
			cellule_foie = nullptr;
			cancer = false;
		}
	}
	
	if (cellule_sang != nullptr) {
		cellule_sang->update(dt);
	}
	
	organ->updateRepresentationAt(getPosition());
}

void CellHandler::updateSubstance(const Substance& substance) {
	cellule_ECM->updateSubstance(substance);
	
	organ->updateRepresentationAt(getPosition());
}

void CellHandler::updateCellHandlerAt(const CellCoord& pos, const Substance& diffusedSubst) {
	organ->updateCellHandlerAt(pos, diffusedSubst);
}

double CellHandler::getQuantity(CellOrgan* cellule, SubstanceId id) const {
	switch (id)
	{
		case GLUCOSE:
			return cellule->getGlucose();
			break;
		case VGEF:
		    return cellule->getVGEF();
			break;
		case BROMOPYRUVATE:
		    return cellule->getInhibitor();
			break;
	}
}

double CellHandler::getECMQuantity(SubstanceId id) const {
	return getQuantity(cellule_ECM, id);		
}

double CellHandler::getLiverQuantity(SubstanceId id) const {
	return getQuantity(cellule_foie, id);	
}

double CellHandler::getBloodQuantity(SubstanceId id) const {
	return getQuantity(cellule_sang, id);	
}

CellCoord CellHandler::getPosition() const {
	return position;
}

void CellHandler::liverTakeFromEcm(SubstanceId id, double fraction) {
	cellule_ECM->uptakeOnGradient(fraction, cellule_foie, id);
}

void CellHandler::setCancer() {
	if (!cancer) {
		if (cellule_foie != nullptr) {
			delete cellule_foie;
			cellule_foie = nullptr;
		}
		cancer = true;
		cellule_foie = new CellLiverCancer(this);
		
		organ->updateRepresentationAt(getPosition());
	}
}

bool CellHandler::hasCancer() const {
	return cancer;
}

void CellHandler::expandLiver(const CellCoord& current_position) {
	organ->expandLiver(current_position);
}

void CellHandler::expandCancer(const CellCoord& current_position) {
	organ->expandCancer(current_position);
}
