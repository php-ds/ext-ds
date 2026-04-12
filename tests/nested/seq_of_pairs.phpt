--TEST--
Nested: Seq containing Pair instances
--FILE--
<?php
$s = new Ds\Seq([
    new Ds\Pair("a", 1),
    new Ds\Pair("b", 2),
    new Ds\Pair("c", 3),
]);

echo "count: " . $s->count() . "\n";
foreach ($s as $pair) {
    echo $pair->key . "=" . $pair->value . "\n";
}
?>
--EXPECT--
count: 3
a=1
b=2
c=3
