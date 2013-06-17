//=================================================================================================
/*!
//  \file blaze/math/expressions/MatMatSubExpr.h
//  \brief Header file for the MatMatSubExpr base class
//
//  Copyright (C) 2011 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. This library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as published by the
//  Free Software Foundation; either version 3, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with a special
//  exception for linking and compiling against the Blaze library, the so-called "runtime
//  exception"; see the file COPYING. If not, see http://www.gnu.org/licenses/.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_EXPRESSIONS_MATMATSUBEXPR_H_
#define _BLAZE_MATH_EXPRESSIONS_MATMATSUBEXPR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/expressions/SubExpr.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Base class for all matrix/matrix subtraction expression templates.
// \ingroup math
//
// The MatMatSubExpr class serves as a tag for all expression templates that implement a
// matrix/matrix subtraction. All classes, that represent a matrix subtraction and that are
// used within the expression template environment of the Blaze library have to derive from
// this class in order to qualify as matrix subtraction expression template. Only in case a
// class is derived from the MatMatSubExpr base class, the IsMatMatSubExpr type trait
// recognizes the class as valid matrix subtraction expression template.
*/
struct MatMatSubExpr : private SubExpr
{};
//*************************************************************************************************

} // namespace blaze

#endif