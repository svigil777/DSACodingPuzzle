            Obstacle Detector Skills Demonstration Project

                           Scott Vigil  scott@svis.com, svigil777@gmail.com

4/4/2020 Results:

   The Obstacle Detector State Machine has been produced and run on 
   features data provided by KBRwyle, contactor for NASA. It has 
   analyzed features provided and recognized objects in that data as 
   presented below.  While the Obstacle Detector does a great job of 
   detecting obstacles presented by rather simple feature datasets, it 
   is in the end a demonstration of development capabilities and 
   process on my part.  For a real-world application, more work on the 
   analysis algorithm. Also, more apriori knowledge of the features 
   environment will be necessary. This will allow for proper tuning of 
   the Obstacle Detector and more accurate obstacle recognition “in the 
   wild”.  

   Results are discussed in detail in the file, 

      "Data Analysis and Discussion.pdf".

3/31/2020 Status:

                     Obstacle Detection State Machine

   One thing I realized as I was implementing the previous delivery was 
   that the complexity was getting a bit out of hand. It just seemed that
   my simplistic approach toward the behavior of the Obstacle Detection
   algorithm was missing some important use case scenarios. 

   So, I decided to produce a state machine for this next effort. That
   construct allows for behavioral modification based upon previous 
   actions. Please see drawing in "Obstacle Identification State 
   Machine.pdf".


   I have realized the design. That is, I have implemented the design in 
   code form. In addition, I have written two test cases on a spread sheet 
   that exercise all but two flows of the state machine. I have written an 
   input test file that supplies the inputs as documented in the test, 
   i4.txt. And, I have watched in horror as everything fell apart. lol

                       Initial Verification Test Case

   And, after suitable rest and recovery, I have come back, instrumented 
   the code, observed it's internal states as it progressed through the test 
   cases, watched as sometimes the code did the wrong thing and sometimes 
   the test supplied the incorrect data for exercising the intended states, 
   transitions and computations.  Per guidance from Agile and the Unified 
   Development Process, I have used iterative, incremental development and 
   test techniques to successively refine the design, the code and the test
   cases.

                            Additional Test Cases

   After adding three more test cases, the code, the drawing, and the 
   spreadsheet containing the test cases are in agreement and all flows are 
   verified per the cases documented in "Verification Test Cases.xlsx" and 
   "Verification Test Cases.pdf". Log files capture and demonstrate test 
   case results.

                           Previously Defined Tasks

   As part of this, the following items have also been completed.
   o Class definitions needs to be refined. Choice between use of vectors or 
     lists needs more attention. 
   o Algorithmic detail (aka business logic) needs to be realized in code.
   o Design material and code needs to be saved into GIT.
   o Design material needs to be written to assigned format.
   o Questions asked about the code not addressed in these comments need to 
     be answered.

Where to go from here:

   The next step that I will take, time permitting, is to experiment with 
   KBRwyle provided feature data and tune state machine parameters to 
   optimize performance for NASA provided features data. There are two. 
   The first is maxDiff. This value is the largest angle between features 
   that are part of the same obstacle. It was set arbitrarily at 10 
   degrees. At this setting if you have a cluster of features that form 
   an obstacle, any feature that is greater than ten degrees to the right 
   or to the left of the outermost feature are considered not part of that 
   obstacle. 

   The second tunable parameter is minFeatureInObst. This is the smallest 
   number of features that must be clumped together in order to have an 
   obstacle. That value was initially set to three. However, it was 
   increased to four simply to make it possible to verify all the flows in 
   the state machine.

   Clearly, this is a rather simplistic algorithm. And yet, depending 
   upon the characteristics of features in the environment, it will have 
   a varied degree of success. Therefore, tuning these parameters for the 
   obstacle environment is an activity that should be performed. This can 
   be done apriori by studying the expected obstacle environment. Or, it 
   could be performed real-time adaptively by monitoring feature 
   characteristics.

   On the Boeing autopilots, we had five or seven flight modes. Different 
   gain schedules were used for the flight modes. Clearly, such an approach 
   could be used with this algorithm.

Requirements:

   Below is an improved set of requirements. This list has been refined
   based on experiences gained on producing the implementation to this stage.

    1.  An object shall be considered a collection of Nmin or more features 
        clustered together, not more than Nd (ten degrees) apart. 
        Default configuration follows:

        Nd = 10, Nmin = 4

   2.   For {Theta: 0 <= Theta < 360}
                Scan feature points from text file, floating point values

   3.   Input text file shall be structured with a series of comma separated 
        floating point values, nominally formatted as...

            aaa.aaa,bbb.bbb,ccc.ccc,...

        Assume the file has <CR><LF> at the end.

   4.   Create a list of features, ListOfFeatures:   
            Attributes: PositionOnCircleDeg, PrevFeature, NextFeature
            Implementation: vector

   5.   Output a list of feature locations in units degrees to standard output.

  Assumptions:
   1.   No feature crosses 360 degree boundary. This boundary is set at the 
        tail of the aircraft to minimize omission of an item of interest. For 
        craft not having a tail, assume that the 360 degree boundary is in 
        the direction opposite of intended travel.

   2.   Object size is between 20 and 30 degrees arc.  << Assumption removed. >>

   3.   A feature with an arc distance from an obstacle > 10 degrees is no a 
        part of that obstacle.

   4.   And outlier feature is one that is greater than ten degrees of arc 
        from the next closest feature in an obstacle.

Build/Test Instructions:
Build and test were performed under the latest CygWin toolset.

Console example...

svigi@DESKTOP-P9H9KT8 /cygdrive/c/cygwin64/home/svigi/Proj/BusLogic
$ g++ -o BusLogic BusLogic.cpp

svigi@DESKTOP-P9H9KT8 /cygdrive/c/cygwin64/home/svigi/Proj/BusLogic
$ ./BusLogic.exe i4.txt
BusLogic 3/28/2020 22:42
Obstacle Detection State Machine
  obstruction center.. 48, left edge.., 41.2, right edge.., 54.8
  obstruction center.. 66.5, left edge.., 65, right edge.., 68

To obtain instrumented output, edit BusLogic.cpp and set desired "tstn" 
variables to "true" and re-build.


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
