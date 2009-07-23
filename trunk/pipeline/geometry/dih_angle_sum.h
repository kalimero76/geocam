#ifndef DIHEDRALANGLESUM_H_
#define DIHEDRALANGLESUM_H_

#include <map>
#include <new>
using namespace std;

#include "triangulation.h"
#include "geoquant.h"
#include "triposition.h"

#include "dih_angle.h"

class DihedralAngleSum : public virtual GeoQuant {
private:
  vector<DihedralAngle*>* angles;

protected:
  DihedralAngleSum( Edge& e );
  void recalculate();

public:
  ~DihedralAngleSum();
  static DihedralAngleSum* At( Edge& e );
  static void CleanUp();
  static void Record( char* filename );
};

#endif /* DIHEDRALANGLESUM_H_ */