#ifndef EUCANGLE_CPP_
#define EUCANGLE_CPP_

#include <new>
#include <cmath>
#include <cstdio>

#include "geoquant.h"
#include "triposition.h"

#include "triangulation.h"
#include "vertex.h"
#include "face.h"

#include "miscmath.h"

class EuclideanAngle : public virtual GeoQuant {
 private:
  GeoQuant* lengthA;
  GeoQuant* lengthB;
  GeoQuant* lengthC;
  
 public:  
  EuclideanAngle(Vertex v, Face f, GQIndex& gqi);
  void recalculate(); 
};

EuclideanAngle::EuclideanAngle(Vertex v, Face f, GQIndex& gqi) : GeoQuant (){
  position = new TriPosition(ANGLE, 2, v.getSerialNumber(), f.getSerialNumber());
  dataID = ANGLE;

  vector<int> sameAs, diff;
  sameAs = listIntersection(v.getLocalEdges(), f.getLocalEdges());
  diff = listDifference(f.getLocalEdges(), v.getLocalEdges());  

  TriPosition t1(LENGTH, 1, Triangulation::edgeTable[sameAs[0]].getSerialNumber());
  TriPosition t2(LENGTH, 1, Triangulation::edgeTable[sameAs[1]].getSerialNumber());
  TriPosition t3(LENGTH, 1, Triangulation::edgeTable[diff[0]].getSerialNumber());  

  lengthA = gqi[ t1 ];
  lengthB = gqi[ t2 ];
  lengthC = gqi[ t3 ];

  if(lengthA == NULL || lengthB == NULL || lengthC == NULL){
    fprintf(stderr, "Error: Length Quantity Not Found.\n");
    exit(0);
  }

  lengthA->addDependent(this);
  lengthB->addDependent(this);
  lengthC->addDependent(this);
}

void EuclideanAngle::recalculate(){
  double a = lengthA->getValue();
  double b = lengthB->getValue();
  double c = lengthC->getValue();
  value = acos((a*a + b*b - c*c)/(2*a*b));
}

void Init_EuclideanAngles(GQIndex& gqi){
  map<int, Vertex>::iterator vit;
  for(vit = Triangulation::vertexTable.begin();
      vit != Triangulation::vertexTable.end(); vit++){
    vector<int>* faces = vit->second.getLocalFaces();
    
    for(int ii = 0; ii < faces->size(); ii++){
      Face& f = Triangulation::faceTable[ faces->at(ii) ];
      EuclideanAngle* ea = new EuclideanAngle(vit->second, f, gqi);
      GeoQuant* gq = gqi[ea->getPosition()];
      if(gq != NULL) {
	fprintf(stdout, "Angle ejected!\n");
	gq->getPosition().print();
      }
      gqi[ ea->getPosition() ] = ea;            
    }
  }
}

#endif /* EUCANGLE_CPP_ */