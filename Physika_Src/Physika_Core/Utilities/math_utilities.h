/*
 * @file math_utilities.h 
 * @brief This file is used to define math constants and functions frequently used in Physika.
 * @author FeiZhu
 * 
 * This file is part of Physika, a versatile physics simulation library.
 * Copyright (C) 2013 Physika Group.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
 * If a copy of the GPL was not distributed with this file, you can obtain one at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#ifndef PHYSIKA_CORE_UTILITIES_MATH_UTILITIES_H_
#define PHYSIKA_CORE_UTILITIES_MATH_UTILITIES_H_

#include <limits>
#include <cmath>

namespace Physika{

////////////////////////////////constants//////////////////////////////////////////////
const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;
const float FLOAT_EPSILON = std::numeric_limits<float>::epsilon();
const double DOUBLE_EPSILON = std::numeric_limits<double>::epsilon();

///////////////////////////////functions/////////////////////////////////////////////////

/*
 * abs(), sqrt() are replacement for functions from std because some compilers do not
 * support sqrt and abs of integer type
 */
template <typename Scalar>
inline Scalar abs(Scalar value)
{
    return value>=0?value:-value;
}

template <typename Scalar>
inline Scalar sqrt(Scalar value)
{
    return std::sqrt(value);
}

inline double sqrt(int value)
{
    return std::sqrt(static_cast<double>(value));
}

//undefine the max in WinDef.h
#undef max
template <typename Scalar>
inline Scalar max(Scalar lhs, Scalar rhs)
{
	return lhs > rhs ? lhs : rhs;	
}

//undefine the min in WinDef.h
#undef min
template <typename Scalar>
inline Scalar min(Scalar lhs, Scalar rhs)
{
	return lhs < rhs ? lhs : rhs;
}

}  //end of namespace Physika

#endif //PHYSIKA_CORE_UTILITIES_MATH_UTILITIES_H_