#include "Organ.hpp"
#include <Env/CellHandler.hpp>
#include <Application.hpp>
#include <Utility/Vertex.hpp>
#include <algorithm>
#include <Random/Random.hpp>
#include <Utility/Constants.hpp>
#include <string>

Organ::Organ(bool generation)
	: currentSubst(GLUCOSE),
	  deltaGlucose(0.0),
	  deltaVGEF(0.0),
	  deltaBromo(0.0)
	  { 
		if (generation) {
			generate();
	    }
	  }

Organ::~Organ()
	{
		for (auto& colonne : cellHandlers) {
			for (auto& cellHandler : colonne) {
				if (cellHandler != nullptr) {
					delete cellHandler;
					cellHandler = nullptr;
				}
			}
		}
		
		cellHandlers.clear();
	}

//----------------------------------------------------------------------

int Organ::getWidth() const {
	return getAppConfig().simulation_organ_size;
}

int Organ::getHeight() const {
	return getAppConfig().simulation_organ_size;
}

int Organ::getNbCells() const {
	return getAppConfig().simulation_organ_nbCells;
}

SubstanceId Organ::getCurrentSubst() const {
	return currentSubst;
}

void Organ::setCurrentSubst(SubstanceId substance_) {
	currentSubst = substance_;
}

double Organ::getDeltaGlucose() const {
	return deltaGlucose;
}

double Organ::getDeltaVGEF() const {
	return deltaVGEF;
}

double Organ::getDeltaBromo() const {
	return deltaBromo;
}

void Organ::setDeltaGlucose(double delta_glucose) {
	deltaGlucose = delta_glucose;
}

void Organ::setDeltaVGEF(double delta_VGEF) {
	deltaVGEF = delta_VGEF;
}

void Organ::setDeltaBromo(double delta_bromo) {
	deltaBromo = delta_bromo;
}

double Organ::getConcentrationAt(const CellCoord& pos, SubstanceId id) const {
	return cellHandlers[pos.x][pos.y]->getECMQuantity(id);
}
				
void Organ::update() {
	for (auto& colonne : cellHandlers) {
		for (auto& cellHandler : colonne) {
			cellHandler->update(sf::seconds(getAppConfig().simulation_fixed_step));
		}
	}
	
	updateRepresentation(false);
}

void Organ::drawOn(sf::RenderTarget& target) {
	sf::Sprite image(renderingCache.getTexture()); //! transforme l'image en texture
	target.draw(image); //! affiche la texture
}

void Organ::generate() {
	reloadConfig();
	reloadCacheStructure();
	createLiver();
	createBloodSystem();
	updateRepresentation();
}

void Organ::reloadConfig() {
	nbCells = getAppConfig().simulation_organ_nbCells;
	cellSize = getWidth()/nbCells;
	
	cellHandlers.resize(nbCells);
	
	for (int i(0); i < nbCells; ++i) {
		for (int k(0); k < nbCells; ++k) {
			cellHandlers[i].push_back(new CellHandler({i, k}, this));
		}
	}
}

void Organ::reloadCacheStructure() {
	renderingCache.create((nbCells * cellSize), (nbCells * cellSize));
	
	const auto& Vertexes = generateVertexes(getAppConfig().simulation_organ["textures"], nbCells, cellSize);
	
	bloodVertexes = Vertexes;
	liverVertexes = Vertexes;
	concentrationVertexes = Vertexes;
	liverCancerVertexes = Vertexes;
}

void Organ::createLiver() {
	for (int x(0); x < nbCells; ++x) {
		for (int y(0); y < nbCells; ++y) {
			if (isInLiver({x, y})) {
				cellHandlers[x][y]->setLiver();
			}
		}
	}
}

void Organ::createBloodSystem(bool createCapillaries) {
	size_t SIZE_ARTERY(std::max((0.03 * nbCells), 1.0));
	size_t START_CREATION_FROM(getAppConfig().blood_creation_start);
	size_t DISTANCE_MIN(getAppConfig().blood_capillary_min_dist);
	size_t DIS_X_LEFT((nbCells/2) - (SIZE_ARTERY/2));
	size_t DIS_X_RIGHT((nbCells/2) + (SIZE_ARTERY/2) + 1);
	size_t NB_CAPILLARY_RIGHT(0);
	size_t NB_CAPILLARY_LEFT(0);
	int m, n; //! Pour ensuite appeler la fonction 'updateCellHandler' qui ne prend que des int pour une CellCoord
	
	std::vector<CellCoord> STARTS_RIGHT; //! Ensemble de départ des capillaires de droite
	std::vector<CellCoord> STARTS_LEFT; //! Ensemble de départ des capillaires de gauche
	
	if (SIZE_ARTERY % 2 == 0) {
		DIS_X_RIGHT -= 1;
	}
	
	generateArtery(DIS_X_LEFT, DIS_X_RIGHT);
	
	if (createCapillaries) {
		bool isFarEnough(true);

		for (size_t y(START_CREATION_FROM); y < size_t(nbCells - 1); ++y) {
			for (size_t i(1); i < DISTANCE_MIN; ++i) {
				if (cellHandlers[(DIS_X_LEFT - 1)][y - i]->hasBlood()) {
					isFarEnough = false;
				}
			}
			if (isFarEnough and (uniform(1, 3) == 3) and (NB_CAPILLARY_LEFT <= ((nbCells - START_CREATION_FROM)/3))) {
				m = y;
				n = DIS_X_LEFT - 1;
				updateCellHandler({n, m}, Organ::Kind::Capillary);
				++NB_CAPILLARY_LEFT;
				STARTS_LEFT.push_back({n, m});
			}
		
			isFarEnough = true;
			for (size_t i(1); i < DISTANCE_MIN; ++i) {
				if (cellHandlers[(DIS_X_RIGHT + 1)][y - i]->hasBlood()) {
					isFarEnough = false;
				}
			}
			if (isFarEnough and (uniform(1, 3) == 3) and (NB_CAPILLARY_RIGHT <= ((nbCells - START_CREATION_FROM)/3))) {
				m = y;
				n = DIS_X_RIGHT + 1;
				updateCellHandler({n, m}, Organ::Kind::Capillary);
				++NB_CAPILLARY_RIGHT;
				STARTS_RIGHT.push_back({n, m});
			}
			
			isFarEnough = true;
		}
		
		for (auto coord : STARTS_RIGHT) {
			generateCapillaryFromPosition(coord, {1, 0});
		}
		for (auto coord : STARTS_LEFT) {
			generateCapillaryFromPosition(coord, {-1, 0});
		}
	}
}

void Organ::generateCapillaryOneStep(CellCoord& current_position, const CellCoord& dir,
									 int& nbCells_, const int& maxLength) {
	
	CellCoord POSITION_UP({current_position.x, current_position.y - 1});
	CellCoord POSITION_DOWN({current_position.x, current_position.y + 1});
	CellCoord dir_tmp({current_position.x + dir.x, current_position.y});
	std::vector<CellCoord> COORDONNEES({dir_tmp, dir_tmp, dir_tmp, dir_tmp, POSITION_UP, POSITION_DOWN});
	CellCoord COORDONNEE_PROB(uniform(COORDONNEES));
	
	if (nbCells_ != maxLength) {
		if (not((cellHandlers[POSITION_UP.x][POSITION_UP.y]->hasBlood())
			and (cellHandlers[POSITION_DOWN.x][POSITION_DOWN.y]->hasBlood())
			and (cellHandlers[dir_tmp.x][dir_tmp.y]->hasBlood()))) {
		
			while (cellHandlers[COORDONNEE_PROB.x][COORDONNEE_PROB.y]->hasBlood()) {
				   COORDONNEE_PROB = uniform(COORDONNEES);
			}
			
			if (COORDONNEE_PROB.y >= (nbCells - 1)) {
				if ((!cellHandlers[COORDONNEE_PROB.x][COORDONNEE_PROB.y - 2]->hasBlood())
					or (!cellHandlers[dir_tmp.x][dir_tmp.y]->hasBlood())) {
						
					while (COORDONNEE_PROB.y >= (nbCells - 1)) {
						COORDONNEE_PROB = uniform(COORDONNEES);
					}
					
				} else {
					COORDONNEE_PROB = current_position;
				}
			}
			
			if (COORDONNEE_PROB.y >= getAppConfig().blood_creation_start) {
				if (current_position != COORDONNEE_PROB) {
					current_position = COORDONNEE_PROB;
					cellHandlers[COORDONNEE_PROB.x][COORDONNEE_PROB.y]->setBlood(CAPILLARY);
					++nbCells_;
				}
			}
		}
	}
}


void Organ::generateCapillaryFromPosition(CellCoord& current_position, CellCoord dir) {
	int LENGTH_CAPILLARY(1);
	int maxLength((nbCells/2) - 4);
	
	for (int i(1); i <= maxLength; ++i) {
		generateCapillaryOneStep(current_position, dir, LENGTH_CAPILLARY, maxLength);
	}
}		

void Organ::updateRepresentation(bool situation) {
	if (situation) {
		for (int x(0); x < nbCells; ++x) {
			for (int y(0); y < nbCells; ++y) {
				updateRepresentationAt({x, y});
			}
		}
	}
	
	if (getApp().isConcentrationOn()) {
		renderingCache.clear(sf::Color(0,0,0));
		
		sf::RenderStates rs_concentration;
		const auto& textures = getAppConfig().simulation_organ["textures"];
		
		std::string nom_substance_courante;
		switch (currentSubst) {
			case GLUCOSE :
				nom_substance_courante = "glucose";
				break;
			case VGEF :
				nom_substance_courante = "vgef";
				break;
			case BROMOPYRUVATE :
				nom_substance_courante = "bromopyruvate";
				break;
		}
		
		rs_concentration.texture = &getAppTexture(textures[nom_substance_courante].toString());
		renderingCache.draw(concentrationVertexes.data(), concentrationVertexes.size(), sf::Quads, rs_concentration);
		
	} else {
		renderingCache.clear(sf::Color(223,196,176));
	}
	
	sf::RenderStates rs_blood;
	sf::RenderStates rs_liver;
	sf::RenderStates rs_cancer;
	auto textures = getAppConfig().simulation_organ["textures"];
	
	rs_blood.texture = &getAppTexture(textures["blood"].toString()); //! ici pour la texture liée une cellule sanguine
	renderingCache.draw(bloodVertexes.data(), bloodVertexes.size(), sf::Quads, rs_blood);
	
	rs_liver.texture = &getAppTexture(textures["liver"].toString()); //! ici pour la texture liée une cellule hépatique
	renderingCache.draw(liverVertexes.data(), liverVertexes.size(), sf::Quads, rs_liver);
	
	rs_cancer.texture = &getAppTexture(textures["cancer"].toString()); //! texture liée à une cellule hépatique avec cancer
	renderingCache.draw(liverCancerVertexes.data(), liverCancerVertexes.size(), sf::Quads, rs_cancer);
	
	renderingCache.display();
}

void Organ::updateRepresentationAt(const CellCoord& coord) {
	if (cellHandlers[coord.x][coord.y]->hasBlood()) {
		for (auto index : indexesForCellVertexes(coord.x, coord.y, nbCells)) {
			liverVertexes[index].color.a = 0;
			bloodVertexes[index].color.a = 255;
		}
	} else {
		if ((cellHandlers[coord.x][coord.y]->hasLiver()) and (!getApp().isConcentrationOn())) {
			for (auto index : indexesForCellVertexes(coord.x, coord.y, nbCells)) {
				bloodVertexes[index].color.a = 0;
				liverVertexes[index].color.a = 255;
			}
		} else {
			for (auto index : indexesForCellVertexes(coord.x, coord.y, nbCells)) {
				liverVertexes[index].color.a = 0;
				bloodVertexes[index].color.a = 0;
			}
		}
	}
	
	double ratio(cellHandlers[coord.x][coord.y]->getECMQuantity(currentSubst) / getAppConfig().substance_max_value);
	for (auto index : indexesForCellVertexes(coord.x, coord.y, nbCells)) {
		concentrationVertexes[index].color.a = std::max(int(ratio * 255), 5);
	}
	
	if ((cellHandlers[coord.x][coord.y]->hasCancer()) and (!cellHandlers[coord.x][coord.y]->hasBlood())) {
		for (auto index : indexesForCellVertexes(coord.x, coord.y, nbCells)) {
			liverCancerVertexes[index].color.a = 255;
		}
	} else {
		for (auto index : indexesForCellVertexes(coord.x, coord.y, nbCells)) {
			liverCancerVertexes[index].color.a = 0;
		}
	}
}

void Organ::updateCellHandler(const CellCoord& pos, Kind kind) {
	switch (kind)
	{
		case Organ::Kind::ECM:
			cellHandlers[pos.x][pos.y]->setECM();
			break;
		case Organ::Kind::Liver:
			cellHandlers[pos.x][pos.y]->setLiver();
			break;
		case Organ::Kind::Artery:
			cellHandlers[pos.x][pos.y]->setBlood(ARTERY);
			break;
		case Organ::Kind::Capillary:
			cellHandlers[pos.x][pos.y]->setBlood(CAPILLARY);
			break;
	}
}

void Organ::updateCellHandlerAt(const CellCoord& pos, const Substance& diffusedSubst) {
	cellHandlers[pos.x][pos.y]->updateSubstance(diffusedSubst);
}

bool Organ::isOut(const CellCoord& coord) const {
    return ((coord.x >= nbCells) or (coord.x < 0.0) or (coord.y >= nbCells) or (coord.y < 0.0));
}

bool Organ::isInLiver(const CellCoord& coord) const {
	return ((((coord.x * coord.x) + (coord.y - nbCells) * (coord.y - nbCells)) < ((nbCells + 1) * (nbCells + 1)))
			and ((((coord.x - nbCells) * (coord.x - nbCells) + (coord.y * coord.y)) < ((nbCells + 1) * (nbCells + 1)))));
}

CellCoord Organ::toCellCoord(const Vec2d& coord) const {
    return vec2dToCellCoord(coord, getWidth(), getHeight(), cellSize);
}

void Organ::generateArtery(int distance_left, int distance_right) {
	for (int x(distance_left); x <= distance_right; ++x) {
		for (int y(0); y < nbCells; ++y) {
			updateCellHandler({x, y}, Organ::Kind::Artery);
		}
	}
}

CellCoord Organ::indexForCell(const Vec2d& pos) const {
	return CellCoord(pos/cellSize);
}

void Organ::setCancerAt(const Vec2d& pos) {
	CellCoord tmp(indexForCell(pos));
	if (!isOut(tmp)) {
		cellHandlers[tmp.x][tmp.y]->setCancer();
	}
}

void Organ::expandLiver(const CellCoord& current_position) {
	std::vector<CellCoord> next_direction;
	
	if ((current_position.x < nbCells - 1) and isInLiver({current_position.x + 1, current_position.y})
		and (!cellHandlers[current_position.x + 1][current_position.y]->hasLiver())) {
		next_direction.push_back({current_position.x + 1, current_position.y});
	}
	if ((current_position.x > 1) and isInLiver({current_position.x - 1, current_position.y})
		and (!cellHandlers[current_position.x - 1][current_position.y]->hasLiver())) {
		next_direction.push_back({current_position.x - 1, current_position.y});
	}
	if ((current_position.y < nbCells - 1) and isInLiver({current_position.x, current_position.y + 1}) 
		and (!cellHandlers[current_position.x][current_position.y + 1]->hasLiver())) {
		next_direction.push_back({current_position.x, current_position.y + 1});
	}
	if ((current_position.y > 1) and isInLiver({current_position.x, current_position.y - 1})
		and (!cellHandlers[current_position.x][current_position.y - 1]->hasLiver())) {
		next_direction.push_back({current_position.x, current_position.y - 1});
	}
	
	if (!next_direction.empty()) {
		CellCoord coord_tmp(uniform(next_direction));
		cellHandlers[coord_tmp.x][coord_tmp.y]->setLiver();
	}
}

void Organ::expandCancer(const CellCoord& current_position) {
	std::vector<CellCoord> next_direction;
	
	if (current_position.x < nbCells - 1) {
		next_direction.push_back({current_position.x + 1, current_position.y});
	}
	if (current_position.x > 1) {
		next_direction.push_back({current_position.x - 1, current_position.y});
	}
	if (current_position.y < nbCells - 1) {
		next_direction.push_back({current_position.x, current_position.y + 1});
	}
	if (current_position.y > 1) {
		next_direction.push_back({current_position.x, current_position.y - 1});
	}
	
	if (!next_direction.empty()) {
		CellCoord coord_tmp(uniform(next_direction));
		cellHandlers[coord_tmp.x][coord_tmp.y]->setCancer();
	}
}
