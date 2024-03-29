package tests;

import inputOutput.TriangulationIO;
import visualization.PlanarDevelopment;


public class PlanarDevelopmentTest {

  public static void main(String[] args) {
    
    TriangulationIO.read2DTriangulationFile("Data/flip_test/eight_triangles_redux.txt");
    
    PlanarDevelopment pd = new PlanarDevelopment();
    
    System.out.println("2");
    
    pd.generatePlane();
    
    System.out.println("3");
    
    pd.printForPython();
  }
  
}
