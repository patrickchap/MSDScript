#import <XCTest/XCTest.h>
#include "run.h"
//#include "Env.hpp"

@interface test : XCTestCase

@end

@implementation test

- (void)testAll {
    if (!run_tests())
        XCTFail(@"failed");
}

@end
