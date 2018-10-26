--TEST--
Decimal operators
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

/**
 * Arithmetic and comparison operators are tested as part of their corresponding
 * method, for example, "+" will be tested by "add"
 */

/**
 * Test that an assign op does not modify the original the original.
 */
function times2(Decimal $a) {
    return $a *= 2;
}

$x = new Decimal("1E+8", 12);

var_dump(times2($x));
var_dump($x);

var_dump($x & 1); /* Unsupported op */

?>
--EXPECT--
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(4) "2E+8"
  ["precision"]=>
  int(12)
}
object(Decimal\Decimal)#1 (2) {
  ["value"]=>
  string(4) "1E+8"
  ["precision"]=>
  int(12)
}
int(0)
