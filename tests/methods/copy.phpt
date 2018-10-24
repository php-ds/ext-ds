--TEST--
Decimal::copy
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo "skip";
?>
--FILE--
<?php
$src = new Decimal\Decimal("123", 32);
$dst = $src->copy();

var_dump($src, $dst);

?>
--EXPECT--
object(Decimal\Decimal)#1 (2) {
  ["value"]=>
  string(3) "123"
  ["precision"]=>
  int(32)
}
object(Decimal\Decimal)#2 (2) {
  ["value"]=>
  string(3) "123"
  ["precision"]=>
  int(32)
}
