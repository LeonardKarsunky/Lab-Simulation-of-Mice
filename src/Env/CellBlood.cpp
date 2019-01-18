#include "CellBlood.hpp"
#include <Application.hpp>
#include <cmath>
#include <Utility/Vec2d.hpp>
#include <Utility/Utility.hpp>
#include "Substance.hpp"
#include "CellHandler.hpp"

CellBlood::CellBlood(CellHandler* strate, TypeBloodCell type)
	: CellOrgan(strate),
	  type(type) {}

//----------------------------------------------------------------------

void CellBlood::update(sf::Time dt) {
	if (type == CAPILLARY) {
		Substance substance_init(strate->getDeltaVGEF(), getAppConfig().base_glucose + strate->getDeltaGlucose(), 
		                         getAppConfig().base_bromo + strate->getDeltaBromo());
		*substance = substance_init;
		
		Substance substance_diff;
		double RAYON_DIFFUSION(getAppConfig().substance_diffusion_radius);
		
		for (int i(-RAYON_DIFFUSION); i <= RAYON_DIFFUSION; ++i) {
			for (int j(-RAYON_DIFFUSION); j <= RAYON_DIFFUSION; ++j) {
				
				if (!strate->isOut(getPosition() + CellCoord(i, j))) {
					substance_diff = substance_init * (0.5 * (1 - std::erf(Vec2d(i, j).length()/
									 std::sqrt(4.0 * getAppConfig().substance_diffusion_constant * dt.asSeconds()))));
					if (!substance_diff.isNull()) {
						CellCoord current_position(getPosition());
						strate->updateCellHandlerAt({current_position.x + i, current_position.y + j}, substance_diff);
					}
				}
			}
		}
	}
}
