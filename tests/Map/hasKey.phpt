--TEST--
Map::hasKey - check if key exists
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);

echo "has a: " . var_export($m->hasKey("a"), true) . "\n";
echo "has z: " . var_export($m->hasKey("z"), true) . "\n";

// Integer keys
$m->put(0, "zero");
echo "has 0: " . var_export($m->hasKey(0), true) . "\n";
echo "has 99: " . var_export($m->hasKey(99), true) . "\n";

// Strict type checking - "0" is not 0
echo "has '0' (string): " . var_export($m->hasKey("0"), true) . "\n";

// Null key
$m->put(null, "null");
echo "has null: " . var_export($m->hasKey(null), true) . "\n";

// Boolean keys
$m->put(true, "true");
$m->put(false, "false");
echo "has true: " . var_export($m->hasKey(true), true) . "\n";
echo "has false: " . var_export($m->hasKey(false), true) . "\n";

// Float key
$m->put(3.14, "pi");
echo "has 3.14: " . var_export($m->hasKey(3.14), true) . "\n";
echo "has 3.15: " . var_export($m->hasKey(3.15), true) . "\n";

// Empty map
$empty = new Ds\Map();
echo "empty has a: " . var_export($empty->hasKey("a"), true) . "\n";

// Key objects with Ds\Key
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$m2 = new Ds\Map();
$m2->put(new IntKey(1), "one");
$m2->put(new IntKey(2), "two");
echo "has IntKey(1): " . var_export($m2->hasKey(new IntKey(1)), true) . "\n";
echo "has IntKey(3): " . var_export($m2->hasKey(new IntKey(3)), true) . "\n";
?>
--EXPECT--
has a: true
has z: false
has 0: true
has 99: false
has '0' (string): false
has null: true
has true: true
has false: true
has 3.14: true
has 3.15: false
empty has a: false
has IntKey(1): true
has IntKey(3): false
