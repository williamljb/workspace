build/debug/main.o build/release/main.o: src/main.cpp src/misc.hpp src/io.hpp src/mesh.hpp \
  src/transformation.hpp src/spline.hpp src/vectors.hpp src/winport.hpp \
  src/runphysics.hpp src/simulation.hpp src/cloth.hpp src/dde.hpp \
  src/sparse.hpp src/util.hpp src/constraint.hpp src/handle.hpp \
  src/morph.hpp src/obstacle.hpp src/timer.hpp
src/main.cpp src/misc.hpp src/io.hpp src/mesh.hpp :
  src/transformation.hpp src/spline.hpp src/vectors.hpp src/winport.hpp :
  src/runphysics.hpp src/simulation.hpp src/cloth.hpp src/dde.hpp :
  src/sparse.hpp src/util.hpp src/constraint.hpp src/handle.hpp :
  src/morph.hpp src/obstacle.hpp src/timer.hpp :
