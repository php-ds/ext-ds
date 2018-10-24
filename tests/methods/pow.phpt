--TEST--
Decimal::pow
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
    /* A ** B = C */

    [decimal(1)     / decimal(3), "2.0", "0.1111111111111111111111111111"],
    [decimal(1)     / decimal(3), "1.5", "0.1924500897298752548363829268"],
    [decimal(1)     / decimal(3), "100", "1.940325217482632837588506009E-48"],
    [decimal(1, 50) / decimal(3), "100", "1.9403252174826328375885060288046503812141166864962E-48"],

    [decimal("0"),   "0",    pow(0, 0)],
    [decimal("0"),   "1",    pow(0, 1)],

    [decimal("1"),  "-2",    pow(1, -2)],
    [decimal("1"),  "-1",    pow(1, -1)],
    [decimal("1"),   "0",    pow(1,  0)],
    [decimal("1"),   "1",    pow(1,  1)],
    [decimal("1"),   "2",    pow(1,  2)],

    [decimal("-1"), "-2",    pow(-1, -2)],
    [decimal("-1"), "-1",    pow(-1, -1)],
    [decimal("-1"),  "0",    pow(-1,  0)],
    [decimal("-1"),  "1",    pow(-1,  1)],
    [decimal("-1"),  "2",    pow(-1,  2)],

    [decimal("0.7"), "1000", "1.253256639965718318107554832E-155"],

    /* Test pow by self */
    [$a = decimal("1.2345678"), $a, "1.297123731240147537684763557"],
];

foreach ($tests as $test) {
    list($op1, $op2, $expect) = $test;

    $results = [
        $op1->pow(decimal($op2)),
        $op1->pow($op2),

        $op1 ** decimal($op2),
        $op1 ** $op2,
    ];

    foreach ($results as $result) {
        if (!$result instanceof Decimal ||  (string) $result !==  (string) $expect) {
            print_r(compact("op1", "op2", "result", "expect"));
            break;
        }
    }
}
?>
--EXPECTF--
