--TEST--
Decimal serialize/unserialize
--SKIPIF--
<?php
if (!extension_loaded("decimal")) echo 'skip';
?>
--FILE--
<?php
use Decimal\Decimal;

$decimal = new Decimal("1234.5678E+9", 42);

var_dump(serialize($decimal));

print_r(unserialize(serialize($decimal)));
?>
--EXPECT--
string(54) "C:15:"Decimal\Decimal":26:{s:13:"1.2345678E+12";i:42;}"
Decimal\Decimal Object
(
    [value] => 1.2345678E+12
    [precision] => 42
)
