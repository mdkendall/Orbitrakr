#ifndef _AstroLib_h_
#define _AstroLib_h_

/*  This file is an excerpt from AstroLib, containing just the functions that
 *  are needed for this application and omiting those that are unneeded and/or
 *  problematic for embedded use.
 *
 *  Fundamentals of Astrodynamics and Applications, David Vallado, ISBN: 978-11881883180
 *  https://www.celestrak.com/software/vallado-sw.php
 *  https://www.celestrak.com/software/vallado/cpp.zip
 *
 *  Comments below are from the original. Minor editing for formatting.
 */

/* --------------------------------------------------------------------
 *
 *                                AstroLib.h
 *
 *  This file contains miscallaneous two-body motion functions.
 *  Several areas are covered: coordinate transformations, 2body routines,
 *  jpl ephemeris conversions, covariance operations.
 *
 * ----------------------------------------------------------------------
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>
#include "MathTimeLib.h"

typedef enum
{
	e80,
	e96,
	e00a,
	e00b,
	e00cio
} eOpt;

namespace AstroLib
{

	// -----------------------------------------------------------------------------------------
	//                                       Coordinate system functions
	// -----------------------------------------------------------------------------------------

	double  gstime
	(
		double jdut1
	);

	void polarm
	(
		double xp, double yp, double ttt, eOpt opt, std::vector< std::vector<double> > &pm
	);

	void teme_ecef
	(
		double rteme[3], double vteme[3], double ateme[3],
		MathTimeLib::edirection direct,
		double recef[3], double vecef[3], double aecef[3],
		double ttt, double jdut1, double lod, double xp, double yp, int eqeterms
	);

	// -----------------------------------------------------------------------------------------
	//                                       2-body functions
	// -----------------------------------------------------------------------------------------

	void ecef2ll
	(
		double recef[3],
		double& latgc, double& latgd, double& lon, double& hellp
	);

	void site
	(
		double latgd, double lon, double alt,
		double rsecef[3], double vsecef[3]
	);

	/* ------------------------- conversion techniques -------------------------- */

	void rv_razel
	(
		double recef[3], double vecef[3], double rsecef[3], double latgd, double lon,
		MathTimeLib::edirection direct,
		double& rho, double& az, double& el, double& drho, double& daz, double& del
	);

	void rvsez_razel
	(
		double rhosez[3], double drhosez[3],
		MathTimeLib::edirection direct,
		double& rho, double& az, double& el, double& drho, double& daz, double& del
	);
}

#endif
