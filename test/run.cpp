extern "C" {
#include "run.h"
    
};

#define CATCH_CONFIG_RUNNER
#include "../buildSrcTest/src/catch.hpp"
bool run_tests() {
    const char *argv[] = { "hw01" };
    return (Catch::Session().run(1, argv) == 0);
}
