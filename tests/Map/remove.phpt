--TEST--
Map::remove - remove a key and return its value
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

// Basic remove
$val = $m->remove("a");
echo "removed: $val\n";
echo "count: " . $m->count() . "\n";
echo "has a: " . var_export($m->hasKey("a"), true) . "\n";

// Remove with default - key exists
$val2 = $m->remove("b", 999);
echo "existing with default: $val2\n";

// Remove with default - key missing
$val3 = $m->remove("missing", 999);
echo "missing with default: $val3\n";

// Remove missing key without default throws OutOfBoundsException
try {
    $m->remove("nonexistent");
    echo "ERROR: should have thrown\n";
} catch (OutOfBoundsException $e) {
    echo "missing: " . $e->getMessage() . "\n";
}

// Remove with null default
$val4 = $m->remove("also_missing", null);
echo "null default: " . var_export($val4, true) . "\n";

// Remove integer key
$m2 = new Ds\Map([1 => "one", 2 => "two"]);
$val5 = $m2->remove(1);
echo "int key removed: $val5\n";
echo "count: " . $m2->count() . "\n";

// Remove last element
$m3 = new Ds\Map(["only" => 42]);
$val6 = $m3->remove("only");
echo "last removed: $val6\n";
echo "empty: " . var_export($m3->isEmpty(), true) . "\n";

// Remove with Key object
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
$val7 = $m4->remove(new IntKey(1));
echo "key obj removed: $val7\n";
echo "count: " . $m4->count() . "\n";
?>
--EXPECT--
removed: 1
count: 2
has a: false
existing with default: 2
missing with default: 999
missing: Key not found
null default: NULL
int key removed: one
count: 1
last removed: 42
empty: true
key obj removed: one
count: 1
