#include <thread>
#include "lazyTypes.h"
#include "lazyInstMgr.h"

/// It copies the keys of the _refs judy structure provided to it into the vector realRefs
void prepareRealRefs ( instanceRefs_t * _refs, instanceRefs &realRefs ) {

    realRefs.clear();

    instanceID last = _refs->end().key;
    instanceID current = _refs->begin().key;

    while( current != last ) {                
        realRefs.push_back( current );
        current = _refs->next().key;
    }

    realRefs.push_back ( last );
}

/// Used by an individual thread to iterate over the keys of the _fwdRefs / _revRefs multiple times. For each iteration it expects the order of the keys to be the same as dictated by realRefs.
void iterateOverRefs ( instanceRefs_t * _refs, instanceRefs &realRefs, bool * success ) {
    const int iterations = 10;
    int i, k, instances = realRefs.size();
    instanceID current;
 
    for( k = 0; k < iterations; k++ ) {
        current = _refs->begin().key;
        for( i = 0; i < instances; i++ ) {

            if( current != realRefs[i] ) {
                break;
            }

            current = _refs->next().key;
        }

        *success = *success && ( i == instances );
    }
}

/// Checks the thread safety of _fwdRefs (_revRefs) when the forward value provided to it is true (false).
void checkRefsSafety( char * fileName, bool forward ) {
    instanceRefs_t * _refs;
    instanceRefs realRefs; 
    lazyInstMgr * mgr = new lazyInstMgr;
    mgr->openFile( fileName );

    if( forward ) {
        std::cout << "Checking thread safety while iterating over forward references..." ;
        _refs = mgr->getFwdRefs();
    } else {
        std::cout << "Checking thread safety while iterating over backward references..." ;
        _refs = mgr->getRevRefs();
    }

    prepareRealRefs( _refs, realRefs );
    bool success[2] = { true, true };

    std::thread first( iterateOverRefs, _refs, realRefs, &success[0] );
    std::thread second( iterateOverRefs, _refs, realRefs, &success[1] );

    first.join();
    second.join();

    if( success[0] && success[1] ) {
        std::cout << "..PASS!" << std::endl;
    } else {
        std::cout << "...FAIL!" << std::endl;

        if( !success[0] ) {
            std::cout << "\tThread 0 could not iterate properly" << std::endl;
        }

        if( !success[1] ) {
            std::cout << "\tThread 1 could not iterate properly" << std::endl;
        }
    }
    
    std::cout << std::endl;
    delete mgr;
}

/// Checks thread safety of getFwdRefs();
void checkFwdRefsSafety( char * fileName ) {
    checkRefsSafety( fileName, true );
}

/// Checks thread safety of getFwdRefs();
void checkRevRefsSafety( char * fileName ) {
    checkRefsSafety( fileName, false );
}

int main( int argc, char ** argv ) {
    if( argc != 2 ) {
        std::cerr << "Expected one argument, given " << argc - 1 << ". Exiting." << std::endl;
        exit( EXIT_FAILURE );
    }

    checkFwdRefsSafety( argv[1] );
    checkRevRefsSafety( argv[1] );
}
