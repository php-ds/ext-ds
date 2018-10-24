--TEST--
Decimal::equalsExactly
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

$tests = [
    [decimal(),             decimal(),          true],
    [decimal(),             decimal(0),         true],
    [decimal("0.1"),        decimal(0),         false],
    [decimal("0.1"),        decimal("0.01"),    false],
    [decimal("0.1"),        decimal("0.1"),     true],
    [decimal("0.1"),        decimal("0.2"),     false],
    [decimal("0.2"),        decimal("0.1"),     false],
    [decimal("0.01"),       decimal("0.1"),     false],

    /* Equal value, precision must also be equal. */
    [decimal(0, 10),        decimal(0),         false],
    [decimal(0, 10),        decimal(0, 9),      false],
    [decimal(0, 10),        decimal(0, 10),     true],

    [decimal('1', 10),      decimal('1'),       false],
    [decimal('1', 10),      decimal('1', 9),    false],
    [decimal('1', 10),      decimal('1', 10),   true],

    /* Match PHP on special numbers. */
    [decimal( "INF"),       decimal( "NAN"),    INF ==  NAN],
    [decimal( "INF"),       decimal( "INF"),    INF ==  INF],
    [decimal( "INF"),       decimal("-INF"),    INF == -INF],
    [decimal("-INF"),       decimal( "NAN"),   -INF ==  NAN],
    [decimal("-INF"),       decimal( "INF"),   -INF ==  INF],
    [decimal("-INF"),       decimal("-INF"),   -INF == -INF],
    [decimal( "NAN"),       decimal( "NAN"),    NAN ==  NAN],
    [decimal( "NAN"),       decimal( "INF"),    NAN ==  INF],
    [decimal( "NAN"),       decimal("-INF"),    NAN == -INF],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->equalsExactly($op2),
        $op2->equalsExactly($op1),
    ];

    foreach ($results as $result) {
        if ($result !== $expect) {
            var_dump(compact("op1", "op2", "result", "expect"));
            break;
        }
    }
}

/* Test that op must be decimal */
decimal()->equalsExactly(1);

?>
--EXPECTF--
Fatal error: Uncaught TypeError: Argument 1 passed to Decimal\Decimal::equalsExactly() must be an instance of Decimal\Decimal, integer given in %s:%d
Stack trace:
#0 %s(%d): Decimal\Decimal->equalsExactly(1)
#1 {main}
  thrown in %s on line %d
