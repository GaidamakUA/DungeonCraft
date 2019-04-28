/******************************************************************************
* Filename: Utilities.cpp
* Copyright (c) 2013, UAF Development Team (email CocoaSpud@hotmail.com)
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
******************************************************************************/
#include "stdafx.h"
#include <string>

#include "Utilities.h"


const char *IntToString(int value)
{
   static char strTemp[1024];
   sprintf(strTemp, "%i", value);
   return strTemp;
}

const char *DoubleToString(double value)
{
   return IntToString((int)value);
}

// assigning empty CString to a json object results in 'null' instead of ""
const char *CStringOrEmptyString(const CString &str)
{
   if (str.GetLength() == 0)
      return "";
   else
      return str;
}
