#include <stdio.h>
#include "Diffusion.hpp"

Diffusion :: Diffusion(Sample * sample, double D0, double Ed, double Q, double Geometry):
    _nbCells    (sample->returnNbCells()),

    _D0         (D0),
    _Ed         (Ed),
    _Q          (Q),
    _coeff_Fick (vector<double>(_nbCells)),
    _flux       (vector<double>(_nbCells)),
	 dC_dx		(vector<double>(_nbCells)),
	 dT_dx		(vector<double>(_nbCells)),
	_Geometry	(Geometry),

    _positions  (& (sample->returnPosition())),
    _temperature(& (sample->returnTemperature())),
    _Css        (& (sample->returnSolutionContent())),
    _Cprec      (& (sample->returnSolutionContent()))
{}

void Diffusion :: computeCoeff()
{
    // The diffusion coefficient follows an Arrhenius law
    for(int k=0; k<_nbCells; k++)
        _coeff_Fick[k] = _D0 * exp(-_Ed / (kb * (*_temperature)[k])) ;
}

void Diffusion :: computeGradient()
{
	// Find Temp and Solid Solution gradients at each node, depending on geometry type
	if (_Geometry > 0)
	{
		// Polar Geometry
	}else {for (int k=0; k<_nbCells-1; k++)
		{
		// Linear Geometry
			dC_dx[k] = ((*_Css)[k+1] - (*_Css)[k]) / ((*_positions)[k+1] - (*_positions)[k]) ;
			dT_dx[k] = ((*_temperature)[k+1] - (*_temperature)[k]) / ((*_positions)[k+1] - (*_positions)[k]) ;
		}
	}
}

void Diffusion :: computeFlux()
{
    // Fick's law and Soret effect diffusion components
    double flux_fick(0.), flux_soret(0.) ;
    
    for(int k=0; k<_nbCells-1; k++)
    {
        flux_fick = - _coeff_Fick[k] * dC_dx[k] ;
        
        flux_soret = - _coeff_Fick[k] * _Q * (*_Css)[k] * dT_dx[k] / (R * pow((*_temperature)[k], 2)) ;
        
        _flux[k] = flux_fick + flux_soret ;
    }
    
}

double  Diffusion :: timeStep()
{
    // The time step associated with diffusion is computed using the convergence criterion from finite element theory
    double dt = pow((*_positions)[1] - (*_positions)[0], 2) / (2 * _coeff_Fick[0]) ;
    for(int k=2; k<_nbCells; k++)
        dt = min(dt, pow((*_positions)[k] - (*_positions)[k-1], 2) / (2 * _coeff_Fick[k]));
    
    return dt ;
}

vector<double>&  Diffusion :: returnFlux() {return _flux;}
