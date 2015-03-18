/****************************************************************************
 *
 * $Id: vpMomentAlpha.cpp 4713 2014-03-28 18:02:26Z mbakthav $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Alpha moment descriptor for in-plane orientation.
 *
 * Authors:
 * Filip Novotny
 *
 *****************************************************************************/

#include <visp/vpMomentAlpha.h>
#include <visp/vpMomentGravityCenter.h>
#include <visp/vpMomentCentered.h>	
#include <cmath>

/*!
  Empty constructor. Initializes alpha moment as a reference alpha. A default-constructed alpha moment may be used as a reference for other alphas.
  A reference alpha is class harbouring an alpha value computed for a \f$[-\pi/2..\pi/2]\f$ portion of the circle. Not setting a reference alpha will prevent you
  from doing more than 180deg rotation with moments.
 */
vpMomentAlpha::vpMomentAlpha() : isRef(true), symmetric(false), ref(), alphaRef(0.) {
    values.resize(1);
}

/*!
  Common constructor. Initializes alpha moment as a non-reference alpha. A default-constructed alpha moment must be used as a reference for other this alpha.
  A reference alpha is class harbouring an alpha value computed for a \f$[0..\pi]\f$ portion of the circle. Not having a reference alpha will prevent you
  from doing more than 180deg rotation with moments.
  \param ref_ : vector of 3rd order centered moments corresponding to the reference alpha in the following order: \f$\mu_{03},\mu_{12},\mu_{21},\mu_{30}\f$.
  \param alpha_ref : value of the reference alpha.
*/
vpMomentAlpha::vpMomentAlpha(std::vector<double>& ref_, double alpha_ref)
  : vpMoment(),isRef(false),symmetric(false),ref(ref_),alphaRef(alpha_ref)
{
  for (std::vector<double>::iterator it = ref_.begin(); it!=ref_.end(); it++)
    if (*it<=1e-4)
      symmetric = true;

  values.resize(1);
}

/*!
	Compute the value of the alpha-moment.
  Depends on vpMomentCentered.
 */
void vpMomentAlpha::compute(){
	//symmetric = symmetric | this->getObject().isSymmetric();
	bool found_moment_centered;

	const vpMomentCentered& momentCentered = (static_cast<const vpMomentCentered&> (getMoments().get("vpMomentCentered",
			found_moment_centered)));

	if (!found_moment_centered)
		throw vpException(vpException::notInitialized, "vpMomentCentered not found");

	double alpha = 0.5 * atan2(2.0 * momentCentered.get(1, 1), (momentCentered.get(2, 0) - momentCentered.get(0, 2)));

	unsigned int order = 4;
	std::vector<double> rotMu(4);
	std::vector<double> realMu(4);

	if (isRef)
	{
		alphaRef = alpha;
	}
	else
	{
		if (!symmetric)
		{
			double r11 = cos(alpha - alphaRef);
			double r12 = sin(alpha - alphaRef);
			double r21 = -sin(alpha - alphaRef);
			double r22 = cos(alpha - alphaRef);
			unsigned int idx = 0;
			for (register unsigned int c = 0; c < (order) * (order); c++)
			{
				unsigned int i = c % order;
				unsigned int j = c / order;

				if (i + j == 3)
				{
					double r11_k = 1.;
					for (register unsigned int k = 0; k <= i; k++)
					{
						double r12_i_k = pow(r12, (int)(i - k));
						double comb_i_k = static_cast<double> (vpMath::comb(i, k));
						for (register unsigned int l = 0; l <= j; l++)
						{
							rotMu[idx] += static_cast<double> (comb_i_k * vpMath::comb(j, l) * r11_k * pow(r21, (int)l) * r12_i_k
									* pow(r22, (int)(j - l)) * momentCentered.get(k + l, (unsigned int)(int)(i + j - k - l)));
						}
						r11_k *= r11;
					}
					realMu[idx] = momentCentered.get(i, j);
					idx++;
				}
			}

			double sum = 0.;
			bool signChange = true;
			for (register unsigned int i = 0; i < 4; i++)
			{
				if (std::abs(rotMu[i]) > 1e10 * std::numeric_limits<double>::epsilon() && std::abs(ref[i]) > 1e10
						* std::numeric_limits<double>::epsilon() && rotMu[i] * ref[i] > 0)
					signChange = false;
				sum += std::abs(rotMu[i] * ref[i]);
			}

			if (sum < 1e4 * std::numeric_limits<double>::epsilon())
				signChange = false;
			if (signChange)
				alpha = alpha + M_PI;
		}
	}
	values[0] = alpha;
}

/*!
  Prints the value of the major-axis orientation in degrees and rad
 */
VISP_EXPORT std::ostream & operator<<(std::ostream & os, const vpMomentAlpha& c){
    os << (__FILE__) << std::endl;
    os << "Alpha = " << c.values[0] << "rad = " << vpMath::deg(c.values[0]) << "deg " << std::endl;
	return os;
}

/*!
Prints the dependencies of alpha, namely centered moments mu11, mu20 ad mu02
*/
void  vpMomentAlpha::printDependencies(std::ostream& os) const{
    os << (__FILE__) << std::endl;
    bool found_moment_centered;
    const vpMomentCentered& momentCentered = (static_cast<const vpMomentCentered&> (getMoments().get("vpMomentCentered",
            found_moment_centered)));
    if (!found_moment_centered)
        throw vpException(vpException::notInitialized, "vpMomentCentered not found");

    os << "mu11 = " << momentCentered.get(1, 1) << "\t";
    os << "mu20 = " << momentCentered.get(2, 0) << "\t";
    os << "mu02 = " << momentCentered.get(0, 2) << std::endl;
}
