#ifndef CELLBLOOD_H
#define CELLBLOOD_H

#include "CellOrgan.hpp"
#include "Types.hpp"
#include <SFML/Graphics.hpp>

class CellBlood : public CellOrgan {
public:
	CellBlood(CellHandler* strate, TypeBloodCell type = ARTERY);
	
	/*!
	 * @brief Permet de simuler le fait que la CellBlood diffuse des substances
	 */
	void update(sf::Time dt) override;
	
private:
	//! Le type de cette cellule sanguine, soit ARTERY soit CAPILLARY
	TypeBloodCell type;
};

#endif
