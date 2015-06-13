/*                                              -*- mode:C++ -*-
  Element_Xtal_L12.h Left-tipped, knight's-L-move crystal
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file Element_Xtal_L12.h Left-tipped, knight's-L-move crystal
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_XTAL_L12_H
#define ELEMENT_XTAL_L12_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "AbstractElement_Xtal.h"
#include "itype.h"

namespace MFM
{

  template <class EC>
  class Element_Xtal_L12 : public AbstractElement_Xtal<EC>
  {
    enum {  ELT_VERSION = 2 };

    // Extract short names for parameter types
    typedef typename EC::ATOM_CONFIG AC;
    typedef typename AC::ATOM_TYPE T;
    enum { R = EC::EVENT_WINDOW_RADIUS };

  private:

  public:

    static Element_Xtal_L12 THE_INSTANCE;

    Element_Xtal_L12(const UUID & uuid) : AbstractElement_Xtal<EC>(uuid)
    {
    }

    Element_Xtal_L12() : AbstractElement_Xtal<EC>(MFM_UUID_FOR("XtalL12", ELT_VERSION))
    {
      Element<EC>::SetAtomicSymbol("Xl");
      Element<EC>::SetName("L1,2 crystal");
    }

    virtual u32 GetElementColor() const
    {
      return 0xff11bb33;
    }

    typedef typename MFM::AbstractElement_Xtal<EC>::XtalSites XtalSites;

    virtual u32 GetSymI(T &atom, EventWindow<EC>& window) const
    {
      return (u32) PSYM_NORMAL;
    }

    virtual void GetSites(T & atom, XtalSites & sites, EventWindow<EC>& window) const
    {
      static XtalSites isites;
      static bool initted = false;
      if (!initted)
      {
        const SPoint ones[] = {
          SPoint(0,0),
          SPoint(2,-1),
          SPoint(-1,-2),
          SPoint(-2,1),
          SPoint(1,2),
          SPoint(1,-3),
          SPoint(-3,-1),
          SPoint(-1,3),
          SPoint(3,1)
        };
        for (u32 i = 0; i < sizeof(ones)/sizeof(ones[0]); ++i)
        {
          this->WriteBit(isites, ones[i], true);
        }
        initted = true;
      }
      sites = isites;
    }

  };

  template <class EC>
  Element_Xtal_L12<EC> Element_Xtal_L12<EC>::THE_INSTANCE;

}

#endif /* ELEMENT_XTAL_L12_H */
