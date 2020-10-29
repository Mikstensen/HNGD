/**
 This class implements Fick's law and Soret effect for hydrogen in zirconium for the HNGD model
 */

#ifndef Diffusion_hpp
#define Diffusion_hpp

#include <stdio.h>
#include <vector>
#include "Sample.hpp"
#include "PhysicsConstants.h"

class Diffusion
{
public:
    Diffusion(Sample * sample, double D0, double Ed, double Q, double geometry, double radius) ;
    
    // Compute the diffusion coefficient at each position
    void computeCoeff() ;
    
    // Compute Gradient at each node (varies between linear and polar)
    void computeGradient() ;

    // Compute the hydrogen flux at each position
    void computeFlux() ;
    
    // Compute the time step associated with diffusion
    double timeStep() ;
    
    vector<double>& returnFlux() ;
    
    vector<double>& returnCGrad    () {return dC_dx;}
    vector<double>& returnTGrad    () {return dT_dx;}

private:
    const int _nbCells ;    	// Number of nodes
    const double _D0 ;     		// Preexponential factor for diffusion coefficient
    const double _Ed ;      	// Activation energy for diffusion coefficient
    const double _Q ;       	// Heat of transport
    const double _geometry ;	// Polar or Linear
    const double _radius ;		// Radius of sample
    
    vector<double> _coeff_Fick ; // Coefficient of diffusion
    vector<double> _flux ;       // Hydrogen flux
    
    const vector<double> * _positions ;  // Position of the nodes
    const vector<double> * _temperature ;// Temperature profile
    const vector<double> * _Css ;        // Solid solution profile
    const vector<double> * _Cprec ;      // Hydride profile

	vector<double> dC_dx ;
	vector<double> dT_dx ;
};

#endif /* Diffusion_hpp */
