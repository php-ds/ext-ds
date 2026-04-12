--TEST--
Map::put - add or update key-value pairs
--FILE--
<?php
$m = new Ds\Map();

// Basic put
$m->put("a", 1);
echo "after put: " . $m->get("a") . "\n";
echo "count: " . $m->count() . "\n";

// Overwrite existing key
$m->put("a", 100);
echo "after overwrite: " . $m->get("a") . "\n";
echo "count still: " . $m->count() . "\n";

// Multiple puts
$m->put("b", 2);
$m->put("c", 3);
echo "count: " . $m->count() . "\n";

// Integer key
$m->put(42, "answer");
echo "int key: " . $m->get(42) . "\n";

// Null key
$m->put(null, "null value");
echo "null key: " . $m->get(null) . "\n";

// Boolean keys
$m->put(true, "true val");
$m->put(false, "false val");
echo "true key: " . $m->get(true) . "\n";
echo "false key: " . $m->get(false) . "\n";

// Float key
$m->put(3.14, "pi");
echo "float key: " . $m->get(3.14) . "\n";

// Null value
$m2 = new Ds\Map();
$m2->put("x", null);
echo "null value: " . var_export($m2->get("x"), true) . "\n";

// Array value
$m2->put("arr", [1, 2, 3]);
echo "array value: " . implode(",", $m2->get("arr")) . "\n";

// Object value
$obj = new stdClass();
$obj->name = "test";
$m2->put("obj", $obj);
echo "obj value: " . $m2->get("obj")->name . "\n";

// Strict type distinction for keys
$m3 = new Ds\Map();
$m3->put(0, "int zero");
$m3->put("0", "string zero");
$m3->put(false, "bool false");
$m3->put(null, "null");
echo "distinct keys: " . $m3->count() . "\n";

// Key objects with Ds\Key
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$m4 = new Ds\Map();
$m4->put(new IntKey(1), "one");
$m4->put(new IntKey(2), "two");
echo "key objects: " . $m4->count() . "\n";
echo "key 1: " . $m4->get(new IntKey(1)) . "\n";

// Overwrite with Key object
$m4->put(new IntKey(1), "ONE");
echo "overwritten: " . $m4->get(new IntKey(1)) . "\n";
echo "still 2: " . $m4->count() . "\n";
?>
--EXPECT--
after put: 1
count: 1
after overwrite: 100
count still: 1
count: 3
int key: answer
null key: null value
true key: true val
false key: false val
float key: pi
null value: NULL
array value: 1,2,3
obj value: test
distinct keys: 4
key objects: 2
key 1: one
overwritten: ONE
still 2: 2
