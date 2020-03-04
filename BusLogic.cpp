/* 
 * DSA Coding Puzzle Assignment v.
 * 
 * Candidate:  Scott Vigil  svigil777@gmail.com
 *
 * Build/Test Instructions:
 * Build and test were performed under the latest CygWin toolset.
 * 
 * Console example...
 * svigi@DESKTOP-P9H9KT8 ~/Proj/BusLogic
 * $ g++ -o BusLogic BusLogic.cpp
 * 
 * svigi@DESKTOP-P9H9KT8 ~/Proj/BusLogic
 * $ ./BusLogic.exe input0.txt
 * 70.9 71.9 72.3 73.2 73.9 77.2 77.2 79.4 79.8 82.3 83.3 85.8 87.3 152.3 169.6 170.7 176.3 176.9 177.2 179.4 180.1 181.2 181.9 182 182.6 185.5 188.5 338.1 338.7 339.8 340 340.2 344.1 344.2 344.8 347.3 348.5 350.4 351.3 351.8
 * 
 * Status:
 * I have implemented code to bring input file data in to a vector of "features" named 
 * "feature". Currently, the code simply prints the input back out again. A pass at the 
 * algorithms and class structures has been designed to solve the DSA Coding Puzzle.
 *
 * Todo:
 * o Class definitions needs to be refined. Choice between use of vectors or lists needs 
 *   more attention. 
 * o Algorithmic detail (aka business logic) needs to be realized in code.
 * o Design material and code needs to be saved into GIT.
 * o Design material needs to be written to assigned format.
 * o Questions asked about the code not addressed in these comments need to be answered.
 *
   Requirements freewrite:
        Configure expected Object size: OBJ_SIZE_MIN, OBJ_SIZE_MAX 

        For {Theta: 0 <= Theta < 360}
                Scan feature points from text file, floating point values
        Assume file format where each line has a valid floating point value with <CR><LF> at the end.
        aaa.aaa,bbb.bbb,ccc.ccc,...

        Create a list of features, ListOfFeatures:  
            Attributes: PositionOnCircleDeg, PrevFeature, NextFeature
            Implementation: vector

        Create a list of objects with center of arc object and boundaries, ListOfObjs: 
            Attributes: PositionOnCircleDeg, PrevObj, NextObj
            Implementation: vector

            Algorithm:    
                Initialize object count to zero.
                Set CurrentFeature = index 0.
                While current feature is valid.
                Label: New feature
                Initialize feature count to zero.
                Label: process a feature
                Repeat:
                   Is next feature within 10 degrees? If not, cycle as current feature is outlier. ObjectFound = false
                Else
                    Print the list of objects, PrintObjs

        Assumptions:
        No feature crosses 360 degree boundary. This boundary is set at the tail at the aircraft to minimize omission of an item of interest.
        Object size is between 20 and 30 degrees arc. (Actually, this is far too big and must be made much smaller.)
        Objects are no closer than 20 degrees next to one another. There is no overlap.
        A feature > ½ sigma away from center of object is considered an outlier. For simplicity, assume sigma is 10.

        Optional features:
        Compute sigma for object.
        If feature is omitted from proposed object, it is removed from feature collection and object center is recomputed. Additional features removed from object as needed until no feature is more than one half sigma away from center of object or object is found to have no more than FEATURE_MIN (3) features in it. If this happens, the object is considered to have been nullified (that is, determined to not be an object).
         * 
 */

#include <assert.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector> 

using namespace std;

template <class T>
class csv_istream_iterator: public iterator<input_iterator_tag, T>
{
    istream * _input;
    char _delim;
    string _value;
public:
    csv_istream_iterator( char delim = ',' ): _input( 0 ), _delim( delim ) {}
    csv_istream_iterator( istream & in, char delim = ',' ): _input( &in ), _delim( delim ) { ++*this; }

    const T operator *() const {
        istringstream ss( _value ); 
        T value;
        ss >> value;
        return value;
    }

    istream & operator ++() {
        if( !( getline( *_input, _value, _delim ) ) )
        {
            _input = 0;
        }
        return *_input;
    }

    bool operator !=( const csv_istream_iterator & rhs ) const {
        return _input != rhs._input;
    }
};

template <>
const string csv_istream_iterator<string>::operator *() const {
    return _value;
}

/*
 * CLASS DESIGN - ALGORITHMIC SECTION
 
Assumptions
Naming conventions in algorithm documentation below do not attempt to precisely model expected naming conventions in code. They are meant to be a guide as to what class, attribute and method names may be.

Certain assumptions about the nature of the obstructions are identified in the attribute initializers. These can be tuned for a given obstruction environment with respect to size of obstructions and separation.

TYPES:
    obstacle_t
        currentFeatureID                // Used during traversal of feature vector
        leftEdgeID                      // Obstacle left edge identifier. Index into feature vector
        rightEdgeID                     // Obstacle right edge identifier. Index into feature vector
        location                        // location of obstacle center.
                                        // 0.5 * feature(rightEdgeID) - feature(leftEgeID)
                                        // units in degrees
STRUCTURES:
#   vector<float>       feature;        // Input data of features. Domain {degrees/ 0 <= degrees < 360}
#   vector<obstacle_t>  obstacle;       // Vector of obstacles. This is the output data.

CLASSES:
    Class obstacleHelper:               // Singleton class used to operate on objects vector.

        obstacleHelper Methods:

                CoutObstacle()          // Cout a single obstacle. 'must index into obstacles vector to obtain
                                        // obstacle location values. Cout units in degrees.

    Class Detector:  // Detector class provides "machinery" to traverse through features and search
                     // for obstacles. Main functions, IDFirstObstacle() and IDNextObstacle(), are used 
                     // to "identify", obstacles. This is a singleton class.

        Detector Attributes:
                currentObstacle = 0   // Index into obstacles vector of current obstacle being "examined".
                leftEdgeID            // Index into feature vector of candidate object left edge.
                rightEdgeID           // Index into feature vector of obstacle right edge.
                boolean lastObstacle = false
                                          // True when obstacleHelper has traversed features and no more 
                                          // obstacles can be found.
                maxDiff = 10 degrees    ‘largest angle between features all on the same obstacle.
                minFeatureInObst = 3     This is the smallest number of clumped features to be
                                        Considered an obstacle
                featureCount = 1        Minimum assumed feature count

                currentFeatureID = -1   // Index into feature vector is -1 until vector is traversed.
                boolean lastObstacle = false

        Detector Methods:
                nextFeatureInRange()
                    thetaDiff = data[currentFeatureID+1] - data[currentFeatureID]
                    result = (thetaDiff <= maxDiff)
                    return result
                nextFeatureValid()
                    false if index computed beyond end of vector.
                    ... when currentFeatureID + 1 goes beyond last index in feature vector.

                isFalseLastObstacle()
                    Return true if lastObstacle is false
                    else return false
        
                IDFirstObstacle()               This might be same as IDNextObstacle, but not sure yet.
                    // currentFeatureID = 0                     Assume initialized and valid data input
                    // featureCount = 1                         Assumed for first ID
                    leftEdgeID = currentFeatureID               Index into feature vector for feature defining left 
                                                                edge of obstacle
                    nextFeatureID = currentFeatureID + 1                // Assume valid
                    While (nextFeatureValid) {
                        If nextFeatureInRange 
                            RightEdgeID = CurrentFeatureID
                            FeatureCount++
                            currentFeatureID++
                            If featureCount > minFeatureInObst, set thisIsAnObj = true   
                        ComputeMidpoint()
                    // Done with this obstacle. Next, we move on to evaluate for next feature and to 
                    // see if possibly it is left edge of the next obstacle

                IDNextObstacle()
                    // General: Cycle through the features in order to identify and locate the next obstacle.

                    // currentFeatureID // Available from feature array. Assume valid data.
                    // featureCount                             Currently valid in feature object
                    leftEdgeID = currentFeatureID               // Candidate left edge
                    // nextFeatureID = currentFeatureID + 1     // Assume set and valid by nextFeatureInRange()
                    While (nextFeatureValid) {
                        If nextFeatureInRange 
                            RightEdgeID = CurrentFeatureID
                            FeatureCount++
                            currentFeatureID++
                            If featureCount > minFeatureInObst, set thisIsAnObj = true      
                    // Done with this obstacle. Next, we move on to evaluate for next feature and to 
                    // see if possibly it is left edge of the next obstacle
                    // Todo: Handle condition where we run out of Features. Then, processing ends.
                    // Todo: Handle condition where an obstacle spans the perimeter at the boundary.      
                    //       For example, if an obstacle spans from 358 to 2 degrees.
        ComputeMidpoint()
                     ObstacleMidpoint   = feature[currentFeatureID].rightEdge 
                                                                 - feature[currentFeatureID].leftEdge
                Todo, compute as average of theta values for all points from left edge to right edge.
 */

// TYPES
typedef unsigned  index_t; // Index into a vector.
typedef float     angle_t; // Angular displacement, degrees.

typedef struct
{
   index_t currentFeatureID;            // Used during traversal of feature vector
   index_t leftEdgeID;                  // Obstacle left edge identifier. Index into feature vector
   index_t rightEdgeID;                 // Obstacle right edge identifier. Index into feature vector
   angle_t location;                    // location of obstacle center.
                                        // 0.5 * feature(rightEdgeID) - feature(leftEgeID)
}   obstacle_t;         // units in degrees

class Detector
{

   public:

   // Data Members
   index_t        currentObstacle = 0;    // Index into obstacles vector of current obstacle being "examined".
   index_t        leftEdgeID;             // Index into feature vector of candidate object left edge.
   index_t        rightEdgeID;            // Index into feature vector of obstacle right edge.
   bool           lastObstacle;           // True when obstacleHelper has traversed features and no more 
                                          // obstacles can be found.
   angle_t        maxDiff = 10;           // ‘largest angle between features all on the same obstacle.
                                          //  - simple form of outlier detection. Todo - work this logic a bit.
   unsigned       minFeatureInObst = 3;   // This is the smallest number of clumped features to be
                                          // Considered an obstacle
   unsigned       featureCount = 1;       // Minimum assumed feature count

   index_t        currentFeatureID = 0;   // Index into feature vector is -1 until vector is traversed.
                                          // Todo - use different way to verify unused. -1 
                                          //        for unused doesn't work with unsigned underlying 
                                          //        type.

   vector<float> &feature_r;              // Pointer to feature vector of floats.

   // Member Functions()

   Detector(vector<float> &featureInit_r)
      : feature_r(featureInit_r)
   {
   }

   bool nextFeatureInRange()
   {
      angle_t thetaDiff;
      bool     result;     // ToDo, place a pointer in Detector class that is initialized
                           // upon instantiation... of Detector.

      thetaDiff = feature_r[currentFeatureID + 1] - feature_r[currentFeatureID];
      result    = (thetaDiff <= maxDiff);
      return result;
   }

   bool isFalseLastObstacle()
   {
      if (lastObstacle)
      {
         return true;
      }
      else
      {
         return false;
      }
   }

}; // Detector

/*
 * MAIN
 */
int main( int argc, char **argv )
{
/*
       DATA STRUCTURES SECTION:
*/
   vector<float>        feature;       // Vector of features... input data.
   vector<obstacle_t>   obstacle;      // Vector of obstacles. This is the output data.
   Detector             detector(feature);
                                       // Todo, provide address of feature to constructor.

   cout << "minFeatureInObst = " << detector.minFeatureInObst << endl;  // Tst
/*
       DATA IMPORT SECTION:
*/
    assert(argc >= 2); // Todo: Make more elegant check for usage errors.
    ifstream fin(argv[1]);
    if( fin )
    {
        copy( csv_istream_iterator<float>( fin ),
              csv_istream_iterator<float>(),
              insert_iterator< vector<float> >( feature, feature.begin() ) );

        fin.close();
    }


/* 
        ALGORITHMIC SECTION:
       
        IDFirstObstacle(Features)
        If CurrentFeatureID == -1, quit
        lastObstacle = false

        While (isFalseLastLastObstacle())
            IDNextObstacle(Features)
*/

/*
       OUTPUT SECTION:
       feature now contains all floating point values in the comma-delimited
       data file.  now dump them to the screen to verify:
*/
    copy( feature.begin(), feature.end(), ostream_iterator<float>( cout, " " ) );

    // Todo: Replace output of input data with output of Algorithmic Section

    //  obstacleHelper.Cout()
    //      For (i=0; i>numObstacles; i++)
    //          CoutObstacle()

    return 0;
}
