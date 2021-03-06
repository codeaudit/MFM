#include "assert.h"
#include "Grid.h"
#include "Grid_Test.h"
#include "Element_Res.h"

namespace MFM {

  void Grid_Test::Test_gridPlaceAtom()
  {

    ElementRegistry<TestEventConfig> ereg;
    TestGrid grid(ereg,4,3);

    grid.SetSeed(1);
    grid.Init();

    grid.Needed(Element_Res<TestEventConfig>::THE_INSTANCE);

    TestAtom atom(Element_Res<TestEventConfig>::THE_INSTANCE.GetDefaultAtom());

    SPoint gloc(5, 10);

    grid.PlaceAtom(atom, gloc);

    const TestAtom* out = grid.GetAtom(gloc);

    assert(out->GetType() == atom.GetType());

  }
} /* namespace MFM */
