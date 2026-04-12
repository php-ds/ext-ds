--TEST--
Pair::clone - clone creates independent copy
--FILE--
<?php
// Basic clone
$p = new Ds\Pair("hello", 42);
$c = clone $p;
echo "clone key: " . $c->key . "\n";
echo "clone value: " . $c->value . "\n";

// Clone with null values
$p2 = new Ds\Pair(null, null);
$c2 = clone $p2;
echo "null clone key: " . var_export($c2->key, true) . "\n";
echo "null clone value: " . var_export($c2->value, true) . "\n";

// Clone with array value - array is copied
$arr = [1, 2, 3];
$p3 = new Ds\Pair("data", $arr);
$c3 = clone $p3;
echo "array clone: " . implode(",", $c3->value) . "\n";

// Clone with object value - object is shared (shallow clone)
$obj = new stdClass();
$obj->x = 10;
$p4 = new Ds\Pair("obj", $obj);
$c4 = clone $p4;
$obj->x = 20; // modify original object
echo "object shared: " . $c4->value->x . "\n";

// Clone equality
$p5 = new Ds\Pair("a", "b");
$c5 = clone $p5;
echo "same key: " . var_export($p5->key === $c5->key, true) . "\n";
echo "same value: " . var_export($p5->value === $c5->value, true) . "\n";

// Clone of clone
$p6 = new Ds\Pair("deep", 999);
$c6 = clone $p6;
$c7 = clone $c6;
echo "clone of clone: key=" . $c7->key . " value=" . $c7->value . "\n";
?>
--EXPECT--
clone key: hello
clone value: 42
null clone key: NULL
null clone value: NULL
array clone: 1,2,3
object shared: 20
same key: true
same value: true
clone of clone: key=deep value=999
