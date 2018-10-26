--TEST--
Decimal::sum
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

    [[[1, 2, 3],   ], "6", 28],
    [[[1, 2, 3], 10], "6", 10],

    [[array(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)),  ], "0.6", 28],
    [[array(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)), 4], "0.6",  6],
    [[array(decimal("0.1", 4), decimal("0.2", 5), "0.3"),              ], "0.6", 28],

    [[iterator(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)),  ], "0.6", 28],
    [[iterator(decimal("0.1", 4), decimal("0.2", 5), decimal("0.3", 6)), 4], "0.6",  6],
    [[iterator(decimal("0.1", 4), decimal("0.2", 5), "0.3"),              ], "0.6", 28],

    [[array('-2.3', '4.1'),   ], "1.8", 28],
    [[array('-2.3', '4.1'), 10], "1.8", 10],
    [[array('-2.3', '4.1'), 30], "1.8", 30],
];

foreach ($tests as $index => $test) {
    list($args, $expect, $precision) = $test;

    $result = Decimal::sum(...$args);

    if ([(string) $result, $result->precision()] !== [$expect, $precision]) {
        print_r(compact("index", "args", "result", "expect", "precision"));
    }
}

/* Test non-traversable */
try {
    Decimal::sum("abc");
} catch (TypeError $e) {
    printf("%s\n", $e->getMessage());
}

/* Test non-integer precision */
try {
    Decimal::sum([], 'a');
} catch (TypeError $e) {
    printf("%s\n", $e->getMessage());
}

/* Test bad types */
try {
    Decimal::sum([1, 2, "abc"]);
} catch (DomainException $e) {
    printf("%s\n", $e->getMessage());
}
?>
--EXPECTF--
Decimal\Decimal::sum() expected parameter 1 to be an array or traversable object, string given
Argument 2 passed to Decimal\Decimal::sum() must be of the type integer%s, string given
Failed to parse string as decimal: "abc"
