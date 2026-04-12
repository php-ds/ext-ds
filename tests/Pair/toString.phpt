--TEST--
Pair::__toString - returns string representation
--FILE--
<?php
$p = new Ds\Pair("hello", 42);
echo (string) $p . "\n";

// With null values
$p2 = new Ds\Pair(null, null);
echo (string) $p2 . "\n";

// Using in string context
$p3 = new Ds\Pair("key", "value");
echo "result: $p3\n";

// Different types produce same string
$p4 = new Ds\Pair(1, [1, 2]);
echo (string) $p4 . "\n";
?>
--EXPECT--
object(Ds\Pair)
object(Ds\Pair)
result: object(Ds\Pair)
object(Ds\Pair)
