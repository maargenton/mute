// mute_runner_stdout.h

#pragma once
#include "mute/mute.h"

struct stdout_output_t : mute::output_t{
    virtual void write(const char * b, size_t l) {
        fwrite(b, l, 1, stdout);
    }
};

int main( int argc, char* argv[] ) {
    stdout_output_t out;
    mute::run_all_tests( out );
    return 0;
}
