--TEST--
Decimal object properties
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
use Decimal\Decimal;

$decimal = new Decimal();

$decimal->test = 5;

var_dump($decimal);
var_dump($decimal->test);

?>
--EXPECT--
object(Decimal\Decimal)#1 (2) {
  ["value"]=>
  string(1) "0"
  ["precision"]=>
  int(28)
}
int(5)
