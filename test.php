<?php

require __DIR__ . '/vendor/autoload.php';

// Check that the extension is loaded.
if ( ! extension_loaded('ds')) {
    echo "ds extension is not enabled\n";
    exit(1);
}

// Check that opcache is enabled correctly.
if (getenv('ENABLE_OPCACHE')) {
    if (!function_exists('opcache_get_status') || ! opcache_get_status(false)['opcache_enabled']) {
        echo "opcache is not enabled\n";
        exit(1);
    }
}

// So that PHPUnit doesn't use "exit"
\PHPUnit\TextUI\Command::main(false);

// Attempt to collect anything left over from the tests.
gc_collect_cycles();
