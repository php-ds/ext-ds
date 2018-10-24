--TEST--
Decimal::avg
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * Shortcut to construct a new decimal.
 */
function decimal(...$args) { return new Decimal(...$args); }

/**
 * Proxy iterator.
 */
function iterator(...$values) { yield from $values; }

/**
 * (values [, prec]), Expected Result, Expected Precision
 */
$tests = [
    [[[],   ], "0", 28],
    [[[], 30], "0", 30],

    [[[1, 2, 3],   ], "2", 28],
    [[[1, 2, 3], 10], "2", 28],

    [[array(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)),  ], "0.2", 28],
    [[array(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)), 4], "0.2",  6],
    [[array(decimal("0.1", 4), decimal("0.2", 5), "0.3"),              ], "0.2", 28],

    [[iterator(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)),  ], "0.2", 28],
    [[iterator(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)), 4], "0.2",  6],
    [[iterator(decimal("0.1", 4), decimal("0.2", 5), "0.3"),              ], "0.2", 28],

    [[array('-2.3', '4.1', 5),   ], "2.266666666666666666666666667",   28],
    [[array('-2.3', '4.1', 5), 10], "2.266666666666666666666666667",   28],
    [[array('-2.3', '4.1', 5), 30], "2.26666666666666666666666666667", 30],
];

foreach ($tests as $index => $test) {
    list($args, $expect, $precision) = $test;

    $result = Decimal::avg(...$args);

    if ([(string) $result, $result->precision()] !== [$expect, $precision]) {
        print_r(compact("index", "args", "result", "expect", "precision"));
    }
}

/* Test non-traversable */
try {
    Decimal::avg("abc");
} catch (TypeError $e) {
    printf("%s\n", $e->getMessage());
}

/* Test bad types */
try {
    Decimal::avg([1, 2, "abc"]);
} catch (DomainException $e) {
    printf("%s\n", $e->getMessage());
}
?>
--EXPECT--
Decimal\Decimal::avg() expected parameter 1 to be an array or traversable object, string given
Failed to parse string as decimal: "abc"
