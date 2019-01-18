#ifndef COLLIDER_H
#define COLLIDER_H

#include <Utility/Vec2d.hpp>
#include <SFML/Graphics.hpp>

/*!
 * @class Collider
 * 
 * @brief Classe d'un objet qui modélise la représentation géométrique
 * simplifiée que vont prendre les objets du monde pour réaliser les
 * tests de collision. Cette forme est simplement un cercle englobant
 */
class Collider {
	
public:
	virtual Vec2d getCenter() const = 0;
	virtual double getRadius() const = 0;
	
	/*!
	 * @brief Donne le rayon initial d'une entité simulée
	 */
	virtual double getInitialRadius() const = 0;
	
	virtual void setCenterX(double x) = 0;
	virtual void setCenterY(double y) = 0;
	virtual void setCenter(const Vec2d& center) = 0;
	
	/*!
	 * @brief Permet de voir si deux Collider sont en collision ou non
	 * 
	 * @brief Soient deux Collider a et b, a est en collision avec b si
	 * la distance entre les centres de a et b est inférieure ou égale à
	 * la somme des rayons de a et b
	 * 
	 * @param other un autre Collider
	 * @return true si les deux Collider sont en collision et false sinon
	 */
	bool isColliding (const Collider& other) const;
	
	/*!
	 * @brief Permet de voir si deux Collider sont en collision ou non,
	 * mais cette fois on prend en paramètres directement le centre de
	 * l'autre Collider et son rayon
	 * 
	 * @param centre un Vec2d et radius un double
	 * 
	 * @return true si les deux Collider sont en collision et false sinon
	 */
	bool isColliding(const Vec2d& centre, double radius) const;
	
	/*!
	 * @brief Permet de voir si un point est à l'intérieur de l'instance
	 * courante
	 * 
	 * @brief Un point p est à l'intérieur d'un Collider body si la
	 * distance entre p et body est inférieure ou égale au rayon de body
	 * 
	 * @param point un Vec2d
	 * @return true si le point est à l'intérieur et false sinon
	 */
	bool isPointInside (const Vec2d& point) const;
	
	/*!
	 * @brief Permet de dessiner un Collider, c'est-à-dire un disque de
	 * rayon getRadius()
	 * 
	 * @param target un RenderTarget (qui fait partie de la librairie SFML)
	 */
	void drawOn(sf::RenderTarget& target) const;
};

/*!
 * @brief Opérateur externe qui vérifie si deux Collider sont en collision
 * 
 * @param body1 et body2 des Collider
 * @return true s'ils sont en collision et false sinon
 */
bool operator|(const Collider& body1, const Collider& body2);

/*! 
 * @brief Opérateur externe qui vérifie si un Vec2d est à l'intérieur
 * d'un Collider
 * 
 * @param body un Collider et point un Vec2d
 * @return true si point est à l'intérieur et false sinon
 */
bool operator>(const Collider& body, const Vec2d& point);

#endif
