#include <algorithm>
#include <stdexcept>
#include <Env/Substance.hpp>
#include <Utility/Utility.hpp>
#include <Utility/Constants.hpp>
#include <iostream>
#include <Application.hpp>

void Substance::computeFractions(double vgef, double glucose, double bromo) 
{
	m_total_concentration = vgef + glucose + bromo;
	
	if 	(!isEqual(m_total_concentration, 0, SUBSTANCE_PRECISION))
	{
		m_fract_vgef = vgef/ m_total_concentration;
		m_fract_glucose = glucose / m_total_concentration;
		m_fract_bromopyruvate  = bromo / m_total_concentration;
	}
	else
	{
		m_fract_vgef = 0;
		m_fract_glucose = 0;
		m_fract_bromopyruvate = 0;
		m_total_concentration = 0;
	}
}

Substance::Substance() :
	m_fract_vgef(0.0),
	m_fract_glucose(0.0),
	m_fract_bromopyruvate(0.0),
	m_total_concentration(0)
{
}

Substance::Substance(double vgef, double glucose , double bromo) 
{
	computeFractions(check(vgef), check(glucose), check(bromo));
}

Substance::Substance(const Substance& other) :
	m_fract_vgef(other.getFractVGEF()),
	m_fract_glucose(other.getFractGlucose()),
	m_fract_bromopyruvate(other.getFractInhibitor()),
	m_total_concentration(other.getTotalConcentration())
{
}


Substance& Substance::operator= (const Substance& other)
{
	m_fract_vgef = other.getFractVGEF();
	m_fract_glucose = other.getFractGlucose();
	m_fract_bromopyruvate = other.getFractInhibitor();
	m_total_concentration = other.getTotalConcentration();
	return (*this);
}

Substance::~Substance()
{
}

double Substance::operator [] (const SubstanceId n) const
{
	switch(n)
	{
		case VGEF : return m_total_concentration*m_fract_vgef;
		case GLUCOSE : return m_total_concentration*m_fract_glucose;
		case BROMOPYRUVATE : return m_total_concentration*m_fract_bromopyruvate;
		default : throw std::invalid_argument("Bad substance index");
	}
	return 0.; /* should not happen */
}



Substance operator*(const Substance& n, double c)
{
	return Substance( n[VGEF]*c,  n[GLUCOSE]*c,  n[BROMOPYRUVATE]*c );
}
	
Substance& Substance::operator +=(const Substance& rhs)
{
	computeFractions(check((*this)[VGEF] +  rhs[VGEF]),
					 check((*this)[GLUCOSE] + rhs[GLUCOSE])   ,
					 check((*this)[BROMOPYRUVATE] + rhs[BROMOPYRUVATE]));
	return (*this);
}

Substance& Substance::operator -= (const Substance& rhs)
{
	computeFractions(check((*this)[VGEF] - rhs[VGEF]),
					 check((*this)[GLUCOSE] - rhs[GLUCOSE])   ,
					 check((*this)[BROMOPYRUVATE] - rhs[BROMOPYRUVATE]));
	return (*this);
}

void Substance::update(SubstanceId f, double c)
{
	double temp_glucose = (*this)[GLUCOSE];
	double temp_bromopyruvate = (*this)[BROMOPYRUVATE];
	double temp_vgef = (*this)[VGEF];
	switch(f)
	{
		case GLUCOSE :
			temp_glucose *= isEqual(c, 0., EPSILON) ? 0. : c;
			break;
		case BROMOPYRUVATE :
			temp_bromopyruvate *= isEqual(c, 0., EPSILON) ? 0. : c;
			break;
		case VGEF :
			temp_vgef *= isEqual(c, 0., EPSILON) ? 0. : c;
			break;
	}
	computeFractions(check(temp_vgef), check(temp_glucose), check(temp_bromopyruvate));
}
	
	

std::ostream& operator<<(std::ostream& out,const Substance& s)
{
	out << "[VGEF] = " << s[VGEF] << "\n"
		<< "[GLUCOSE] = " << s[GLUCOSE] << "\n"
		<< "[BROMOPYRUVATE] = " << s[BROMOPYRUVATE] << "\n";
	return out;
}

bool operator==(const Substance& lhs,const Substance& rhs)
{
	return  isEqual(lhs[VGEF], rhs[VGEF], SUBSTANCE_PRECISION)
		&&
		isEqual(lhs[BROMOPYRUVATE], rhs[BROMOPYRUVATE], SUBSTANCE_PRECISION)
		&&
		isEqual (lhs[GLUCOSE], rhs[GLUCOSE], SUBSTANCE_PRECISION);
}

bool operator!=(const Substance& lhs,const Substance& rhs)
{
	return !(rhs==lhs);
}

double check(double value, double minValue, double maxValue)
{
	value = std::max(minValue, value);
	value = std::min(value, maxValue);
	
	if (value < SUBSTANCE_PRECISION) return 0.;
	return value;
}

double check(double value, double minValue)
{
	return check(value, minValue, getAppConfig().substance_max_value);
}

void Substance::uptakeOnGradient(double fraction, Substance& receiver, SubstanceId id)
{
	if ((*this)[id] < SUBSTANCE_PRECISION) { 
		return;
	}

	Substance temp;
	
	switch(id)
	{
		case GLUCOSE : temp = Substance(0, fraction*(*this)[id], 0);break;		
		case BROMOPYRUVATE : temp = Substance(0,0, fraction*(*this)[id]);break;		
		case VGEF : temp = Substance(fraction*(*this)[id], 0, 0);break;	
	}
	receiver+=temp;
	(*this) -= temp;
	

}

bool Substance::isNull() const
		{
			return ( ((*this)[VGEF] < SUBSTANCE_PRECISION)
					 &&
					 ((*this)[GLUCOSE] < SUBSTANCE_PRECISION)
					 &&
					 ((*this)[BROMOPYRUVATE] < SUBSTANCE_PRECISION));
		}
