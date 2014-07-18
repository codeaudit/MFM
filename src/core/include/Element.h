/*                                              -*- mode:C++ -*-
  Element.h Base of all MFM elemental types
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
  \file Element.h Base of all MFM elemental types
  \author Trent R. Small.
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ELEMENT_H
#define ELEMENT_H

#include "itype.h"
#include "StaticLoader.h"
#include "EventWindow.h"
#include "UUID.h"
#include "Dirs.h"
#include "Logger.h"

namespace MFM
{
  typedef u32 ElementType;

  template <class CC> class Atom; // Forward declaration

  /**
   * An Element describes how a given type of Atom behaves.
   */
  template <class CC>
  class Element
  {
   private:
    typedef typename CC::ATOM_TYPE T;
    typedef typename CC::PARAM_CONFIG P;
    enum { R = P::EVENT_WINDOW_RADIUS };

    /**
     * The unique UUID of this Element . Each Element must be given
     * one upon construction.
     */
    const UUID m_UUID;

    /**
     * The type shared by every Atom of this Element.
     */
    u32 m_type;

    /**
     * A flag indicating whether or not the type of this Atom (held by
     * \c m_type ) is a valid type.
     */
    bool m_hasType;

    /**
     * A flag used when rendering every Atom of this Element which
     * determines which color to use during rendering.
     */
    bool m_renderLowlight;

    /**
     * The basic, most generic Atom of this Element to be used when
     * placing a new Atom.
     */
    T m_defaultAtom;

    /**
     * The Atomic Symbol of this Element, i.e. a One or Two character
     * string representing a very short name for this Element.
     */
    const char* m_atomicSymbol;

   protected:

    /**
     * Describes how the default Atom of this Element should be
     * constructed. The default behavior of this method builds an atom
     * with the type of this Element and otherwise empty fields.
     *
     * @returns The default Atom of this Element, to be stored in \c m_defaultAtom .
     */
    virtual T BuildDefaultAtom() const
    {
      T defaultAtom(this->GetType(), 0, 0, 0);
      return defaultAtom;
    }

    /**
     * Retrieves the read-only BitVector held inside a specified Atom .
     *
     * @param atom The Atom to retrieve the held BitVector from.
     *
     * @returns The BitVector held inside \c atom .
     */
    const BitVector<P::BITS_PER_ATOM> & GetBits(const T & atom) const
    {
      return atom.m_bits;
    }

    /**
     * Retrieves the writable BitVector held inside a specified Atom .
     *
     * @param atom The Atom to retrieve the held BitVector from.
     *
     * @returns The BitVector held inside \c atom .
     */
    BitVector<P::BITS_PER_ATOM> & GetBits(T & atom) const
    {
      return atom.m_bits;
    }

    /**
     * Checks to see if a specified Atomic Symbol is a valid symbol. A
     * valid symbol must be either a single capital letter, or a
     * capital letter followed by a single lowercase letter.
     *
     * @param symbol The string to check symbolic validity of.
     *
     * @returns \c true If \c symbol is a valid atomic symbol, else \c
     *          false .
     */
    bool IsValidAtomicSymbol(const char* symbol)
    {
      u32 len = strlen(symbol);

      if(len == 2)
      {
        return (symbol[0] >= 'A' && symbol[0] <= 'Z') &&
               (symbol[1] >= 'a' && symbol[1] <= 'z');
      }
      else if(len == 1)
      {
        return (symbol[0] >= 'A' && symbol[0] <= 'Z');
      }
      else
      {
        return false;
      }
    }

    /**
     * Sets the Atomic Symbol of this Element, which is mainly used
     * during the rendering of this Element .
     *
     * @param symbol The string representing the Atomic Symbol of this
     *               Element. If this symbol is not valid (in terms of
     *               the \c IsValidAtomicSymbol() method), this will
     *               FAIL with ILLEGAL_ARGUMENT .
     */
    void SetAtomicSymbol(const char* symbol)
    {
      if(!IsValidAtomicSymbol(symbol))
      {
        /* Single or double letters only, like the periodic table. */
        FAIL(ILLEGAL_ARGUMENT);
      }
      m_atomicSymbol = symbol;
    }

    /**
     * The four Von Neumann neighbors, represented as four unit
     * vectors in the four cardinal directions.
     */
    static const SPoint VNNeighbors[4];

    /**
     * Fills a specified SPoint with a randomly selected Von Neumann
     * neighbor of the center of a specified EventWindow .
     *
     * @param window The EventWindow whose center will be scanned for
     *               an empty Von Neumann neighbor.
     *
     * @param pt The SPoint to fill a Von Neumann unit vector with.
     *
     * @returns \c true if an empty Von Neumann neighbor is found,
     *          else \c false .
     */
    bool FillAvailableVNNeighbor(EventWindow<CC>& window, SPoint& pt) const;

    /**
     * Master search method for finding an Atom inside an EventWindow .
     *
     * @param window The EventWindow to search.
     *
     * @param pt Output, representing a valid location in this search
     *           which satisfies the search parameters.
     *
     * @param relevants The range of SPoints which should be searched
     *                  using this method.
     *
     * @param relevantCount The number of SPoints held by \c relevants .
     *
     * @param rotation A symmetric rotation, describing the flipping
     *                 of the relevant points. This can be used to
     *                 search for a particular corner, then flipped to
     *                 searchfor another.
     *
     * @param type The type of the Element to search for during this
     *             search.
     *
     *
     * @returns \c true if any Atoms are found which satisfy the
     *          search parameters, else \c false .
     */
    bool FillPointWithType(EventWindow<CC>& window,
                           SPoint& pt, const SPoint* relevants, u32 relevantCount,
                           Dir rotation, ElementType type) const;

    /**
     * Diffuses the central Atom of a given EventWindow based on the
     * empty places around the Atom and the odds that the central Atom
     * can be diffused.
     *
     * @param window The EventWindow of which the central Atom should
     *               attempt to be diffused.
     */
    void Diffuse(EventWindow<CC>& window) const;


    /**
     * Determines how likely an Atom of this type is to be swapped
     * with during diffusal.
     *
     * @returns an integral percentage, from 0 to 100, describing the
     *          desire of an atom of this type to be moved; 0 being
     *          impossible to move and 100 being completely fine with
     *          moving.
     */
    virtual u32 PercentMovable(const T& you,
                               const T& me, const SPoint& offset) const = 0;

   public:

    /**
     * Constructs a new Element with a given UUID .
     *
     * @param uuid The UUID of this Element, used for various
     *              operations. It is vital that this UUID be unique,
     *              but this constructor does not check for that.
     */
    Element(const UUID & uuid) : m_UUID(uuid), m_type(0),
                                 m_hasType(false),
                                 m_renderLowlight(false),
                                 m_atomicSymbol("!!")
    {
      LOG.Debug("Constructed %@",&m_UUID);
    }

    // For use by Element_Empty only!
    Element(const UUID & uuid, u32 type) : m_UUID(uuid), m_type(type),
                                           m_hasType(true),
                                           m_renderLowlight(false),
                                           m_atomicSymbol("!!")
    { }

    /**
     * Assigns the type of this Element using the U16StaticLoader
     * . This type is only assigned if it has not been assigned
     * already. Once this type has been allocated, the default Atom of
     * this Element is constructed and an Atom of this Element may be
     * placed.
     *
     * @sa U16StaticLoader;
     */
    void AllocateType()
    {
      if (!m_hasType)
      {
        m_type = U16StaticLoader::AllocateType(m_UUID);
        m_hasType = true;
        m_defaultAtom = BuildDefaultAtom();
      }
    }

    /**
     * Gets the unique type of this Element . If the type has not been
     * assigned yet (by using \c AllocateType() ), this will FAIL with
     * ILLEGAL_STATE .
     *
     * @returns The unique type of this Element .
     */
    u32 GetType() const
    {
      if (!m_hasType)
      {
        FAIL(ILLEGAL_STATE);
      }
      return m_type;
    }

    /**
     * Gets the Atomic Symbol of this Element . If this has not been
     * set, the default Atomic Symbol is the invalid symbol "!!" .
     *
     * @returns The Atomic Symbol of this Element.
     */
    const char* GetAtomicSymbol() const
    {
      return m_atomicSymbol;
    }

    /**
     * Checks to see if this Element is of a specified type.
     *
     * @param type The \c type of which to check against they type of
     *             this Element.
     *
     * @returns \c true if this Element is of \c type type, else \c
     *          false.
     */
    bool IsType(u32 type) const
    {
      return GetType() == type;
    }

    /**
     * Gets the UUID of this Element .
     *
     * @returns The UUID of this Element.
     */
    const UUID & GetUUID() const
    {
      return m_UUID;
    }

    /**
     * Describes the behavior of this Element. This is invoked when an
     * Atom of this Element is chosen as the center of an EventWindow
     * for an Event, which is given as a parameter.
     *
     * @param window The EventWindow describing the Event which is
     * currently being executed.
     */
    virtual void Behavior(EventWindow<CC>& window) const = 0;

    /**
     * Gets the default Atom of this Element . If this Element has not
     * been assigned a type through \c AllocateType() , this will FAIL
     * with ILLEGAL_STATE .
     *
     * @returns The default Atom of this Element.
     */
    const T & GetDefaultAtom() const
    {
      if (!m_hasType)
      {
        FAIL(ILLEGAL_STATE);
      }
      return m_defaultAtom;
    }

    /**
     * Gets the current 32-bit ARGB color which this all Atoms of this
     * Element should be rendered with.
     *
     * @param The selected current 32-bit ARGB color which is
     *        suggested to render any Atom of this Element with.
     */
    virtual u32 PhysicsColor() const
    {
      if(m_renderLowlight)
      {
        return DefaultLowlightColor();
      }
      else
      {
        return DefaultPhysicsColor();
      }
    }

    /**
     * Gets the 32-bit ARGB formatted color that all Atoms of this
     * Element will be drawn with.
     *
     * @returns The 32-bit ARGB formatted color that all Atoms of this
     *          Element will be drawn with.
     */
    virtual u32 DefaultPhysicsColor() const = 0;

    /**
     * Gets the 32-bit ARGB formatted color that all Atoms of this
     * Element will be drawn with when they have lowlight drawing
     * enabled.
     *
     * @returns The lowlight color to draw all Atoms of this Element
     *          with.
     */
    virtual u32 DefaultLowlightColor() const
    {
      u8 r, g, b;
      u32 oc = DefaultPhysicsColor();

      /* Shift them one more to divide by 2 */
      r = (oc & 0x00ff0000) >> 17;
      g = (oc & 0x0000ff00) >> 9;
      b = (oc & 0x000000ff) >> 1;

      return 0xff000000 | (r << 16) | (g << 8) | b;
    }

    /**
     * Toggles the lowlight / normal rendering of all Atoms of this
     * Element.
     */
    void ToggleLowlightPhysicsColor()
    {
      m_renderLowlight = !m_renderLowlight;
    }

    /**
     * Used during rendering, will select a color for any Atom of this
     * Element to be rendered with. This should be overridden if
     * wanting to use a gradient or some other variable color based on
     * the body of the specified Atom .
     *
     * @param atom The Atom of this element of which to find a color
     *             for.
     *
     * @param selector An additional argument which may be used to
     *                 determine the color of which to render \c atom
     *                 .
     *
     * @returns The 32-bit ARGB color of which to render \c atom with.
     */
    virtual u32 LocalPhysicsColor(const T& atom, u32 selector) const
    {
      return PhysicsColor();
    }

    /**
     * On entry, the Atom at \a nowAt will be an instance of the type
     * of this Element.  How much does that atom like the idea that it
     * should be moved to (or remain at; the two SPoints might be
     * equal) location \a maybeAt?  Return 0 to flat-out veto the
     * idea, or from 0 up to COMPLETE_DIFFUSABILITY to request that
     * fraction of diffusability.  (Values larger than
     * COMPLETE_DIFFUSABILITY are treated as equal to it.)  Note that
     * \a nowAt and \a maybeAt are both relative to the EventWindow \a
     * ew, but neither is necessarily the center atom!
     *
     * By default all atoms are considered diffusable, and they return
     * COMPLETE_DIFFUSABILITY chances for every suggested move
     */
    virtual u32 Diffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const
    {
      return COMPLETE_DIFFUSABILITY;
    }

    /**
       A utility method available to subclasses that specifies no
       diffusion is allowed, by specifying a 0 value for the
       Diffusability of every position except when \a nowAt == \a
       maybeAt.
    */
    u32 NoDiffusability(EventWindow<CC> & ew, SPoint nowAt, SPoint maybeAt) const
    {
      return (nowAt == maybeAt)? COMPLETE_DIFFUSABILITY : 0;
    }

    /**
     * A standard basis for specifying degrees of diffusability.
     */
    static const u32 COMPLETE_DIFFUSABILITY = 1000;

  };
}

#include "Element.tcc"

#endif /* ELEMENT_H */
