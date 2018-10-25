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
unset($decimal->test);
isset($decimal->test);
var_dump($decimal->test);

try {
    $decimal[0] = 5;
} catch (Error $e) {
    printf("%s\n", $e->getMessage());
}

echo "Done";

?>
--EXPECTF--
Notice: Object properties are not supported in %s on line 6

Notice: Object properties are not supported in %s on line 7

Notice: Object properties are not supported in %s on line 8

Notice: Object properties are not supported in %s on line 9
NULL
Cannot use object of type Decimal\Decimal as array
Done
