#include "Lab.hpp"
#include <Application.hpp>
#include <algorithm>
#include <iostream>

Lab::Lab()
	: animal_tracked(nullptr)
	{ 
		makeBoxes(getAppConfig().simulation_lab_nb_boxes);
    }

Lab::~Lab()
	{
		destroyBoxes();
		reset();
	}

//----------------------------------------------------------------------

void Lab::makeBoxes(unsigned int nbCagesPerRow) {
	if (nbCagesPerRow <= 0) {
		throw std::invalid_argument("Nombre de cages doit être plus grand que zéro");
	} else {
		boites.resize(nbCagesPerRow);
		
		for (auto& colonne : boites) {
			for (size_t i(0); i < nbCagesPerRow; ++i) {
				colonne.push_back(new Box({0.0, 0.0}));
			}
		}
		
		double longueur_boite((getApp().getLabSize().x)/nbCagesPerRow);
		for (size_t i(0); i < nbCagesPerRow; ++i) {
			for (size_t k(0); k < nbCagesPerRow; ++k) {
				boites[i][k]->setWidth(longueur_boite);
				boites[i][k]->setHeight(longueur_boite);
				boites[i][k]->setWallWidth(0.05 * (longueur_boite/2));
				boites[i][k]->setCenter({(i * longueur_boite) + (longueur_boite/2), 
										 (k * longueur_boite) + (longueur_boite/2)});
				boites[i][k]->setWalls();
			}
		}
    }
}

void Lab::destroyBoxes() {
	for (auto& colonne : boites) {
        for (auto& boite : colonne) {
            delete boite;
		}
	}
	
	boites.clear();
}

void Lab::update(sf::Time dt) {
	for (auto& entite : lesEntites) {
		if (entite != nullptr) {
			entite->update(dt);
			if (entite->getEnergy() == 0.0) {
				entite = nullptr;
				std::swap(entite, lesEntites.back());
				lesEntites.pop_back();
			}
		}
	}
	
	
	for (auto& animal : animals) {
		if (animal != nullptr) {
			if (animal->getEnergy() == 0.0) {
				if (animal->isBeingTracked()) {
					animal_tracked = nullptr;
					switchToView(LAB);
				}
				for (auto colonne : boites) {
					for (auto boite : colonne) {
						if (boite->isPositionInside(animal->getCenter())) {
							boite->reset();
						}
					}
				}
				delete animal;
				animal = nullptr;
				std::swap(animal, animals.back());
				animals.pop_back();
			}
		}
	}
	for (auto& cheese : cheeses) {
		if (cheese != nullptr) {
			if (cheese->getEnergy() == 0.0) {
				delete cheese;
				cheese = nullptr;
				std::swap(cheese, cheeses.back());
				cheeses.pop_back();
			}
		}
	}
	
	/*!
	 * @brief Si on enlève les boites, il faut aussi enlever les souris
	 */
	if (boites.size() == 0) {
		reset();
	}
}

void Lab::updateTrackedAnimal() {
	if (animal_tracked != nullptr) {
		animal_tracked->updateOrgan();
	}
}

void Lab::drawOn(sf::RenderTarget& targetWindow) const {
	for (auto const& colonne : boites) {
		for (auto const& boite : colonne) {
			boite->drawOn(targetWindow);
		}
    }
    
    for (auto const& entite : lesEntites) {
		if (entite != nullptr) {
			entite->drawOn(targetWindow);
		}
	}
}

void Lab::drawCurrentOrgan(sf::RenderTarget& target) const {
	if (animal_tracked != nullptr) {
		animal_tracked->drawOrgan(target);
	}
}

void Lab::reset() {
	for (auto& colonne : boites) {
		for (auto& boite : colonne) {
			boite->reset();
		}
	}
	
	for (auto& entite : lesEntites) {
		if (entite != nullptr) {
			entite->resetBox();
			entite = nullptr;
		}
	}
	
	for (auto& animal : animals) {
		if (animal != nullptr) {
			if (animal->isBeingTracked()) {
				animal->setTrack(false);
				animal_tracked = nullptr;
			}
			delete animal;
			animal = nullptr;
		}
	}
	for (auto& cheese : cheeses) {
		if (cheese != nullptr) {
			delete cheese;
			cheese = nullptr;
		}
	}
	
	lesEntites.erase(std::remove(lesEntites.begin(), lesEntites.end(), nullptr), lesEntites.end());
	lesEntites.clear();
	animals.erase(std::remove(animals.begin(), animals.end(), nullptr), animals.end());
	animals.clear();
	cheeses.erase(std::remove(cheeses.begin(), cheeses.end(), nullptr), cheeses.end());
	cheeses.clear();
}

bool Lab::addEntity(SimulatedEntity* entite) {
	if (entite != nullptr) {
		for (auto& colonne : boites) {
			for (auto& boite : colonne) {
				if (entite->canBeConfinedIn(boite)) {
					entite->placeEntity(boite);
					lesEntites.push_back(entite);
					return true;
				}
			}
		}
		
		if (entite->getBox() == nullptr) {
			delete entite;
		}
	}
	
	return false;
}

bool Lab::addAnimal(Animal* animal) {
	if (animal != nullptr) {
		if (addEntity(animal)) {
			animals.push_back(animal);
			return true;
		}
	}
	
	return false;
}

bool Lab::addCheese(Cheese* cheese) {
	if (cheese != nullptr) {
		if (addEntity(cheese)) {
			cheeses.push_back(cheese);
			return true;
		}
	}
	
	return false;
}

SimulatedEntity* Lab::closestEntity(Animal const* entity) {
	if (lesEntites.size() == 1) {
		return nullptr;
	}
	
	SimulatedEntity* entite_tmp(nullptr);
	
	for (size_t i(0); i < lesEntites.size(); ++i) {
		if (entity->isTargetInSight(lesEntites[i]->getCenter()) and entity->eatable(lesEntites[i])) {
			if (i == 0) {
				entite_tmp = lesEntites[i];
			} else {
				if (entite_tmp != nullptr) {
					if (sqrt((entity->getCenter() - lesEntites[i]->getCenter()).lengthSquared()) 
						< sqrt((entity->getCenter() - entite_tmp->getCenter()).lengthSquared())) {
						entite_tmp = lesEntites[i];
					}
				} else {
					entite_tmp = lesEntites[i];
				}
			}
		}
	}

	return entite_tmp;
}

void Lab::trackAnimal(Animal* animal) {
	if (animal_tracked != nullptr) {
		animal->setTrack(false);
	}
	animal_tracked = animal;
	animal->setTrack(true);
}

void Lab::trackAnimal(const Vec2d& position_cursor) {
	for (auto& animal : animals) {
		if (animal != nullptr) {
			if ((animal->getDistance(position_cursor - animal->getCenter()) < animal->getRadius())
				and (animal_tracked == nullptr)) {

				trackAnimal(animal);
				animal_tracked = animal;
			}
		}
	}
}

void Lab::stopTrackingAnyEntity() {
	for (auto& animal : animals) {
		if (animal != nullptr) {
			if (animal->isBeingTracked()) {
				animal->setTrack(false);
				animal_tracked = nullptr;
			}
		}
	}
}

void Lab::switchToView(View view) {
	if (view != LAB) {
		if (animal_tracked != nullptr) {
			getApp().switchToView(view);
		}
	} else {
		getApp().switchToView(view);
	}
}

void Lab::nextSubstance() {
	if (animal_tracked != nullptr) {
		SubstanceId substance_tmp(animal_tracked->getCurrentSubst());
		
		switch (substance_tmp) {
			case GLUCOSE:
				animal_tracked->setCurrentSubst(BROMOPYRUVATE);
				break;
			case BROMOPYRUVATE:
				animal_tracked->setCurrentSubst(VGEF);
				break;
			case VGEF:
				animal_tracked->setCurrentSubst(GLUCOSE);
				break;
		}
	}
}

void Lab::increaseCurrentSubst() {
	if (animal_tracked != nullptr) {
		switch (animal_tracked->getCurrentSubst()) {
			case GLUCOSE:
				animal_tracked->setDeltaGlucose(delta_borne(animal_tracked->getDeltaGlucose() + getAppConfig().delta_glucose));
				break;
			case BROMOPYRUVATE:
				animal_tracked->setDeltaBromo(delta_borne(animal_tracked->getDeltaBromo() + getAppConfig().delta_bromo));
				break;
			case VGEF:
				animal_tracked->setDeltaVGEF(delta_borne(animal_tracked->getDeltaVGEF() + getAppConfig().delta_vgef));
				break;
		}
	}		
}

void Lab::decreaseCurrentSubst() {
	if (animal_tracked != nullptr) {
		switch (animal_tracked->getCurrentSubst()) {
			case GLUCOSE:
				animal_tracked->setDeltaGlucose(delta_borne(animal_tracked->getDeltaGlucose() - getAppConfig().delta_glucose));
				break;
			case BROMOPYRUVATE:
				animal_tracked->setDeltaBromo(delta_borne(animal_tracked->getDeltaBromo() - getAppConfig().delta_bromo));
				break;
			case VGEF:
				animal_tracked->setDeltaVGEF(delta_borne(animal_tracked->getDeltaVGEF() - getAppConfig().delta_vgef));
				break;
		}	
	}
}

double Lab::getDelta(SubstanceId id) const {
	switch (id) {
		case GLUCOSE:
			return animal_tracked->getDeltaGlucose();
			break;
		case BROMOPYRUVATE:
			return animal_tracked->getDeltaBromo();
			break;
		case VGEF:
			return animal_tracked->getDeltaVGEF();
			break;
	}
}

SubstanceId Lab::getCurrentSubst() const {
	return animal_tracked->getCurrentSubst();
}

double Lab::delta_borne(double deltaSubst) {
	if (deltaSubst > getAppConfig().substance_max_value) {
		return getAppConfig().substance_max_value;
	}
	
	if (deltaSubst < (-getAppConfig().substance_max_value)) {
		return (-getAppConfig().substance_max_value);
	}
	
	return deltaSubst;
}

void Lab::setCancerAt(const Vec2d& pos) {
	if (animal_tracked != nullptr) {
		animal_tracked->setCancerAt(pos);
	}
}

