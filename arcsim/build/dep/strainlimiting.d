build/debug/strainlimiting.o build/release/strainlimiting.o: src/strainlimiting.cpp src/strainlimiting.hpp \
  src/cloth.hpp src/dde.hpp src/sparse.hpp src/util.hpp src/mesh.hpp \
  src/transformation.hpp src/spline.hpp src/vectors.hpp src/winport.hpp \
  src/constraint.hpp src/optimization.hpp src/simulation.hpp \
  src/handle.hpp src/morph.hpp src/obstacle.hpp src/timer.hpp \
  /usr/local/gfortran/lib/gcc/x86_64-apple-darwin15/6.1.0/include/omp.h \
  src/io.hpp
src/strainlimiting.cpp src/strainlimiting.hpp :
  src/cloth.hpp src/dde.hpp src/sparse.hpp src/util.hpp src/mesh.hpp :
  src/transformation.hpp src/spline.hpp src/vectors.hpp src/winport.hpp :
  src/constraint.hpp src/optimization.hpp src/simulation.hpp :
  src/handle.hpp src/morph.hpp src/obstacle.hpp src/timer.hpp :
  /usr/local/gfortran/lib/gcc/x86_64-apple-darwin15/6.1.0/include/omp.h :
  src/io.hpp :
