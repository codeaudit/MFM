#ifndef ELEMENTTABLE_H      /* -*- C++ -*- */
#define ELEMENTTABLE_H

#include "bitfield.h"
#include "eventwindow.h"
#include "eucliddir.h"
#include "itype.h"
#include "p1atom.h"
#include "element.h"
#include "elementtype.h"

namespace MFM {

#define ELEMENT_COUNT 7


template <class T,u32 R>
class EventWindow;

template <class T,u32 R>
class ElementTable
{
private:
  /*
   * This is used to get the "state" field
   * out of an atom.
   */

  typedef u32 (* StateFunction )(T* atom);
  StateFunction m_statefunc;

  // the atomCounts argument can be NULL on any call to these functions.
  /*
  typedef void (* BehaviorFunction )
  (EventWindow<T,R>&, StateFunction f);

  BehaviorFunction m_funcmap[0xff];
  */

  Element<T,R>* m_elementTable[0xff];

  static void FlipSEPointToCorner(Point<s32>& pt, EuclidDir corner);

  /* Fills pt with the coordinates of a randomly selected          */
  /* Atom with the specified type. Returns false if there is none. */
  static bool FillSubWindowContaining(Point<s32>& pt, EventWindow<T,R>& window,
				      ElementType type, StateFunction f, 
				      EuclidDir corner);


  static void FillSubwindowIndices(s32* indices, EventWindow<T,R>& window,StateFunction f,
				   ElementType type, EuclidDir corner);

  static u32 FoundIndicesCount(s32* indices);

  static void ReproduceVertically(EventWindow<T,R>& w, 
				  StateFunction f,
				  ElementType type);
  
public:
  ElementTable();

  static ElementTable<T,R> & get();

  ~ElementTable() { }

  void Execute(EventWindow<T,R>& window)
  { u32 state = m_statefunc(&(window.GetCenterAtom()));
    if(state != ELEMENT_NOTHING)
    m_elementTable[state]->Behavior(window, m_statefunc); }

  void SetStateFunction(StateFunction f);

  void FillAtom(T* atom, ElementType type);

  bool RegisterElement(ElementType type, Element<T,R>* e)
  {
    m_elementTable[(u32)type] = e;
    return true;
  }
  
};

} /* namespace MFM */

#include "elementtable.tcc"

#endif /*ELEMENETTABLE_H*/
