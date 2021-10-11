
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

/* ----------------------------------------------------------------
 *
 *                                AstroLib.cpp
 *
 * this library contains various astrodynamic routines.
 *
 *                            companion code for
 *               fundamentals of astrodynamics and applications
 *                                    2013
 *                              by david vallado
 *
 *       (w) 719-573-2600, email dvallado@agi.com, davallado@gmail.com
 *
 *    current :
 *              15 jan 19  david vallado
 *                           combine with astiod etc
 *    changes :
 *              11 jan 18  david vallado
 *                           misc cleanup
 *              30 sep 15  david vallado
 *                           fix jd, jdfrac
 *               3 nov 14  david vallado
 *                           update to msvs2013 c++
 *               4 may 09  david vallado
 *                           misc updates
 *              23 feb 07  david vallado
 *                           3rd edition baseline
 *              21 jul 05  david vallado
 *                           2nd printing baseline
 *              14 may 01  david vallado
 *                           2nd edition baseline
 *              23 nov 87  david vallado
 *                           original baseline
 * ---------------------------------------------------------------- */

#include "AstroLib.h"

namespace AstroLib
{

	// -----------------------------------------------------------------------------------------
	//                                       coordinate functions
	// -----------------------------------------------------------------------------------------

	/* -----------------------------------------------------------------------------
	*
	*                           function gstime
	*
	*  this function finds the greenwich sidereal time (iau-82).
	*
	*  author        : david vallado                  719-573-2600    1 mar 2001
	*
	*  inputs          description                    range / units
	*    jdut1       - julian date in ut1             days from 4713 bc
	*
	*  outputs       :
	*    gstime      - greenwich sidereal time        0 to 2pi rad
	*
	*  locals        :
	*    temp        - temporary variable for doubles   rad
	*    tut1        - julian centuries from the
	*                  jan 1, 2000 12 h epoch (ut1)
	*
	*  coupling      :
	*    none
	*
	*  references    :
	*    vallado       2013, 187, eq 3-45
	* --------------------------------------------------------------------------- */

	double  gstime
	(
		double jdut1
	)
	{
		const double deg2rad = pi / 180.0;
		double       temp, tut1;

		tut1 = (jdut1 - 2451545.0) / 36525.0;
		temp = -6.2e-6* tut1 * tut1 * tut1 + 0.093104 * tut1 * tut1 +
			(876600.0 * 3600 + 8640184.812866) * tut1 + 67310.54841;  // sec
		temp = fmod(temp * deg2rad / 240.0, twopi); //360/86400 = 1/240, to deg, to rad

		// ------------------------ check quadrants ---------------------
		if (temp < 0.0)
			temp = temp + twopi;

		return temp;
	}

    /* -----------------------------------------------------------------------------
    *
    *                           function polarm
    *
    *  this function calulates the transformation matrix that accounts for polar
    *    motion within the fk5 and iau2010 systems.
    *
    *  author        : david vallado                  719-573-2600   25 jun 2002
    *
    *  revisions
    *    vallado     - conversion to c++                             23 nov 2005
    *    vallado     - conversion to c#                              16 Nov 2011
    *
    *  inputs          description                                 range / units
    *    xp          - polar motion coefficient                         rad
    *    yp          - polar motion coefficient                         rad
    *    ttt         - julian centuries of tt (00 theory only)
    *    opt         - method option                           e80, e96, e00a, e00cio
    *
    *  outputs       :
    *    pm          - transformation matrix for itrf - pef
    *
    *  locals        :
    *    convrt      - conversion from arcsec to rad
    *    sp          - s prime value (00 theory only)
    *
    *  coupling      :
    *    none.
    *
    *  references    :
    *    vallado       2013, 212, 223
    * --------------------------------------------------------------------------- */

	void polarm
	(
		double xp, double yp, double ttt, eOpt opt, std::vector< std::vector<double> > &pm
	)
	{
		pm.resize(3);  // rows
		for (std::vector< std::vector<double> >::iterator it = pm.begin(); it != pm.end(); ++it)
			it->resize(3);
		double cosxp, cosyp, sinxp, sinyp, sp, cossp, sinsp;

		cosxp = cos(xp);
		sinxp = sin(xp);
		cosyp = cos(yp);
		sinyp = sin(yp);

		if ((opt == e80) | (opt == e96))
		{
			pm[0][0] = cosxp;
			pm[0][1] = 0.0;
			pm[0][2] = -sinxp;
			pm[1][0] = sinxp * sinyp;
			pm[1][1] = cosyp;
			pm[1][2] = cosxp * sinyp;
			pm[2][0] = sinxp * cosyp;
			pm[2][1] = -sinyp;
			pm[2][2] = cosxp * cosyp;

			// alternate approach
			//MathTimeLib::rot2mat(xp, a1);
			//MathTimeLib::rot1mat(yp, a2);
			//MathTimeLib::matmult(a2, a1, pm, 3, 3, 3);
		}
		else
		{
			// approximate sp value in rad
			sp = -47.0e-6 * ttt * pi / (180.0 * 3600.0);
			cossp = cos(sp);
			sinsp = sin(sp);

			// form the matrix
			pm[0][0] = cosxp * cossp;
			pm[0][1] = -cosyp * sinsp + sinyp * sinxp * cossp;
			pm[0][2] = -sinyp * sinsp - cosyp * sinxp * cossp;
			pm[1][0] = cosxp * sinsp;
			pm[1][1] = cosyp * cossp + sinyp * sinxp * sinsp;
			pm[1][2] = sinyp * cossp - cosyp * sinxp * sinsp;
			pm[2][0] = sinxp;
			pm[2][1] = -sinyp * cosxp;
			pm[2][2] = cosyp * cosxp;

			// alternate approach
			//MathTimeLib::rot1mat(yp, a1);
			//MathTimeLib::rot2mat(xp, a2);
			//MathTimeLib::rot3mat(-sp, a3);
			//MathTimeLib::matmult(a2, a1, tr1, 3, 3, 3);
			//MathTimeLib::matmult(a3, tr1, pm, 3, 3, 3);
		}
	}  //  polarm

    /* ----------------------------------------------------------------------------
    *
    *                           function teme_ecef
    *
    *  this function transforms a vector between the earth fixed(ITRF) frame and the
    *  true equator mean equniox frame(teme).the results take into account
    *    the effects of sidereal time, and polar motion.
    *
    *  author        : david vallado                  719 - 573 - 2600   30 oct 2017
    *
    *  revisions
    *
    *  inputs          description                    range / units
    *    rteme        - position vector teme            km
    *    vteme        - velocity vector teme            km / s
    *    ateme        - acceleration vector teme        km / s2
    *    direct       - direction of transfer           eFrom, 'TOO '
    *    ttt          - julian centuries of tt          centuries
    *    jdut1        - julian date of ut1              days from 4713 bc
    *    lod          - excess length of day            sec
    *    xp           - polar motion coefficient        rad
    *    yp           - polar motion coefficient        rad
    *    eqeterms     - use extra two terms(kinematic) after 1997  0, 2
    *
    *  outputs       :
    *    recef        - position vector earth fixed     km
    *    vecef        - velocity vector earth fixed     km / s
    *    aecef        - acceleration vector earth fixed km / s2
    *
    *  locals :
    *    st           - matrix for pef - tod
    *    pm           - matrix for ecef - pef
    *
    *  coupling :
    *   gstime        - greenwich mean sidereal time    rad
    *   polarm        - rotation for polar motion       pef - ecef
    *
    *  references :
    *    vallado       2013, 231 - 233
    * ----------------------------------------------------------------------------*/

	void teme_ecef
	(
		double rteme[3], double vteme[3], double ateme[3],
		MathTimeLib::edirection direct,
		double recef[3], double vecef[3], double aecef[3],
		double ttt, double jdut1, double lod, double xp, double yp, int eqeterms
	)
	{
		double deg2rad, omega, gmstg, thetasa;
		//trans.resize(3);  // rows
		//for (std::vector< std::vector<double> >::iterator it = trans.begin(); it != trans.end(); ++it)
		//	it->resize(3);
		std::vector< std::vector<double> > st = std::vector< std::vector<double> >(3, std::vector<double>(3));
		std::vector< std::vector<double> > stdot = std::vector< std::vector<double> >(3, std::vector<double>(3));
		std::vector< std::vector<double> > temp = std::vector< std::vector<double> >(3, std::vector<double>(3));
		std::vector< std::vector<double> > tempmat = std::vector< std::vector<double> >(3, std::vector<double>(3));
		std::vector< std::vector<double> > pm, pmp, stp = std::vector< std::vector<double> >(3, std::vector<double>(3));
		double omegaearth[3], rpef[3], vpef[3], apef[3], omgxr[3], omgxomgxr[3],
			omgxv[3], tempvec1[3], tempvec[3], gmst;

		deg2rad = pi / 180.0;

		// find omeage from nutation theory
		omega = 125.04452222 + (-6962890.5390 *ttt + 7.455 *ttt*ttt + 0.008 *ttt*ttt*ttt) / 3600.0;
		omega = fmod(omega, 360.0) * deg2rad;

		// ------------------------find gmst--------------------------
		gmst = AstroLib::gstime(jdut1);

		// teme does not include the geometric terms here
		// after 1997, kinematic terms apply
		if ((jdut1 > 2450449.5) && (eqeterms > 0))
		{
			gmstg = gmst
				+ 0.00264*pi / (3600.0 * 180.0)*sin(omega)
				+ 0.000063*pi / (3600.0 * 180.0)*sin(2.0 *omega);
		}
		else
			gmstg = gmst;
		gmstg = fmod(gmstg, 2.0*pi);

		thetasa = 7.29211514670698e-05 * (1.0 - lod / 86400.0);
		omegaearth[0] = 0.0;
		omegaearth[1] = 0.0;
		omegaearth[2] = thetasa;

		st[0][0] = cos(gmstg);
		st[0][1] = -sin(gmstg);
		st[0][2] = 0.0;
		st[1][0] = sin(gmstg);
		st[1][1] = cos(gmstg);
		st[1][2] = 0.0;
		st[2][0] = 0.0;
		st[2][1] = 0.0;
		st[2][2] = 1.0;

		// compute sidereal time rate matrix
		stdot[0][0] = -omegaearth[2] * sin(gmstg);
		stdot[0][1] = -omegaearth[2] * cos(gmstg);
		stdot[0][2] = 0.0;
		stdot[1][0] = omegaearth[2] * cos(gmstg);
		stdot[1][1] = -omegaearth[2] * sin(gmstg);
		stdot[1][2] = 0.0;
		stdot[2][0] = 0.0;
		stdot[2][1] = 0.0;
		stdot[2][2] = 0.0;

		polarm(xp, yp, ttt, e80, pm);

		if (direct == MathTimeLib::eTo)
		{
			MathTimeLib::mattrans(pm, pmp, 3, 3);
			MathTimeLib::mattrans(st, stp, 3, 3);

			MathTimeLib::matvecmult(stp, rteme, rpef);
			MathTimeLib::matvecmult(pmp, rpef, recef);

			MathTimeLib::cross(omegaearth, rpef, omgxr);
			MathTimeLib::matvecmult(stp, vteme, tempvec1);
			MathTimeLib::addvec(1.0, tempvec1, -1.0, omgxr, vpef);
			MathTimeLib::matvecmult(pmp, vpef, vecef);

			MathTimeLib::addvec(1.0, tempvec1, -1.0, omgxr, vpef);
			MathTimeLib::cross(omegaearth, vpef, omgxv);
			MathTimeLib::cross(omegaearth, omgxr, omgxomgxr);
			MathTimeLib::matvecmult(stp, ateme, tempvec1);
			MathTimeLib::addvec(1.0, tempvec1, -1.0, omgxomgxr, tempvec);
			MathTimeLib::addvec(1.0, tempvec, -2.0, omgxv, apef);
			MathTimeLib::matvecmult(pmp, apef, aecef);
		}
		else
		{
			MathTimeLib::matvecmult(pm, recef, rpef);
			MathTimeLib::matvecmult(st, rpef, rteme);

			MathTimeLib::matvecmult(pm, vecef, vpef);
			MathTimeLib::cross(omegaearth, rpef, omgxr);
			MathTimeLib::addvec(1.0, vpef, 1.0, omgxr, tempvec1);
			MathTimeLib::matvecmult(st, tempvec1, vteme);

			MathTimeLib::matvecmult(pm, aecef, apef);
			MathTimeLib::cross(omegaearth, omgxr, omgxomgxr);
			MathTimeLib::cross(omegaearth, vpef, omgxv);
			MathTimeLib::addvec(1.0, apef, 1.0, omgxomgxr, tempvec);
			MathTimeLib::addvec(1.0, tempvec, 2.0, omgxv, tempvec1);
			MathTimeLib::matvecmult(st, tempvec1, ateme);
		}
	}
}
