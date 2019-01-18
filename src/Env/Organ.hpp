#ifndef ORGAN_H
#define ORGAN_H

#include "Animal.hpp"
#include "Substance.hpp"
#include <SFML/Graphics.hpp>
#include <Utility/Utility.hpp>
#include <vector>
#include "Types.hpp"

class CellHandler;
class Organ {
public:
	enum class Kind : short { ECM, Liver, Artery, Capillary };
	
	Organ(bool generation = true);
	virtual ~Organ();

	int getWidth() const;
	int getHeight() const;
	int getNbCells() const;
	
	SubstanceId getCurrentSubst() const;
	void setCurrentSubst(SubstanceId substance_);
	
	double getDeltaGlucose() const;
	double getDeltaVGEF() const;
	double getDeltaBromo() const;
	
	void setDeltaGlucose(double delta_glucose);
	void setDeltaVGEF(double delta_VGEF);
	void setDeltaBromo(double delta_bromo);
	
	double getConcentrationAt(const CellCoord& pos, SubstanceId id) const;
	
	/*!
	 * @brief Dessine le contenu de l'organ
	 */
	void drawOn(sf::RenderTarget& target);
	
	/*!
	 * @brief Fait évoluer l'organe, donc l'ensemble des cellules qui le constituent
	 */
	void update();
	
	/*!
	 * @brief Permet la mise à jour de la représentation (image) associée
	 * à l'organe à chaque cycle de simulation
	 */
	void updateRepresentation(bool situation = true);
	
	/*!
	 * @brief Permet la mise à jour d'une case
	 */
	virtual void updateRepresentationAt(const CellCoord& coord);
	   
	/*!
	 * @brief Vérifie si une case est en dehors de la grille ou non
	 * 
	 * @param coord la position d'une case (un CellCoord)
	 * @return true si la case est en dehors de la grille et false sinon
	 */
	bool isOut(const CellCoord& coord) const;
	
	/*!
	 * @brief Permet de voir si une case logique est dans la zone qui va
	 * devenir le foie
	 * 
	 * @brief Toute case comprise entre l'arc de cercle de centre (0,nbCells)
	 * et de de rayon nbCells et l'arc de cercle de centre (nbCells,0)
	 * et de rayon nbCells sera considérée comme faisant partie du foie
	 */
	bool isInLiver(const CellCoord& coord) const;
	
	/*!
	 * @brief Permet de trouver la case de la grille contenant le point pos,
	 * donc transforme une position physique en une position logique
	 * 
	 * @param coord une position physique dans la grille (un Vec2d)
	 * @return la case, une position logique, dans laquelle se trouve le point pos
	 */
	virtual CellCoord toCellCoord(const Vec2d& coord) const;
	
	/*!
	 * @brief Permet de générer l'artère
	 */
	virtual void generateArtery(int distance_left, int distance_right);
	
	/*!
	 * @brief Permet d'ajouter diffusedSubst au niveau ECM du CellHandler
	 * à la position logique pos
	 */
	virtual void updateCellHandlerAt(const CellCoord& pos, const Substance& diffusedSubst);
	
	/*!
	 * @brief Convertit une position physique en une position logique
	 */
	CellCoord indexForCell(const Vec2d& pos) const;
	
	/*!
	 * @brief Permet mettre une cellule cancéreuse dans l'organe de l'animal
	 * traqué à la position physique pos
	 */
	void setCancerAt(const Vec2d& pos);
	
	/*!
	 * @brief Permet de faire accroître le foie en faisant diviser ses cellules
	 */
	void expandLiver(const CellCoord& current_position);
	
	/*!
	 * @brief Permet de faire accroître la tumeur en faisant diviser les
	 * cellules cancéreuses
	 */
	void expandCancer(const CellCoord& current_position);

protected:
	/*!
	 * @brief Permet d'initialiser l'organ
	 */
	virtual void generate();
	
	/*!
	 * @brief Permet d'initialiser le nombre de cellules par ligne et la
	 * taille graphique de chaque cellule
	 */
	void reloadConfig();
	
	/*!
	 * @brief Permet d'initialiser l'attribut renderingCache
	 */
	void reloadCacheStructure();
	
	/*!
	 * @brief Permet de générer le foie
	 */
	void createLiver();
	
	/*!
	 * @brief Permet de générer le système sanguin
	 */
	void createBloodSystem(bool createCapillaries = true);
	
	/*!
	 * @brief Permet de faire croître un capillaire d'une seule case dans la direction dir
	 */
	virtual void generateCapillaryOneStep(CellCoord& current_position, const CellCoord& dir,
										  int& nbCells_, const int& maxLength);
										  
	/*!
	 * @brief Utilise generateCapillaryOneStep pour générer un capillaire entier
	 */
	virtual void generateCapillaryFromPosition(CellCoord& current_position, CellCoord dir);
	
	/*!
	 * @brief Dote le CellHandler à la position pos d'une cellule du type
	 * approprié selon la valeur de kind
	 */
	virtual void updateCellHandler(const CellCoord& pos, Kind kind);

	//! Le nombre de cellules par ligne
	int nbCells;
	
	//! La taille graphique de chaque cellule
	float cellSize;
	
	//! L'image associée à l'organe pour son dessin
	sf::RenderTexture renderingCache;
	
	//! Substance observée dans la vue CONCENTRATION
	SubstanceId currentSubst;
	
	//! La quantité de Glucose initialement diffusées par les cellules sanguines
	double deltaGlucose;
	
	//! La quantité de VGEF initialement diffusées par les cellules sanguines
	double deltaVGEF;
	
	//! La quantité de Bromopyruvate initialement diffusées par les cellules sanguines
	double deltaBromo;
	
	//! Le tableau de CellHandler (strates)
	std::vector<std::vector<CellHandler*> > cellHandlers;
	
	//! L'ensemble des sommets représentant des cellules sanguines
	std::vector<sf::Vertex> bloodVertexes;
	
	//! L'ensemble des sommets représentant des cellules hépatiques
	std::vector<sf::Vertex> liverVertexes;
	
	//! L'ensemble des sommets représentant des cellules hépatiques cancéreuses
	std::vector<sf::Vertex> liverCancerVertexes;
	
	//! L'ensemble des sommets représentant des carrés montrant la
	//! quantité de substance (concentration) au niveau ECM de chaque case
	std::vector<sf::Vertex> concentrationVertexes;
};

#endif
