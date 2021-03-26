#pragma once
#include "BaseComponent.h"
#include "Engine2.h"
#include "Maths.h"
#include "Data.h"
#include <assert.h>

//kg/s
#define c_fuelTransferRateWingToFuse 1.8 
#define c_fuelTransferRateExternWingToWing 0.60
#define c_fuelTransferRateExternCentreToWing 0.69
#define c_fuelTransferRateWingToFuseEmergency 1.1

namespace Scooter
{

class FuelSystem2 : public BaseComponent
{
public:

	FuelSystem2( Scooter::Engine2& engine );
	virtual void zeroInit();
	virtual void coldInit();
	virtual void hotInit();
	virtual void airborneInit();

	enum Tank
	{
		TANK_FUSELAGE,
		TANK_WING,
		TANK_EXTERNAL_LEFT,
		TANK_EXTERNAL_CENTRE,
		TANK_EXTERNAL_RIGHT,
		NUMBER_OF_TANKS
	};

	void addFuel( double dm, bool wingFirst = false );
	void drawFuel( double dm );
	void update( double dt );
	bool handleInput( int command, float value );

	inline bool hasFuel() const;
	inline void updateMechPumpPressure();
	inline double transferFuel( Tank from, Tank to, double dm );
	inline double addFuelToTank( Tank tank, double dm, double min = 0.0 );
	inline double transferRateFactor();

	inline void setExternalTankPressure( bool pressure );
	inline void setMechPumpPressure( bool pressure );
	inline void setWingTankPressure( bool pressure );
	inline void setWingTankBypass( bool bypass );

	inline double getFuelQty( Tank tank ) const;
	inline double getFuelQtyExternal() const;
	inline double getFuelQtyInternal() const;
	inline double getFuelQtyDelta( Tank tank ) const;
	inline const Vec3& getFuelPos( Tank tank ) const;
	inline Tank getSelectedTank() const;
	inline void setFuelQty( Tank tank, const Vec3& position, double value );
	inline void setInternal( double value );
	inline void setFuelCapacity( double l, double c, double r );
	inline void setFuelPrevious( Tank tank );
	inline void setSelectedTank( Tank tank );
	inline void setFuelDumping(bool dumping);

private:
	double m_fuel[NUMBER_OF_TANKS] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	double m_fuelPrevious[NUMBER_OF_TANKS] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	//										fuselage wing     l    c   r
	double m_fuelCapacity[NUMBER_OF_TANKS] = { 731.0, 1737.0, 0.0, 0.0, 0.0 }; //changed it to 1737 to match the values from the entry.
	Vec3 m_fuelPos[NUMBER_OF_TANKS] = { Vec3(), Vec3(), Vec3(), Vec3(), Vec3() };

	bool m_externalTankPressure = true;
	bool m_mechPumpPressure = true; //is the mechanical pump working
	bool m_wingTankPressure = false; //emergency pressure
	bool m_wingTankBypass = false; //bypass the wing tanks, fueling only the fuselage and external tanks

	bool m_hasFuel = true; //this is false is the fuel cannot be delivered or we are out of fuel.
	bool m_dumpingFuel = false;

	Tank m_selectedTank = TANK_FUSELAGE;

	Scooter::Engine2& m_engine;
};

void FuelSystem2::setExternalTankPressure( bool pressure )
{
	m_externalTankPressure = pressure;
}

void FuelSystem2::setMechPumpPressure( bool pressure )
{
	m_mechPumpPressure = pressure;
}

void FuelSystem2::setWingTankPressure( bool pressure )
{
	m_wingTankPressure = pressure;
}

void FuelSystem2::setWingTankBypass( bool bypass )
{
	m_wingTankBypass = bypass;
}

void FuelSystem2::setFuelQty( Tank tank, const Vec3& position, double value )
{
	m_fuel[tank] = value;
	m_fuelPos[tank] = position;
}

void FuelSystem2::setFuelPrevious( Tank tank )
{
	m_fuelPrevious[tank] = m_fuel[tank];
}

void FuelSystem2::setInternal( double value )
{
	if ( value <= m_fuelCapacity[TANK_FUSELAGE] )
	{
		m_fuel[TANK_FUSELAGE] = value;
		m_fuel[TANK_WING] = 0.0;
	}
	else
	{
		m_fuel[TANK_FUSELAGE] = m_fuelCapacity[TANK_FUSELAGE];
		value -= m_fuel[TANK_FUSELAGE];

		//assert( value <= m_fuelCapacity[TANK_WING] );

		m_fuel[TANK_WING] = std::min( value, m_fuelCapacity[TANK_WING] );
	}

	m_fuelPos[TANK_WING] = Vec3();
	m_fuelPos[TANK_FUSELAGE] = Vec3();
}

void FuelSystem2::setFuelCapacity( double l, double c, double r )
{
	m_fuelCapacity[TANK_EXTERNAL_LEFT] = l;
	m_fuelCapacity[TANK_EXTERNAL_CENTRE] = c;
	m_fuelCapacity[TANK_EXTERNAL_RIGHT] = r;
}

void FuelSystem2::setSelectedTank( Tank tank )
{
	m_selectedTank = tank;
}

void FuelSystem2::setFuelDumping( bool dumping )
{
	m_dumpingFuel = dumping;
}

double FuelSystem2::getFuelQtyDelta( Tank tank ) const
{
	return m_fuel[tank] - m_fuelPrevious[tank];
}

double FuelSystem2::getFuelQty( Tank tank ) const
{
	return m_fuel[tank];
}

double FuelSystem2::getFuelQtyExternal() const
{
	return m_fuel[TANK_EXTERNAL_CENTRE] + m_fuel[TANK_EXTERNAL_LEFT] + m_fuel[TANK_EXTERNAL_RIGHT];
}

double FuelSystem2::getFuelQtyInternal() const
{
	return m_fuel[TANK_FUSELAGE] + m_fuel[TANK_WING];
}

const Vec3& FuelSystem2::getFuelPos( Tank tank ) const
{
	return m_fuelPos[tank];
}

FuelSystem2::Tank FuelSystem2::getSelectedTank() const
{
	return m_selectedTank;
}

bool FuelSystem2::hasFuel() const
{
	return m_hasFuel;
}

void FuelSystem2::updateMechPumpPressure()
{
	//Rough approximation for the pump pressure driven from the engine, in psi because who cares.
	m_mechPumpPressure = 4.0 * m_engine.getRPMNorm() * m_engine.getRPMNorm();
}

double FuelSystem2::transferRateFactor()
{
	return clamp( 2.0 * m_engine.getRPMNorm() * m_engine.getRPMNorm(), 0.0, 1.0 );
}

double FuelSystem2::transferFuel( Tank from, Tank to, double dm )
{
	double desiredTransfer = dm;

	//15 kg minimum usable should be tank specific but it's not that different.
	double remainingFrom = m_fuel[from] - 15.0;
	//Check there is enough fuel to take.
	if ( remainingFrom < dm )
		dm = std::max( remainingFrom, 0.0 );

	//Check there is enough room in the to tank.
	double spaceInTo = m_fuelCapacity[to] - m_fuel[to];
	if ( spaceInTo < dm )
		dm = std::max( spaceInTo, 0.0 );

	//Actually transfer the fuel
	m_fuel[from] -= dm;
	m_fuel[to] += dm;

	return desiredTransfer - dm;
}

double FuelSystem2::addFuelToTank( Tank tank, double dm, double min )
{
	double desiredTransfer = dm;

	double remainingFuel = m_fuel[tank] - min;
	double remainingSpace = m_fuelCapacity[tank] - m_fuel[tank];

	if ( dm < 0.0 && remainingFuel + dm < 0.0 )
		dm = std::min( -remainingFuel, 0.0 );

	if ( dm > 0.0 && remainingSpace < dm )
		dm = std::max( remainingSpace, 0.0 );

	m_fuel[tank] += dm;

	return desiredTransfer - dm;
}

} //end scooter namespace