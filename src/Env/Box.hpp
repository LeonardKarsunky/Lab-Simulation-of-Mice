#ifndef BOX_H
#define BOX_H

#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>

typedef std::pair<Vec2d, Vec2d> Wall; //! bottom right corner, top left corner

/*!
 * @class Box
 * 
 * @brief Un Box est une boîte d'un laboratoire dans lequel sera élevé
 * une souris
 */
class Box {
public:
	Box();
	
	/*! 
	 * @brief Ici on prend l'epaisseur des murs dans les valeurs de 
	 * la largeur et de la hauteur
	 * 
	 * @brief Dans SFML, laxe des y pointe vers le bas
	 */
	Box (const Vec2d& centre, double largeur = 300.0, double hauteur = 300.0, double epaisseur = 10.0);
	
	~Box();
	
	Vec2d getCenter() const;
	double getWidth() const;
	double getHeight() const;
	double getWallWidth() const;
	
	Wall getWallTop() const;
	Wall getWallBottom() const;
	Wall getWallRight() const;
	Wall getWallLeft() const;
	
	void setWidth(double Width);
	void setHeight(double Height);
	void setWallWidth(double WallWidth);
	void setCenter(const Vec2d& Center);
	void setWalls();
	
	/*!
	 * @brief Permet de trouver la position en x de la face interne du
	 * mur de gauche
	 * 
	 * @brief Par défaut la valeur du paramètre sera false
	 * 
	 * @param intern un booléen
	 * @return la coordonnée en x de la face interne du mur de gauche si
	 * intern vaut true et de la face externe si intern vaut false
	 */
	double getLeftLimit(bool intern = false) const;
	
	/*!
	 * @brief Permet de trouver la position en x de la face interne du
	 * mur de droite
	 * 
	 * @brief Par défaut la valeur du paramètre sera false
	 * 
	 * @param intern un booléen
	 * @return la coordonnée en x de la face interne du mur de droite si
	 * intern vaut true et de la face externe si intern vaut false
	 */
	double getRightLimit(bool intern = false) const;
	
	/*!
	 * @brief Permet de trouver la position en y de la face interne du
	 * mur du haut
	 * 
	 * @brief Par défaut la valeur du paramètre sera false
	 * 
	 * @param intern un booléen
	 * @return la coordonnée en y de la face interne du mur du haut si
	 * intern vaut true et de la face externe si intern vaut false
	 */
	double getTopLimit(bool intern = false) const;
	
	/*!
	 * @brief Permet de trouver la position en y de la face interne du
	 * mur du bas
	 * 
	 * @brief Par défaut la valeur du paramètre sera false
	 * 
	 * @param intern un booléen
	 * @return la coordonnée en y de la face interne du mur du bas si
	 * intern vaut true et de la face externe si intern vaut false
	 */
	double getBottomLimit(bool intern = false) const;
	
	/*!
	 * @brief Permet de voir si un Vec2d est complètement à l'intérieur
	 * de la boîte (l'instance courante)
	 * 
	 * @brief Un point sur le bord interne d'un mur ne sera pas
	 * considéré comme à l'intérieur
	 * 
	 * @param position un Vec2d
	 * @return true si position est complètement à l'intérieur et false sinon
	 */
	bool isPositionInside(const Vec2d& position) const;
	
	/*!
	 * @brief Permet de voir si un Vec2d est sur un des murs de la boîte
	 * (l'instance courante)
	 * 
	 * @brief Un point sur le bord interne ou externe d'un mur sera
	 * considéré comme sur le mur
	 * 
	 * @param position un Vec2d
	 * @return true si position est sur l'un des murs et false sinon
	 */
	bool isPositionOnWall(const Vec2d& position) const;
	
	/*!
	 * @brief Permet de voir si un Collider (un Vec2d avec un rayon)
	 * est dans une boite
	 * 
	 * @param position un Vec2d et radius un double
	 * 
	 * @return true si le Collider est dedans et false sinon
	 */
	bool isColliderInside(const Vec2d& position, double radius) const;
	
	/*!
	 * @brief Permet de voir si un Collider est en contact avec un des
	 * 4 murs de la boite, et si oui, lequel
	 * 
	 * @param position un Vec2d et radius un double
	 * 
	 * @return le mur avec lequel le Collider est en contact
	 */
	Wall whichWall(const Vec2d& position, double radius) const;
	
	/*!
	 * @brief Permet de dessiner les 4 murs de la boite
	 * 
	 * @param target un RenderTarget (qui fait partie de la librairie SFML)
	 */
	void drawOn(sf::RenderTarget& target) const;
	
	/*!
	 * @brief Permet de dessiner un mur de la boite
	 * 
	 * @param point1 et point2 les 2 points qui caractérisent un mur,
	 * et target un RenderTarget (qui fait partie de la librairie SFML)
	 */
	void drawWall(Vec2d point1, Vec2d point2, sf::RenderTarget& target) const;
	
	/*!
	 * @brief Permet de "vider" la boite de l'animal, c'est-à-dire que
	 * l'attribut animal_present devient false
	 */
	void reset();
	
	/*!
	 * @brief Permet de voir si la boite est vide (si elle ne contient
	 * aucun animal), c'est-à-dire si l'attribut animal_present vaut false
	 */
	bool isEmpty() const;
	
	/*!
	 * @brief Permet d'ajouter un animal à la boite, c'est-à-dire que
	 * l'attribut animal_present vaut true
	 */
	void addOccupant();

private:
	Vec2d centre;
	double largeur;
	double hauteur;
	double epaisseur;
	Wall wall_top;
	Wall wall_bottom;
	Wall wall_left;
	Wall wall_right;
	bool animal_present;
};

#endif
