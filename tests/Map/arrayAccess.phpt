--TEST--
Map ArrayAccess - bracket syntax for get, set, isset, unset
--FILE--
<?php
$m = new Ds\Map();

// offsetSet: $m[$key] = $val
$m["a"] = 1;
$m["b"] = 2;
$m["c"] = 3;
echo "count: " . $m->count() . "\n";

// offsetGet: $m[$key]
echo "a: " . $m["a"] . "\n";
echo "b: " . $m["b"] . "\n";
echo "c: " . $m["c"] . "\n";

// offsetExists: isset($m[$key])
echo "isset a: " . var_export(isset($m["a"]), true) . "\n";
echo "isset z: " . var_export(isset($m["z"]), true) . "\n";

// isset returns false for null values (PHP behavior)
$m["nullval"] = null;
echo "isset nullval: " . var_export(isset($m["nullval"]), true) . "\n";
echo "has nullval: " . var_export($m->hasKey("nullval"), true) . "\n";

// offsetUnset: unset($m[$key])
unset($m["a"]);
echo "after unset a: " . $m->count() . "\n";
echo "has a: " . var_export($m->hasKey("a"), true) . "\n";

// Overwrite via bracket syntax
$m["b"] = 200;
echo "overwritten b: " . $m["b"] . "\n";
echo "count same: " . $m->count() . "\n";

// Integer key with bracket syntax
$m[42] = "answer";
echo "int key: " . $m[42] . "\n";

// Get missing key throws OutOfBoundsException
try {
    $val = $m["nonexistent"];
    echo "ERROR: should have thrown\n";
} catch (OutOfBoundsException $e) {
    echo "missing: " . $e->getMessage() . "\n";
}

// Unset missing key is silent (returns void)
unset($m["nonexistent"]);
echo "unset missing: ok\n";

// Boolean key
$m[true] = "yes";
echo "bool key: " . $m[true] . "\n";
echo "isset bool: " . var_export(isset($m[true]), true) . "\n";
?>
--EXPECT--
count: 3
a: 1
b: 2
c: 3
isset a: true
isset z: false
isset nullval: false
has nullval: true
after unset a: 3
has a: false
overwritten b: 200
count same: 3
int key: answer
missing: Key not found
unset missing: ok
bool key: yes
isset bool: true
