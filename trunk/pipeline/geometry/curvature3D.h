#ifndef THREEDCURVATURE_H_
#define THREEDCURVATURE_H_

#include <cmath>
#include <map>
#include <new>
using namespace std;

#include "triangulation.h"

#include "geoquant.h"
#include "triposition.h"

#include "edge_curvature.h"
#include "partial_edge.h"

class Curvature3D : public virtual GeoQuant {
private:
  vector<GeoQuant*>* edgeCurvs;
  vector<GeoQuant*>* partials;

protected:
  Curvature3D( Vertex& v );
  void recalculate();

public:
  ~Curvature3D();
  static Curvature3D* At( Vertex& v );
  static void CleanUp();
  static void Record( char* filename );
};

#endif /* THREEDCURVATURE_H_ */
