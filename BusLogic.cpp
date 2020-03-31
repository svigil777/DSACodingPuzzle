/* 
 * DSA Coding Puzzle Assignment v.
 * 
 * Candidate:  Scott Vigil  svigil777@gmail.com
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

// TYPES
typedef unsigned  index_t; // Index into a vector.
typedef float     angle_t; // Angular displacement, degrees.

class Detector
{

   public:

   // Data Members
   vector<angle_t>::iterator lEdge_it;      // Interator into feature vector of candidate object left edge.
   vector<angle_t>::iterator rEdge_it;      // Interator into feature vector of obstacle right edge.

   bool           lastObstacle;           // True when obstacleHelper has traversed features and no more 
                                          // obstacles can be found.
   angle_t        maxDiff = 10;           // ‘largest angle between features all on the same obstacle.
                                          //  - simple form of outlier detection. Todo - work this logic a bit.
   unsigned       minFeatureInObst = 4;   // This is the smallest number of clumped features to be
                                          // Considered an obstacle
   unsigned       featureCount = 1;       // Minimum assumed feature count
   bool           isAnObstacle = false;   // True, when the current feature is part of an object.
   angle_t        location;               // location of obstacle center.
                                          // (feature(rightEdge) - feature(leftEge)) / 2
   vector<angle_t>            &feature_r; // Pointer to feature vector of angle_t (float) values.
   vector<angle_t>::iterator  f_it;       // feature iterator.

   // Member Functions()

   // Initialize feature iterator in constructor.
   Detector(vector<angle_t> &featureInit_r)
      : feature_r(featureInit_r)
   {
   }

   bool nextFeatureInRange()
   {
      angle_t                   thetaDiff;
      bool                      result = false;
      bool                      tst4   = false;      // Enable nextFeatureInRange() debug statements.
      vector<angle_t>::iterator n_it;
      n_it = f_it + 1;              // Assume next value in vector is valid.
                                    // upon instantiation... of Detector.
      if (tst4) { cout << ", nfir... Next:" << *(n_it); }

      // thetaDiff = nextfeature_Degrees - currentFeature_Degrees
      thetaDiff = *n_it - *f_it;
      if (tst4) { cout << ", nfir... Diff:" << (angle_t)thetaDiff; }

      result    = (thetaDiff <= maxDiff);
      if (tst4) { cout << ", nfir... NextInRange:" << result << endl; }
      return result;
   }

   bool nextFeatureValid()
   {
       //    false if pointer computed beyond end of vector.
       // ... when currentFeatureIterator + 1 goes beyond last entry in feature vector.
       //
       // f = current iterator value.
       // n = f + 1 = next iterator value
       // return ( (n is not last element) );

      bool result = false;
      bool tst5 = false;
      vector<angle_t>::iterator n_it;
      n_it = f_it + 1;
      result = (n_it != feature_r.end());
      if (tst5) {cout << "NextValid:" << result;}

     return (result);
   }

   // Return true if number of features sufficient to consider cluster as an obstacle.
   bool featureCountSufficient(void)
   {
     return featureCount >= minFeatureInObst;
   }

   void outputObstacleRecord(void)
   {
     bool enableObstacleOutput = true; // Enable record output

     if (enableObstacleOutput)
     { // Output obstacle data

        cout  
           << "  obstruction center.. " 
           << obstacleCenter() << ", "
           << "left edge.., "  << *lEdge_it << ", "
           << "right edge.., " << *rEdge_it << endl; 
     }
   }

   angle_t obstacleCenter()
   {
     location = (*lEdge_it + *rEdge_it)/2;
     return location;
   }
 
}; // Detector

/*
 * MAIN
 */
int main( int argc, char **argv )
{
   cout << "BusLogic 3/30/2020 21:39" << endl;
   cout << "Obstacle Detection State Machine" << endl;
/*
       DATA STRUCTURES SECTION:
*/
   vector<angle_t>        feature;       // Vector of features... input data.
   Detector               d(feature);    // Singleton instance of Detector class.

/*
       DATA IMPORT SECTION:
*/
    assert(argc >= 2);
    ifstream fin(argv[1]);
    if( fin )
    {
        copy( csv_istream_iterator<angle_t>( fin ),
              csv_istream_iterator<angle_t>(),
              insert_iterator< vector<angle_t> >( feature, feature.begin() ) );

        fin.close();
    }

/* 
        ALGORITHMIC SECTION:
*/
       bool tst1 = false; // Detailed debug statements
       bool tst2 = false; // Top of For Loop debug statements

       enum class eState {freeFeature, obstacleCandidate, obstacleOpen, terminate};
       eState state = eState::freeFeature;

        // On entry:
        d.featureCount = 1;
        d.isAnObstacle = false;

        if (tst1) { cout << "t0, freeFeature "; }
        for (d.f_it = feature.begin() ; d.f_it != feature.end(); ++d.f_it) 
        {
           if (tst2) { cout << endl << endl << "TOF:  *d.f_it " << *d.f_it 
              << "d.featureCount " << d.featureCount 
              << endl; }
           if (state == eState::freeFeature)
           {
              if (tst1) { cout << "freeFeature  " << endl; }
              // Todo - Add flow from freeFeature to terminate here and on diagram.
              // Todo - test flow t1t
              // Last feature...
              if  (d.nextFeatureValid() == false)
              {
                 if (tst1) { cout << "t1t, "; }
                 state = eState::terminate;
              }
              else if (d.nextFeatureInRange())
              {
                 if (tst1) { cout << "t1a, "; }
                 d.featureCount++;
                 d.lEdge_it = d.f_it;
                 state = eState::obstacleCandidate;
                 if (tst1) { cout << "  lEdge " << *d.lEdge_it << ", " << endl; }
              }
              // not nextFeatureInRange
               else // if ((d.nextFeatureInRange()) == false)
              {
                 if (tst1) { cout << "t1b, "; }
                 d.featureCount = 1; // Todo - remove from code here and drawing. Verify no change in behavior.
              }
           }
           else if (state == eState::obstacleCandidate)
           {
             // Last feature and feature count sufficient...
              if  ((d.nextFeatureValid() == false) && d.featureCountSufficient())
              {
                 if (tst1) { cout << "t2t, "; }
                 d.rEdge_it = d.f_it;  // Capture right edge of obstacle.
                 if (tst1) { cout << "  rEdge " << *d.rEdge_it << ", " << endl; }
                 d.outputObstacleRecord();
                 state = eState::terminate;
              }
              // Last feature in range
              else if (d.nextFeatureInRange()             //  If current is the end,
                 && d.featureCountSufficient())           //  and sufficient features
              {                                           //  to be considered obstacle.
                 if (tst1) { cout << "t2a, "; }
                 d.isAnObstacle = true;
                 state = eState::obstacleOpen;
             }
              else if (d.nextFeatureInRange() && (d.featureCountSufficient() == false))
              {
                 if (tst1) { cout << "t2c, "; }
                 d.featureCount++;
              }
             // nextFeatureInRange not in range.
               else // if ((d.nextFeatureInRange()) == false)
              {
                 if (tst1) { cout << "t2b, "; }
                 d.featureCount = 1;
                 state = eState::freeFeature;
              }
            }  // ( state == eState::obstacleCandidate)

           else if (state == eState::obstacleOpen)
           {
              if (tst1) { cout << "obstacleOpen " << endl; }

              // Last feature

              // Todo - test flow t3t
              // Last feature...
              if  (d.nextFeatureValid() == false)
              {
                 if (tst1) { cout << "t3t, "; }
                 d.rEdge_it = d.f_it;
                 d.isAnObstacle = false;
                 state = eState::terminate;
                 d.outputObstacleRecord();
             }
              else if (d.nextFeatureInRange())
              {
                 if (tst1) { cout << "t3c, "; }
                 d.featureCount++;
              }
              else // if ((d.nextFeatureInRange() == false)
              {
                 if (tst1) { cout << "t3b, "; }
                //Record Right Edge
                 d.rEdge_it = d.f_it;

                 d.featureCount = 1;
                 d.isAnObstacle = false;
                 state = eState::freeFeature;
                 d.outputObstacleRecord();
              }
           }  // state == eState::obstacleOpen)
           else if (state == eState::terminate)
           {
              if (tst1) { cout << "terminate " << endl; }
           }
           else 
           {
              cout << endl << "ERROR - Unauthorized end of state machine." << endl;
           }

        } // End Obstacle Detector State Machine
        if (tst1) { cout << "State machine exit. " << endl; }
  return 0;
}
