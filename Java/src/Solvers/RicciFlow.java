package Solvers;

import Geoquant.Curvature2D;
import Geoquant.Geometry;
import Geoquant.Radius;

public class RicciFlow implements DESystem{
  public double[] calcSlopes(double[] x)
  {
    int i = 0;
    for(Radius r : Geometry.getRadii()){
      r.setValue(x[i]);
      i++;
    }
    
    double[] slopes = new double[x.length];
    i = 0;
    for(Curvature2D K : Geometry.getCurvature2D()){
      slopes[i] = -K.getValue() * x[i];
      i++;
    }
    
    return slopes;
  }
}
