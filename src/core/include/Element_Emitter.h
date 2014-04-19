/* -*- C++ -*- */
#ifndef ELEMENT_EMITTER_H
#define ELEMENT_EMITTER_H

#include "Element.h"
#include "EventWindow.h"
#include "ElementTable.h"
#include "Element_Empty.h"
#include "Element_Data.h"
#include "Element_Reprovert.h"
#include "itype.h"
#include "P1Atom.h"

namespace MFM
{

#define DATA_CREATE_PER_1000 150

  template <class CC>
  class Element_Emitter : public Element_Reprovert<CC>
  {
    // Extract short names for parameter types
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

    const T m_defaultAtom;

  public:
    // Element Data Slot names
    enum {
      DATUMS_EMITTED_SLOT,
      DATUMS_REJECTED_SLOT,
      DATA_SLOT_COUNT
    };

    const char* GetName() const { return "Emitter"; }

    static Element_Emitter THE_INSTANCE;
    static const u32 TYPE = 0xdada1;

    Element_Emitter() : m_defaultAtom(BuildDefaultAtom()) {}

    const T BuildDefaultAtom() const {
      T defaultAtom(TYPE,0,0,Element_Reprovert<CC>::STATE_BITS);
      this->SetGap(defaultAtom,2);
      return defaultAtom;
    }

    virtual const T & GetDefaultAtom() const
    {
      return m_defaultAtom;
    }

    virtual u32 DefaultPhysicsColor() const
    {
      return 0xff808080;
    }

    virtual void Behavior(EventWindow<CC>& window) const
    {
      Random & random = window.GetRandom();

      this->ReproduceVertically(window);

      if(random.OddsOf(DATA_CREATE_PER_1000,1000))
      {
        Tile<CC> & tile = window.GetTile();
        ElementTable<CC> & et = tile.GetElementTable();

        u64 * datap = et.GetDataAndRegister(TYPE,DATA_SLOT_COUNT);
        ++datap[DATUMS_EMITTED_SLOT];                  // Count emission attempts

        // Pick random nearest empty, if any
        const MDist<R> md = MDist<R>::get();
        for (u32 ring = 1; ring <= 2; ++ring) {
          u32 emptiesFound = 0;
          SPoint emptyPoint;
          for (u32 idx = md.GetFirstIndex(ring); idx <= md.GetLastIndex(ring); ++idx) {
            const SPoint sp = md.GetPoint(idx);
            const T other = window.GetRelativeAtom(sp);
            const u32 otherType = other.GetType();
            bool isEmpty = otherType == Element_Empty<CC>::TYPE;
            if (isEmpty && random.OneIn(++emptiesFound))
              emptyPoint = sp;
          }
          if (emptiesFound > 0) {
            T atom = Element_Data<CC>::THE_INSTANCE.GetDefaultAtom();
            Element_Data<CC>::THE_INSTANCE.SetDatum(atom,random.Between(DATA_MINVAL, DATA_MAXVAL));
            window.SetRelativeAtom(emptyPoint, atom);
            return;
          }
        }

        ++datap[DATUMS_REJECTED_SLOT];  // Opps, no room at the inn
      }
    }

    static void Needed();

  };

  template <class CC>
  Element_Emitter<CC> Element_Emitter<CC>::THE_INSTANCE;

  /*
  template <class CC>
  void Element_Emitter<CC>::Needed()
  {
    ElementTable<CC>::get().RegisterElement(Element_Emitter<CC>::THE_INSTANCE);
  }
  */
}

#endif /* ELEMENT_EMITTER_H */