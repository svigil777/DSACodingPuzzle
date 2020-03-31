DSA Coding Puzzle Assignment v.2

Candidate:  Scott Vigil  svigil777@gmail.com

3/13/2020:

DSA Coding Puzzle Assignment v.

Candidate:  Scott Vigil  svigil777@gmail.com

Build/Test Instructions:
Build and test were performed under the latest CygWin toolset.

Console example...

svigi@DESKTOP-P9H9KT8 /cygdrive/c/cygwin64/home/svigi/Proj/BusLogic
$ g++ -o BusLogic BusLogic.cpp

svigi@DESKTOP-P9H9KT8 /cygdrive/c/cygwin64/home/svigi/Proj/BusLogic
$ ./BusLogic i2.txt
BusLogic 3/13/2020 15:41
f_it:
70.9, NextValid:1, Next:71.9, Diff:1, NextInRange:1
71.9, NextValid:1, Next:72.3, Diff:0.400002, NextInRange:1
72.3, NextValid:1, Next:82.3001, Diff:10.0001, NextInRange:0
82.3001, NextValid:0

Status:
I have implemented code to bring input file data in to a vector of "features" named 
"feature". Currently, the code simply prints the input back out again. A pass at the 
algorithms and class structures has been designed to solve the DSA Coding Puzzle.

Now, many of the methods of the Detector class have been implemented and are
up and running.

Todo:
o Complete final Detector methods.
o Product output section.
o Tune parameters for expected dataset.
o Cleanup.

3/3/2020:

Build/Test Instructions:
Build and test were performed under the latest CygWin toolset.

Console example...
svigi@DESKTOP-P9H9KT8 ~/Proj/BusLogic
$ g++ -o BusLogic BusLogic.cpp

svigi@DESKTOP-P9H9KT8 ~/Proj/BusLogic
$ ./BusLogic.exe input0.txt
70.9 71.9 72.3 73.2 73.9 77.2 77.2 79.4 79.8 82.3 83.3 85.8 87.3 152.3 169.6 170.7 176.3 176.9 177.2 179.4 180.1 181.2 181.9 182 182.6 185.5 188.5 338.1 338.7 339.8 340 340.2 344.1 344.2 344.8 347.3 348.5 350.4 351.3 351.8

Status:
I have implemented code to bring input file data in to a vector of "features" named 
"feature". Currently, the code simply prints the input back out again. A pass at the 
algorithms and class structures has been designed to solve the DSA Coding Puzzle.
 *
Todo:
o Class definitions needs to be refined. Choice between use of vectors or lists needs 
  more attention. 
o Algorithmic detail (aka business logic) needs to be realized in code.
o Design material and code needs to be saved into GIT.
o Design material needs to be written to assigned format.
o Questions asked about the code not addressed in these comments need to be answered.

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
    vector<float>       feature;        // Input data of features. Domain {degrees/ 0 <= degrees < 360}
    vector<obstacle_t>  obstacle;       // Vector of obstacles. This is the output data.

CLASSES:
    Class obstacleHelper:               // Singleton class used to operate on objects vector.

        obstacleHelper Methods:

             CoutObstacle()          // Cout a single obstacle. 'must index into obstacles vector to obtain
                                        // obstacle location values. Cout units in degrees.

    Class detector:  // Detector class provides "machinery" to traverse through features and search
                     // for obstacles. Main functions, IDFirstObstacle() and IDNextObstacle(), are used 
                     // to "identify", obstacles. This is a singleton class.

        detector Attributes:
           CurrentObstacle = 0   // Index into obstacles vector of current obstacle being "examined".
           leftEdgeID            // Index into feature vector of candidate object left edge.
           rightEdgeID           // Index into feature vector of obstacle right edge.
           boolean lastObstacle = false
                                          // True when obstacleHelper has traversed features and no more 
                                          // obstacles can be found.
        obstacleHelper Attributes:
          maxDiff = 10 degrees    ‘largest angle between features all on the same obstacle.
          minfeatureInObj = 3     This is the smallest number of clumped features to be
                                  Considered an obstacle
          featureCount = 1        Minimum assumed feature count

          currentFeatureID = -1   // Index into feature vector is -1 until vector is traversed.
          boolean lastObstacle = false

        detector Methods:
          nextFeatureInRange()
              return “quit” if nextFeatureValid is false
              thetaDiff = data[currentFeatureID+1] - data[currentFeatureID]
              result = (thetaDiff <= maxDiff)
              return result


          isFalseLastObstacle()
              Return true if lastObstacle is false
              else return false
        
          IDFirstObstacle()               This might be same as IDNextObstacle, but not sure yet.
              // currentFeatureID = 0                     Assume initialized and valid data input
              // featureCount = 1                         Assumed for first ID
              leftEdgeID = currentFeatureID               Index into feature vector for feature defining left 
                                                          edge of obstacle
              nextFeatureID = currentFeatureID + 1                // Assume valid
              While (nextFeatureInRange) {
                  If nextFeatureInRange 
                      RightEdgeID = CurrentFeatureID
                      FeatureCount++
                      currentFeatureID++
                      If featureCount > minfeatureInObj, set thisIsAnObj = true   
                  ComputeMidpoint()
              // Done with this obstacle. Next, we move on to evaluate for next feature and to 
              // see if possibly it is left edge of the next obstacle

          IDNextObstacle()
              // General: Cycle through the features in order to identify and locate the next obstacle.

              // currentFeatureID // Available from feature array. Assume valid data.
              // featureCount                             Currently valid in feature object
              leftEdgeID = currentFeatureID               // Candidate left edge
              // nextFeatureID = currentFeatureID + 1     // Assume set and valid by nextFeatureInRange()
              While (nextFeatureInRange) {
                  If nextFeatureInRange 
                      RightEdgeID = CurrentFeatureID
                      FeatureCount++
                      currentFeatureID++
                      If featureCount > minfeatureInObj, set thisIsAnObj = true      
              // Done with this obstacle. Next, we move on to evaluate for next feature and to 
              // see if possibly it is left edge of the next obstacle
              // Todo: Handle condition where we run out of Features. Then, processing ends.
              // Todo: Handle condition where an obstacle spans the perimeter at the boundary.      
              //       For example, if an obstacle spans from 358 to 2 degrees.

            ComputeMidpoint()
               ObstacleMidpoint   = feature[currentFeatureID].rightEdge 
                                                                 - feature[currentFeatureID].leftEdge
             Todo, compute as average of theta values for all points from left edge to right edge.

       DATA IMPORT SECTION:

        vector<float> feature;

        assert(argc >= 2); // Todo: Make more elegant check for usage errors.
        ifstream fin(argv[1]);
        if( fin )
        {
            copy( csv_istream_iterator<float>( fin ),
                  csv_istream_iterator<float>(),
                  insert_iterator< vector<float> >( feature, feature.begin() ) );

            fin.close();
        }

        ALGORITHMIC SECTION:
       
        IDFirstObstacle(Features)
        If CurrentFeatureID == -1, quit
        lastObstacle = false

        While (isFalseLastLastObstacle())
            IDNextObstacle(Features)

       OUTPUT SECTION:

        obstacleHelper.Cout()
            For (i=0; i>numObstacles; i++)
                CoutObstacle()
