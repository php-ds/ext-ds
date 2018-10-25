--TEST--
Decimal::equals
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
    [decimal(),  0,      true],
    [decimal(),  null,   false],
    [decimal(),  false,  false],
    [decimal(),  0.0,    true],
    [decimal(),  "",     false],

    [decimal(1), 0,      false],
    [decimal(1), null,   false],
    [decimal(1), false,  false],
    [decimal(1), 0.0,    false],
    [decimal(1), "",     false],

    [decimal(),  1,      false],
    [decimal(),  true,   true],
    [decimal(),  1.0,    false],
    [decimal(),  "1",    false],

    [decimal(1), 1,      true],
    [decimal(1), true,   true],
    [decimal(1), 1.0,    true],
    [decimal(1), "1",    true],

    [decimal("0.1"),        0.1,    true],
    [decimal("0.2"),        0.2,    true],
    [decimal("0.200"),      0.2,    true],
    [decimal("-0.1"),      -0.1,    true],
    [decimal("-0.2"),      -0.2,    true],
    [decimal("1E-10"),      1E-10,  true],
    [decimal("1E+10"),      1E+10,  true],
    [decimal("-1E-10"),    -1E-10,  true],
    [decimal("-1E+10"),    -1E+10,  true],

    [decimal("0.1000000000000000000001"), 0.1,  false],
    [decimal("0.2000000000000000000001"), 0.2,  false],

    [decimal("0.0000000000000000000001"), 0.1,  false],
    [decimal("0.0000000000000000000001"), 0.0,  false],

    [decimal("0.1"),        "0.01",             false],
    [decimal("0.1"),        "0.1",              true],
    [decimal("0.1"),        "0.2",              false],
    [decimal("0.2"),        "0.1",              false],
    [decimal("0.01"),       "0.1",              false],

    [decimal(),             decimal(),          true],
    [decimal(),             decimal(0),         true],
    [decimal("0.1"),        decimal(0),         false],
    [decimal("0.1"),        decimal("0.01"),    false],
    [decimal("0.1"),        decimal("0.1"),     true],
    [decimal("0.1"),        decimal("0.2"),     false],
    [decimal("0.2"),        decimal("0.1"),     false],
    [decimal("0.01"),       decimal("0.1"),     false],

    /* Equal value, precision considered. */
    [decimal(0, 10),        decimal(0),         false],
    [decimal(0, 10),        decimal(0, 9),      false],
    [decimal(0, 10),        decimal(0, 10),     true],

    [decimal('1', 10),      decimal('1'),       false],
    [decimal('1', 10),      decimal('1', 9),    false],
    [decimal('1', 10),      decimal('1', 10),   true],

    [decimal(NAN, 10),      decimal(NAN),       false],
    [decimal(NAN, 10),      decimal(NAN, 9),    false],
    [decimal(NAN, 10),      decimal(NAN, 10),   false],

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

    [decimal( "INF"),       NAN,                INF ==  NAN],
    [decimal( "INF"),       INF,                INF ==  INF],
    [decimal( "INF"),      -INF,                INF == -INF],
    [decimal("-INF"),       NAN,               -INF ==  NAN],
    [decimal("-INF"),       INF,               -INF ==  INF],
    [decimal("-INF"),      -INF,               -INF == -INF],
    [decimal( "NAN"),       NAN,                NAN ==  NAN],
    [decimal( "NAN"),       INF,                NAN ==  INF],
    [decimal( "NAN"),      -INF,                NAN == -INF],
];

foreach ($tests as $index => $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1 == $op2,
        $op2 == $op1,
      !($op1 != $op2),
      !($op2 != $op1),
    ];

    if ($op1 instanceof Decimal) {
        $results[] = $op1->equals($op2);
    }

    if ($op2 instanceof Decimal) {
        $results[] = $op2->equals($op1);
    }

    foreach ($results as $result) {
        if ($result !== $expect) {
            var_dump(compact("op1", "op2", "result", "expect"));
            break;
        }
    }
}

?>
--EXPECT--
