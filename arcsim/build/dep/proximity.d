build/debug/proximity.o build/release/proximity.o: src/proximity.cpp src/proximity.hpp src/cloth.hpp \
  src/dde.hpp src/sparse.hpp src/util.hpp src/mesh.hpp \
  src/transformation.hpp src/spline.hpp src/vectors.hpp src/winport.hpp \
  src/constraint.hpp src/collisionutil.hpp src/bvh.hpp \
  /usr/local/gfortran/lib/gcc/x86_64-apple-darwin15/6.1.0/include/float.h \
  src/geometry.hpp src/magic.hpp src/simulation.hpp src/handle.hpp \
  src/morph.hpp src/obstacle.hpp src/timer.hpp
src/proximity.cpp src/proximity.hpp src/cloth.hpp :
  src/dde.hpp src/sparse.hpp src/util.hpp src/mesh.hpp :
  src/transformation.hpp src/spline.hpp src/vectors.hpp src/winport.hpp :
  src/constraint.hpp src/collisionutil.hpp src/bvh.hpp :
  /usr/local/gfortran/lib/gcc/x86_64-apple-darwin15/6.1.0/include/float.h :
  src/geometry.hpp src/magic.hpp src/simulation.hpp src/handle.hpp :
  src/morph.hpp src/obstacle.hpp src/timer.hpp :
