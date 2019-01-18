#ifndef CELLECM_H
#define CELLECM_H

#include "CellOrgan.hpp"
#include <SFML/Graphics.hpp>

class CellECM : public CellOrgan {
public:
	CellECM(CellHandler* strate);
	
	void update(sf::Time dt) override;
};

#endif
