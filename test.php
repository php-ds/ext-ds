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

// Attempt to collect anything left over from the tests.
register_shutdown_function('gc_collect_cycles');

$application = new \PHPUnit\TextUI\Application();
$application->run($_SERVER['argv'] ?? ['phpunit']);
