<?php

require __DIR__ . '/vendor/autoload.php';

// Check that the extension is loaded.
if ( ! extension_loaded('ds')) {
    echo "ds extension is not enabled\n";
    exit(1);
}

// Specify the test configuration file
$_SERVER['argv'][] = '--configuration';
$_SERVER['argv'][] = __DIR__ . '/vendor/php-ds/tests/phpunit.xml';

// 'false' so that PHPUnit doesn't use "exit"
PHPUnit_TextUI_Command::main(false);

// Attempt to collect anything left over from the tests.
gc_collect_cycles();
