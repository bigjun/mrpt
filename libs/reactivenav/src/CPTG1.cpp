/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2013, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2013, MAPIR group, University of Malaga                |
   | Copyright (c) 2012-2013, University of Almeria                            |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */

#include <mrpt/reactivenav.h>  // Precomp header

using namespace mrpt;
using namespace mrpt::reactivenav;
using namespace mrpt::system;

#define CTE_DIV_ALFA 2

/*---------------------------------------------------------------
						Constructor
  ---------------------------------------------------------------*/
CPTG1::CPTG1(const TParameters<double> &params ) :
	CParameterizedTrajectoryGenerator (params)
{
	this->K = params["K"];
}

/*---------------------------------------------------------------
						getDescription
  ---------------------------------------------------------------*/
std::string CPTG1::getDescription() const
{
	return mrpt::format("Type#1PTG,circ.arcs,K=%i",(int)K);
}


/*---------------------------------------------------------------

  ---------------------------------------------------------------*/
void CPTG1::PTG_Generator( float alpha, float t,float x, float y, float phi, float &v, float &w )
{
    // (v,w)
    v = V_MAX * sign(K);
    w = tan( alpha/CTE_DIV_ALFA ) * W_MAX * sign(K);
}

/*---------------------------------------------------------------
					PTG_IsIntoDomain
  ---------------------------------------------------------------*/
bool CPTG1::PTG_IsIntoDomain( float x, float y )
{
	return true;
}

/*---------------------------------------------------------------
                lambdaFunction
  ---------------------------------------------------------------*/
void CPTG1::lambdaFunction( float x, float y, int &k_out, float &d_out )
{
	double		R,a;

	if (y!=0)
	{
		R = (x*x+y*y)/(2*y);
		a = sign(K)*2*atan( V_MAX / (W_MAX*R) );
		k_out = alpha2index( (float)a );

		double theta;

		if (K>0)
		{
			if (y>0)
					theta = atan2( (double)x,fabs(R)-y );
			else	theta = atan2( (double)x,y+fabs(R) );
		}
		else
		{
			if (y>0)
					theta = atan2( -(double)x,fabs(R)-y );
			else	theta = atan2( -(double)x,y+fabs(R) );
		}

		if (theta<0) theta += (float)M_2PI;

		d_out = (float)(theta * (fabs(R)+turningRadiusReference));
	}
	else
	{
		if (sign(x)==sign(K))
		{
			k_out = alpha2index(0);
			d_out = x;
		}
		else
		{
			k_out = alpha2index((float)M_PI);
			d_out = 1e+3;
		}
	}

	// Normalize:
	d_out = d_out / refDistance;

}
