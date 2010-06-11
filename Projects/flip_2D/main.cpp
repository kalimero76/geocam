#include "hinge_flip.h"
#include "TriangulationDisplay.h"
#include "delaunay.h"
#include "FlipAlgorithm.h"
#include "triangulation.h"
#include "Matrix.h"

#include "miscmath.h"


using namespace std;

void printStuff(ostream *output) {
  //ofstream output("../../Data/flip_test/otherInfo.txt");
  *output << "Lengths\n";
  map<int, Edge>::iterator eit;
  eit = Triangulation::edgeTable.begin();
  for (; eit != Triangulation::edgeTable.end(); eit++) {
    *output << "Edge " << eit->first << "{" << (*(eit->second).getLocalVertices())[0] << (*(eit->second).getLocalVertices())[1] << "}" << "\t" << Length::valueAt(eit->second) << "\n";
  }

  *output << "Radii\n";
  map<int, Vertex>::iterator vit;
  vit = Triangulation::vertexTable.begin();
  for (; vit != Triangulation::vertexTable.end(); vit++) {
    *output << "Vertex " << vit->first << "\t" << Radius::valueAt(vit->second) << "\n";
  }

  *output << "Triangles\n";
  map<int, Face>::iterator fit;
  fit = Triangulation::faceTable.begin();
  vector<int> * indices;
  for (; fit != Triangulation::faceTable.end(); fit++) {
    *output << "Face " << fit->first << "\n";
    indices = (fit->second).getLocalVertices();
    for (int i = 0; i < indices->size(); i++) {
      *output << (*indices)[i] << " ";
    }
    *output << "\n";
    indices = (fit->second).getLocalEdges();
    for (int i = 0; i < indices->size(); i++) {
      *output << (*indices)[i] << " ";
    }
    *output << "\n";
    indices = (fit->second).getLocalFaces();
    for (int i = 0; i < indices->size(); i++) {
      *output << (*indices)[i] << " ";
    }
    *output << "\n";
  }
}

/*
 *  main takes the name of a file to read from as its only argument
 *
 *  to give it an argument in dev, go to Execute > Parameters...
 *  then in the top field labeled "Parameters to pass to your program"
 *  type the file name containing the triangulation data you want used
 */
int main(int argc, char** argv) {
    char* inputFile;

    //cout << "starting!\n";
    //makeTriangulationFile("test_files/toms_triangulation.lutz", "test_files/toms_triangulation.txt");
    //cout << "done!\n";
    //system("PAUSE");
    //exit(0);

    if (argc <= 1) {
            cout << "\nATTENTION: YOU NEED TO SPECIFY A FILE TO BE READ BY GIVING MAIN AN ARGUMENT!!!\n"
             << "READ THE COMMENT ABOVE MAIN's DEFINITION\n\nif you press any key to continue now,"
             << " the file being used will default to test_files/convex_pair.txt\n\n";
            system("PAUSE");
            inputFile = "../../Data/flip_test/non_convex_pair.txt";

    } else {
            inputFile = argv[1];
    }
    char outFile[strlen(inputFile)+5];
    strcpy(outFile, inputFile);
    strcpy(&outFile[strlen(inputFile)], ".out");


    //two different ways to set of the display info
    readTriangulationFile(inputFile);
    TriangulationDisplay::setFile(inputFile);

    //opportunity to change the radius info
    int radiiLength = 300;
    double radii[] = {0.256781349327,0.196672798255,0.407583295119,0.328531900965,0.444885190593,0.489962752632,0.483032224703,0.257251882185,0.475132873684,0.273493560808,0.225008250761,0.468981832947,0.207181126077,0.296626021264,0.300121827385,0.456877702773,0.0220446256105,0.246407406716,0.0691686637731,0.208417504999,0.160482593225,0.437716112496,0.248770196426,0.0463714224012,0.4784275176,0.15078510154,0.151064231491,0.242528037825,0.499548237228,0.00457743466339,0.356687662502,0.193269031628,0.0835042395297,0.0154430722909,0.0488458912792,0.328229188591,0.161697259808,0.135232465356,0.312836160105,0.303050930229,0.232758948898,0.239181080098,0.403359823987,0.325143338325,0.476291850166,0.0337086925016,0.235740522744,0.0901084919311,0.228742666101,0.240663554542,0.282984198039,0.0933572531287,0.193622085353,0.40776559007,0.369996658683,0.310921297853,0.443754782307,0.459710793093,0.0951470793904,0.340979846485,0.150900320969,0.265108663103,0.167594043282,0.0136582427962,0.00623786690749,0.294442058126,0.130796545206,0.386264057635,0.212658073226,0.331840660788,0.49539617728,0.308933075382,0.0147170808718,0.14612433727,0.435232717511,0.129217424106,0.216841461924,0.25773665695,0.258242162066,0.468060268885,0.169717785738,0.349110551935,0.47374476732,0.374066569919,0.091479391663,0.420143755883,0.45765938395,0.440484530832,0.145604253324,0.309732704561,0.0136163251262,0.0380510582206,0.409151236631,0.346133697318,0.442570058626,0.327659586613,0.10334798642,0.137594475853,0.424392691289,0.0643677396635,0.0265257788321,0.440324600109,0.431471999453,0.29622885298,0.132141669429,0.0917369238979,0.363219282809,0.36274453186,0.172519520076,0.0763420612216,0.355262229036,0.389336065629,0.148796898173,0.285085875758,0.127946404516,0.441750166054,0.0196157429497,0.262792407794,0.193108775963,0.0409503619491,0.297532565512,0.240979209544,0.330379108069,0.47386477574,0.220236620312,0.495560538163,0.265128814758,0.0959213168731,0.184905456329,0.00600696270157,0.276562440924,0.414040743197,0.0146615283456,0.0744303882713,0.480942878843,0.198331648826,0.124508681937,0.199124887349,0.48861166845,0.220894260751,0.245130285198,0.498256480054,0.387423662066,0.0923256727161,0.144813943801,0.27083324169,0.208532975077,0.133427584295,0.27108780262,0.309824935995,0.467446117654,0.0201435099467,0.383169804854,0.190177995214,0.0818769032481,0.0805051409551,0.15084291865,0.362767034755,0.132306467916,0.292245470409,0.238934540509,0.328068175657,0.455247949179,0.0565563230116,0.465370537176,0.0382456483021,0.237572427711,0.320742405029,0.106277351745,0.197847601511,0.185050729534,0.390094685061,0.0602487115,0.0699575940451,0.298799526036,0.238628124007,0.446035212554,0.0968388242408,0.385443370746,0.324475459226,0.419429275788,0.0847549521731,0.240958265398,0.362489933685,0.0519934645786,0.311721209125,0.174905343768,0.184006089555,0.0991592517199,0.0313098901122,0.333605632608,0.256461827388,0.493827765961,0.0040759932956,0.0475634684331,0.225975191832,0.0945843228083,0.0424374188713,0.393396075322,0.00710252788034,0.24534168253,0.389738366643,0.365786102349,0.342635393756,0.0623643473897,0.00522652146076,0.327343496994,0.165533647262,0.476334392954,0.363406510126,0.0506646358766,0.0463245117051,0.0635561066775,0.0429640687994,0.00531723030739,0.379331739874,0.258859851467,0.448714408884,0.205824299826,0.470822922403,0.368418197435,0.436505053353,0.118677790702,0.00941895881222,0.081893526794,0.395380575593,0.315376021075,0.194045035604,0.357075501296,0.154997845971,0.100725805005,0.484627556095,0.181204383806,0.439904675671,0.106443938743,0.384539215335,0.481646158468,0.332203100241,0.0193525229771,0.112852568596,0.475327677474,0.0696791826007,0.247147976484,0.246691920494,0.378237166418,0.0223102120144,0.090640506572,0.330208769819,0.137963907893,0.357909565806,0.240980291658,0.449378870578,0.488012307865,0.0335753603291,0.106858873714,0.0909313597262,0.233005123466,0.142669426678,0.494419739331,0.334202005645,0.371604143032,0.00210264516934,0.375491691401,0.416978622585,0.246782436765,0.480009845663,0.0669025122099,0.288304251301,0.42295871033,0.0963401812992,0.321296477946,0.180508094617,0.467738215145,0.0938703543612,0.466423091755,0.4852188936,0.142833793067,0.0651391613675,0.00684489585176,0.21655314326,0.425022619404,0.0129713138997,0.432970379665,0.130614942674,0.17009581612,0.354588497245,0.423859967473,0.284402071321,0.0508873563503,0.0441220925133,0.40826721813,0.289835317347,0.297517371217,0.45026212613,0.190162433516,0.423466830674,0.232434073657,0.108196242012,0.0588905280561,0.138251986117,0.067792680658};

    int numEdges = Triangulation::edgeTable.size();

    //save the length info
    double lengths[numEdges];
    map<int, Edge>::iterator eit = Triangulation::edgeTable.begin();
    for (; eit != Triangulation::edgeTable.end(); eit++) {
        lengths[eit->first] = Length::valueAt(eit->second);
    }

    //add radius info
    int index = 0;
    map<int,Vertex>::iterator vit = Triangulation::vertexTable.begin();
    for (; vit != Triangulation::vertexTable.end(); vit++, index++) {
        Radius::At(vit->second)->setValue(radii[index]);
    }

    //testing
    //vit = Triangulation::vertexTable.begin();
    //for (; vit != Triangulation::vertexTable.end(); vit++) {
    //    cout << Radius::valueAt(vit->second) << "\n";
    //}


    //reapply length info
    eit = Triangulation::edgeTable.begin();
    for (; eit != Triangulation::edgeTable.end(); eit++) {
        Length::At(eit->second)->setValue(lengths[eit->first]);
    }

    TriangulationDisplay::ShowTriangulation();

    //TriangulationDisplay::ShowTriangulation(inputFile, argc, argv);
    return 0;
}