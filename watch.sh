#! /bin/bash
FILE_PATTERN="./**/*.h ./**/*.c *.m4"

# Wait for file changes.
function wait_for_changes {
    inotifywait -r -e modify,move,create,delete $FILE_PATTERN
}

function clean {
    make clean
    phpize --clean
}

function init {
    phpize
    ./configure
}

function build {
    clear
    make install -j
}

function test {
    php test.php
}

# Setup and intial run.
clean
init
build

while wait_for_changes; do
    build
    test
done
