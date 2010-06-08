package InputOutput;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.PrintStream;
import java.util.HashMap;
import java.util.Scanner;
import java.util.regex.Pattern;

import Geoquant.Length;
import Geoquant.Radius;
import Geoquant.TriPosition;
import Triangulation.*;

public class TriangulationIO {
  private TriangulationIO() {
    
  }
  
  public static void read2DTriangulationFile(String filename) {
    read2DTriangulationFile(new File(filename));
  }
  
  public static void read2DTriangulationFile(File file) {
    Scanner fileScanner;
    Scanner stringScanner;
    String type;
    Simplex simplex;
    int index;
    Vertex v;
    Edge e;
    Face f;
    
    try {
      fileScanner = new Scanner(file);
    } catch (FileNotFoundException ex) {
      // TODO Auto-generated catch block
      System.err.print("File given by " + file + " could not be found\n");
      ex.printStackTrace();
      return;
    }
    
    while(fileScanner.hasNextLine()) {
      stringScanner = new Scanner(fileScanner.nextLine());
      type = stringScanner.next();
      index = stringScanner.nextInt();
      
      if(type.compareTo("Vertex:") == 0) {
        simplex = Triangulation.vertexTable.get(index);
        if(simplex == null) {
          simplex = new Vertex(index);
          Triangulation.putVertex((Vertex) simplex);
        }
      } else if(type.compareTo("Edge:") == 0) {
          simplex = Triangulation.edgeTable.get(index);
          if(simplex == null) {
            simplex = new Edge(index);
            Triangulation.putEdge((Edge) simplex);
          }       
      } else if(type.compareTo("Face:") == 0) {
          simplex = Triangulation.faceTable.get(index);
          if(simplex == null) {
            simplex = new Face(index);
            Triangulation.putFace((Face) simplex);
          }   
      } else {
        System.err.print("Invald simplex type " + type + "\n");
        Triangulation.reset();
        return;
      }
      
      // Local vertices
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        v = Triangulation.vertexTable.get(index);
        if(v == null) {
          v = new Vertex(index);
          Triangulation.putVertex(v);
        }
        simplex.addVertex(v);
      }
      
      // Local edges
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        e = Triangulation.edgeTable.get(index);
        if(e == null) {
          e = new Edge(index);
          Triangulation.putEdge(e);
        }
        simplex.addEdge(e);
      }
      
      // Local faces
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        f = Triangulation.faceTable.get(index);
        if(f == null) {
          f = new Face(index);
          Triangulation.putFace(f);
        }
        simplex.addFace(f);
      }
      
      // Check for radii / lengths
      if(fileScanner.hasNextDouble()) {
        stringScanner = new Scanner(fileScanner.nextLine());
        if(type.compareTo("Vertex:") == 0) {
          Radius.At((Vertex) simplex).setValue(stringScanner.nextDouble());
        } else if(type.compareTo("Edge:") == 0) {
          Length.At((Edge) simplex).setValue(stringScanner.nextDouble());
        }
      }
    }
  }
  
  public static void read3DTriangulationFile(String filename) {
    read3DTriangulationFile(new File(filename));
  }
  
  public static void read3DTriangulationFile(File file) {
    Scanner fileScanner;
    Scanner stringScanner;
    String type;
    Simplex simplex;
    int index;
    Vertex v;
    Edge e;
    Face f;
    Tetra t;
    
    try {
      fileScanner = new Scanner(file);
    } catch (FileNotFoundException ex) {
      // TODO Auto-generated catch block
      System.err.print("File given by " + file + " could not be found\n");
      ex.printStackTrace();
      return;
    }
    
    while(fileScanner.hasNextLine()) {
      stringScanner = new Scanner(fileScanner.nextLine());
      type = stringScanner.next();
      index = stringScanner.nextInt();
      
      if(type.compareTo("Vertex:") == 0) {
        simplex = Triangulation.vertexTable.get(index);
        if(simplex == null) {
          simplex = new Vertex(index);
          Triangulation.putVertex((Vertex) simplex);
        }
      } else if(type.compareTo("Edge:") == 0) {
          simplex = Triangulation.edgeTable.get(index);
          if(simplex == null) {
            simplex = new Edge(index);
            Triangulation.putEdge((Edge) simplex);
          }       
      } else if(type.compareTo("Face:") == 0) {
          simplex = Triangulation.faceTable.get(index);
          if(simplex == null) {
            simplex = new Face(index);
            Triangulation.putFace((Face) simplex);
          }   
      } else if(type.compareTo("Tetra:") == 0) {
        simplex = Triangulation.tetraTable.get(index);
        if(simplex == null) {
          simplex = new Tetra(index);
          Triangulation.putTetra((Tetra) simplex);
        }       
      } else {
        System.err.print("Invald simplex type " + type + "\n");
        Triangulation.reset();
        return;
      }
      
      // Local vertices
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        v = Triangulation.vertexTable.get(index);
        if(v == null) {
          v = new Vertex(index);
          Triangulation.putVertex(v);
        }
        simplex.addVertex(v);
      }
      
      // Local edges
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        e = Triangulation.edgeTable.get(index);
        if(e == null) {
          e = new Edge(index);
          Triangulation.putEdge(e);
        }
        simplex.addEdge(e);
      }
      
      // Local faces
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        f = Triangulation.faceTable.get(index);
        if(f == null) {
          f = new Face(index);
          Triangulation.putFace(f);
        }
        simplex.addFace(f);
      }
      
      // Local tetra
      stringScanner = new Scanner(fileScanner.nextLine());
      while(stringScanner.hasNextInt()) {
        index = stringScanner.nextInt();
        t = Triangulation.tetraTable.get(index);
        if(t == null) {
          t = new Tetra(index);
          Triangulation.putTetra(t);
        }
        simplex.addTetra(t);
      }
      
      // Check for radii / lengths
      if(fileScanner.hasNextDouble()) {
        if(type.compareTo("Vertex:") == 0) {
          Radius.At((Vertex) simplex).setValue(fileScanner.nextDouble());
        } else if(type.compareTo("Edge:") == 0) {
          Length.At((Edge) simplex).setValue(fileScanner.nextDouble());
        }
      }
    }
  }
  
  public static void read2DLutzFile(String filename) {
    read2DLutzFile(new File(filename));
  }
  
  public static void read2DLutzFile(File file) {
    String faces;
    Scanner scanner = null;
    Scanner line;
    HashMap<TriPosition, Edge> edgeList = new HashMap<TriPosition, Edge>();
    Vertex v;
    Edge e;
    Face f;
    Vertex[] verts = new Vertex[3];
    int index;
    TriPosition T;
    
    try {
      scanner = new Scanner(file);
    } catch (FileNotFoundException ex) {
      ex.printStackTrace();
    }
    
    faces = "";
    while(scanner.hasNextLine()) {
      faces = faces.concat(scanner.nextLine());
    }
    
    faces = faces.substring(faces.lastIndexOf("=") + 1);
    faces = faces.replaceAll("[^0-9],[^0-9]", "\n");
    faces = faces.replaceAll(",", " ");
    faces = faces.replaceAll("[^0-9 \n]", "");
        
    scanner = new Scanner(faces);
    while(scanner.hasNextLine()) {
      line = new Scanner(scanner.nextLine());
      
      //Create face;
      f = new Face(Triangulation.greatestFace() + 1);
      Triangulation.putFace(f);
      
      // Fill out verts, create vertices, add to face
      for(int i = 0; i < verts.length; i++) {
        index = line.nextInt();
        v = Triangulation.vertexTable.get(index);
        if(v == null) {
          v = new Vertex(index);
          Triangulation.putVertex(v);
        }
        v.addFace(f);
        f.addVertex(v);
        verts[i] = v;
        // add to other vertices
        for(int j = 0; j < i; j++) {
          if(!verts[j].isAdjVertex(v)) {
            verts[j].addVertex(v);
            v.addVertex(verts[j]);
          }
        }
      }
      
      // Build edges
      for(int i = 0; i < verts.length; i++) {
        for(int j = 0; j < i; j++) {
          T = new TriPosition(verts[i].getIndex(), verts[j].getIndex());
          e = edgeList.get(T);
          if(e == null) {
            e = new Edge(Triangulation.greatestEdge() + 1);
            Triangulation.putEdge(e);
            edgeList.put(T, e);
            for(Edge e2 : verts[i].getLocalEdges()) {
              e.addEdge(e2);
              e2.addEdge(e);
            }
            for(Edge e2 : verts[j].getLocalEdges()) {
              e.addEdge(e2);
              e2.addEdge(e);
            }
            verts[i].addEdge(e);
            verts[j].addEdge(e);
            e.addVertex(verts[j]);
            e.addVertex(verts[i]);
          } else {
            for(Face f2 : e.getLocalFaces()) {
              f.addFace(f2);
              f2.addFace(f);
            }
          }
          e.addFace(f);
          f.addEdge(e);
        }
      }
    }
  }
  
  public static void read3DLutzFile(String filename) {
    read3DLutzFile(new File(filename));
  }
  
  public static void read3DLutzFile(File file) {
    String tetras;
    Scanner scanner = null;
    Scanner line;
    HashMap<TriPosition, Edge> edgeList = new HashMap<TriPosition, Edge>();
    HashMap<TriPosition, Face> faceList = new HashMap<TriPosition, Face>();
    Vertex v;
    Edge e;
    Face f;
    Tetra t;
    Vertex[] verts = new Vertex[4];
    int index;
    TriPosition T;
    
    try {
      scanner = new Scanner(file);
    } catch (FileNotFoundException ex) {
      ex.printStackTrace();
    }
    
    tetras = "";
    while(scanner.hasNextLine()) {
      tetras = tetras.concat(scanner.nextLine());
    }
    
    tetras = tetras.substring(tetras.lastIndexOf("=") + 1);
    tetras = tetras.replaceAll("[^0-9],[^0-9]", "\n");
    tetras = tetras.replaceAll(",", " ");
    tetras = tetras.replaceAll("[^0-9 \n]", "");
        
    scanner = new Scanner(tetras);
    while(scanner.hasNextLine()) {
      line = new Scanner(scanner.nextLine());
    
      // Create tetra
      t = new Tetra(Triangulation.greatestTetra() + 1);
      Triangulation.putTetra(t);
      
      // Fill out verts, create vertices, add to face
      for(int i = 0; i < verts.length; i++) {
        index = line.nextInt();
        v = Triangulation.vertexTable.get(index);
        if(v == null) {
          v = new Vertex(index);
          Triangulation.putVertex(v);
        }
        v.addTetra(t);
        t.addVertex(v);
        verts[i] = v;
        // add to other vertices
        for(int j = 0; j < i; j++) {
          if(!verts[j].isAdjVertex(v)) {
            verts[j].addVertex(v);
            v.addVertex(verts[j]);
          }
        }
      }
      
      // Build edges
      for(int i = 0; i < verts.length; i++) {
        for(int j = 0; j < i; j++) {
          T = new TriPosition(verts[i].getIndex(), verts[j].getIndex());
          e = edgeList.get(T);
          if(e == null) {
            e = new Edge(Triangulation.greatestEdge() + 1);
            Triangulation.putEdge(e);
            edgeList.put(T, e);
            for(Edge e2 : verts[i].getLocalEdges()) {
              e.addEdge(e2);
              e2.addEdge(e);
            }
            for(Edge e2 : verts[j].getLocalEdges()) {
              e.addEdge(e2);
              e2.addEdge(e);
            }
            verts[i].addEdge(e);
            verts[j].addEdge(e);
            e.addVertex(verts[j]);
            e.addVertex(verts[i]);
          }
          e.addTetra(t);
          t.addEdge(e);
        }
      }
      
      // Build faces
      Edge eij, eik, ejk;
      for(int i = 0; i < verts.length; i++) {
        for(int j = 0; j < i; j++) {
          for(int k = 0; k < j; k++) {
            T = new TriPosition(verts[i].getIndex(), verts[j].getIndex(), verts[k].getIndex());
            f = faceList.get(T);
            if(f == null) {
              f = new Face(Triangulation.greatestFace() + 1);
              Triangulation.putFace(f);
              faceList.put(T, f);
              // Vertices
              f.addVertex(verts[k]);
              f.addVertex(verts[j]);
              f.addVertex(verts[i]);
              verts[i].addFace(f);
              verts[j].addFace(f);
              verts[k].addFace(f);
              
              //Edges
              eij = edgeList.get(new TriPosition(verts[i].getIndex(), verts[j].getIndex()));
              eik = edgeList.get(new TriPosition(verts[i].getIndex(), verts[k].getIndex()));
              ejk = edgeList.get(new TriPosition(verts[j].getIndex(), verts[k].getIndex()));
              for(Face f2 : ejk.getLocalFaces()) {
                f.addFace(f2);
                f2.addFace(f);
              }
              for(Face f2 : eik.getLocalFaces()) {
                f.addFace(f2);
                f2.addFace(f);
              }
              for(Face f2 : eij.getLocalFaces()) {
                f.addFace(f2);
                f2.addFace(f);
              }
              eij.addFace(f);
              eik.addFace(f);
              ejk.addFace(f);
              f.addEdge(ejk);
              f.addEdge(eik);
              f.addEdge(eij);
            } else {
              for(Tetra t2 : f.getLocalTetras()) {
                t.addTetra(t2);
                t2.addTetra(t);
              }
            }
            f.addTetra(t);
            t.addFace(f);
          }
        }
      }     
    }
  }
  
  public static void write2DTriangulationFile(String filename) {
    try {
      write2DTriangulationFile(new PrintStream(filename));
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }
  
  public static void write2DTriangulationFile(PrintStream out) {
    for(Vertex v : Triangulation.vertexTable.values()) {
      out.printf(v + ":\n");
      for(Vertex v2 : v.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : v.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : v.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
    }
    for(Edge e : Triangulation.edgeTable.values()) {
      out.printf(e + ":\n");
      for(Vertex v2 : e.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : e.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : e.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
    }
    for(Face f : Triangulation.faceTable.values()) {
      out.printf(f + ":\n");
      for(Vertex v2 : f.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : f.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : f.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
    }
    out.close();
  }
  
  public static void write3DTriangulationFile(String filename) {
    try {
      write3DTriangulationFile(new PrintStream(filename));
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }
  
  public static void write3DTriangulationFile(PrintStream out) {
    for(Vertex v : Triangulation.vertexTable.values()) {
      out.printf(v + ":\n");
      for(Vertex v2 : v.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : v.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : v.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
      for(Tetra t2 : v.getLocalTetras()) {
        out.print(t2.getIndex() + " ");
      }
      out.println();
    }
    for(Edge e : Triangulation.edgeTable.values()) {
      out.printf(e + ":\n");
      for(Vertex v2 : e.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : e.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : e.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
      for(Tetra t2 : e.getLocalTetras()) {
        out.print(t2.getIndex() + " ");
      }
      out.println();
    }
    for(Face f : Triangulation.faceTable.values()) {
      out.printf(f + ":\n");
      for(Vertex v2 : f.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : f.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : f.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
      for(Tetra t2 : f.getLocalTetras()) {
        out.print(t2.getIndex() + " ");
      }
      out.println();
    }
    for(Tetra t : Triangulation.tetraTable.values()) {
      out.printf(t + ":\n");
      for(Vertex v2 : t.getLocalVertices()) {
        out.print(v2.getIndex() + " ");
      }
      out.println();
      for(Edge e2 : t.getLocalEdges()) {
        out.print(e2.getIndex() + " ");
      }
      out.println();
      for(Face f2 : t.getLocalFaces()) {
        out.print(f2.getIndex() + " ");
      }
      out.println();
      for(Tetra t2 : t.getLocalTetras()) {
        out.print(t2.getIndex() + " ");
      }
      out.println();
    }
    out.close();
  }
}