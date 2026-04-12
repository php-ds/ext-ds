--TEST--
Map::get - retrieve values by key with optional default
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

// Basic get
echo "a: " . $m->get("a") . "\n";
echo "b: " . $m->get("b") . "\n";
echo "c: " . $m->get("c") . "\n";

// Get with default - key exists
echo "existing with default: " . $m->get("a", 999) . "\n";

// Get with default - key missing
echo "missing with default: " . $m->get("z", 999) . "\n";

// Get missing key without default throws OutOfBoundsException
try {
    $m->get("missing");
    echo "ERROR: should have thrown\n";
} catch (OutOfBoundsException $e) {
    echo "missing: " . $e->getMessage() . "\n";
}

// Get with null default
echo "null default: " . var_export($m->get("missing", null), true) . "\n";

// Get with false default
echo "false default: " . var_export($m->get("missing", false), true) . "\n";

// Get with integer keys
$m2 = new Ds\Map([0 => "zero", 1 => "one"]);
echo "int key: " . $m2->get(0) . "\n";
echo "int key: " . $m2->get(1) . "\n";

// Get with null key
$m3 = new Ds\Map();
$m3->put(null, "null value");
echo "null key: " . $m3->get(null) . "\n";

// Get with boolean key
$m4 = new Ds\Map();
$m4->put(true, "true val");
$m4->put(false, "false val");
echo "true key: " . $m4->get(true) . "\n";
echo "false key: " . $m4->get(false) . "\n";

// Get with float key
$m5 = new Ds\Map();
$m5->put(1.5, "float val");
echo "float key: " . $m5->get(1.5) . "\n";
?>
--EXPECT--
a: 1
b: 2
c: 3
existing with default: 1
missing with default: 999
missing: Key not found
null default: NULL
false default: false
int key: zero
int key: one
null key: null value
true key: true val
false key: false val
float key: float val
