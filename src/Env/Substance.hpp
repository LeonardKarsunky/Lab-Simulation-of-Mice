#ifndef INFOSV_SUBSTANCE_HPP_
#define INFOSV_SUBSTANCE_HPP_

#include <Types.hpp>
#include <limits>
#include <iostream>


class Substance
{
	public:
	/*!
     * @brief Default ctor, zero initialization
     */
	Substance();
	
	/*!
     * @brief ctor
	 * @param vgef          : quantity of VGEF
	 * @param glucose       : quantity of VGEF
	 * @param bromopyruvate : quantity of BROMOPYRUVATE
	 * clamped  to be positive or null and less than getAppConfig().substance_max_value
     */
	Substance(double vgef, double glucose, double bromopyruvate);
	
     /*!
     * @brief Copy ctor
     */
	Substance(const Substance&);

	/*!
     * @brief Destructor
     */
	~Substance();
	
     /*!
     * @brief assignment operator
     */
	Substance& operator=(const Substance&);

	/*!
     * @brief adds Substance to this quantitywise
	 * fractions are adjusted 
     */
	Substance& operator+=(const Substance&);

	/*!
     * @brief removes Substance from this quantitywise
	 * fractions are adjusted 
     */
	Substance& operator-=(const Substance&);


	/*!
     * @brief returns the quantity of the substance identified by SubstanceId
     */
	double operator[] (const SubstanceId) const;

	/*!
     * @brief returns the VGEF's fraction
     */
	double getFractVGEF() const {return m_fract_vgef;}

	/*!
     * @brief returns the GLUCOSE's fraction
     */
	double getFractGlucose() const {return m_fract_glucose;}

	/*!
     * @brief returns the BROMOPYRUVATE's fraction
     */
	double getFractInhibitor() const {return m_fract_bromopyruvate;}

	/*!
     * @brief returns the total concentration (sum of quantities)
     */
	double getTotalConcentration() const {return m_total_concentration;}

	/*!
     * @brief multiplies by coeff the substance identified by SubstanceId
	 * if coeff is too small, the related quantity is set to zero
     */
	void update(SubstanceId, double coeff);


     /*!
     * @brief returns true is each quantity is approximately equal to zero
     */
	
	bool isNull() const;
	
	/*!
     * @brief removes from this a fraction of the substance identified by id
	 * and gives it to receiver
     */
	void uptakeOnGradient(double fraction, Substance& receiver, SubstanceId id);

protected:
     /*!
     * @brief helper method : given the quantities, computes the fractions
	 * when the total concentration is too small, everything is set to zero
     */
	
	void computeFractions(double vgef, double glucose, double bromo);
	
	private:
	// fraction of VGEF
	double m_fract_vgef;
	// fraction of GLUCOSE
	double m_fract_glucose;
	// fraction of bormpyruvate
	double m_fract_bromopyruvate;

	// sum of quantities
	double m_total_concentration;
	
};

/*!
 * @brief helper function  : clamps a value between a min and a max
 */
double check(double, double minValue = 0.0);
double check(double, double minValue , double maxValue);

/*!
 * @brief operator* overload for the Substance class
 * multiplies each quantity by coeff
 */
Substance operator*(const Substance&,double coeff);

/*!
 * @brief operator== overload for the Substance class
 * @brief operator!= overload for the Substance class
 * comparison is done with approximation
 */
bool operator==(const Substance&, const Substance&);
bool operator!=(const Substance&,const Substance&);

/*!
 * @brief operator<< overload for the Substance class
 */
std::ostream& operator<<(std::ostream&,const Substance&);

#endif 
