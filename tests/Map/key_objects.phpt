--TEST--
Map with Ds\Key objects - comprehensive custom key equality
--FILE--
<?php
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
    public function getId(): int { return $this->id; }
}

// Basic put and get
$m = new Ds\Map();
$m->put(new IntKey(1), "one");
$m->put(new IntKey(2), "two");
$m->put(new IntKey(3), "three");

echo "count: " . $m->count() . "\n";
echo "get 1: " . $m->get(new IntKey(1)) . "\n";
echo "get 2: " . $m->get(new IntKey(2)) . "\n";
echo "get 3: " . $m->get(new IntKey(3)) . "\n";

// Overwrite with equal key
$m->put(new IntKey(1), "ONE");
echo "overwritten: " . $m->get(new IntKey(1)) . "\n";
echo "still 3: " . $m->count() . "\n";

// hasKey with Key objects
echo "has 1: " . var_export($m->hasKey(new IntKey(1)), true) . "\n";
echo "has 99: " . var_export($m->hasKey(new IntKey(99)), true) . "\n";

// remove with Key objects
$val = $m->remove(new IntKey(2));
echo "removed: $val\n";
echo "count: " . $m->count() . "\n";
echo "has 2: " . var_export($m->hasKey(new IntKey(2)), true) . "\n";

// remove missing Key
try {
    $m->remove(new IntKey(99));
    echo "ERROR: should have thrown\n";
} catch (OutOfBoundsException $e) {
    echo "remove missing: " . $e->getMessage() . "\n";
}

// remove with default
$val2 = $m->remove(new IntKey(99), "default");
echo "remove default: $val2\n";

// Hash collisions: same hash, different equals
class CollidingKey implements Ds\Key {
    public function __construct(private string $name) {}
    public function hash(): mixed { return 42; } // all same hash
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->name === $other->name;
    }
    public function getName(): string { return $this->name; }
}

$m2 = new Ds\Map();
$m2->put(new CollidingKey("alpha"), 1);
$m2->put(new CollidingKey("beta"), 2);
$m2->put(new CollidingKey("gamma"), 3);

echo "collision count: " . $m2->count() . "\n";
echo "alpha: " . $m2->get(new CollidingKey("alpha")) . "\n";
echo "beta: " . $m2->get(new CollidingKey("beta")) . "\n";
echo "gamma: " . $m2->get(new CollidingKey("gamma")) . "\n";

// Overwrite with colliding hash
$m2->put(new CollidingKey("alpha"), 100);
echo "collision overwrite: " . $m2->get(new CollidingKey("alpha")) . "\n";
echo "still 3: " . $m2->count() . "\n";

// hasKey with colliding keys
echo "has alpha: " . var_export($m2->hasKey(new CollidingKey("alpha")), true) . "\n";
echo "has delta: " . var_export($m2->hasKey(new CollidingKey("delta")), true) . "\n";

// Remove with colliding keys
$m2->remove(new CollidingKey("beta"));
echo "after remove beta: " . $m2->count() . "\n";
echo "has alpha: " . var_export($m2->hasKey(new CollidingKey("alpha")), true) . "\n";
echo "has gamma: " . var_export($m2->hasKey(new CollidingKey("gamma")), true) . "\n";

// Different Key classes are different keys even with same hash
class OtherKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$m3 = new Ds\Map();
$m3->put(new IntKey(1), "IntKey 1");
$m3->put(new OtherKey(1), "OtherKey 1"); // same hash(1), but different class
echo "different classes: " . $m3->count() . "\n";
echo "IntKey 1: " . $m3->get(new IntKey(1)) . "\n";
echo "OtherKey 1: " . $m3->get(new OtherKey(1)) . "\n";

// Key objects in iteration
$m4 = new Ds\Map();
$m4->put(new IntKey(10), "ten");
$m4->put(new IntKey(20), "twenty");
$result = [];
foreach ($m4 as $key => $value) {
    $result[] = $key->getId() . "=" . $value;
}
echo "iteration: " . implode(",", $result) . "\n";

// first/last with Key objects
$first = $m4->first();
echo "first key id: " . $first->key->getId() . "\n";
echo "first value: " . $first->value . "\n";

$last = $m4->last();
echo "last key id: " . $last->key->getId() . "\n";
echo "last value: " . $last->value . "\n";

// skip with Key objects
$pair = $m4->skip(1);
echo "skip 1 key id: " . $pair->key->getId() . "\n";

// keys() returns Set of Key objects
$keys = $m4->keys();
echo "keys count: " . $keys->count() . "\n";
echo "contains IntKey(10): " . var_export($keys->contains(new IntKey(10)), true) . "\n";
echo "contains IntKey(30): " . var_export($keys->contains(new IntKey(30)), true) . "\n";
?>
--EXPECT--
count: 3
get 1: one
get 2: two
get 3: three
overwritten: ONE
still 3: 3
has 1: true
has 99: false
removed: two
count: 2
has 2: false
remove missing: Key not found
remove default: default
collision count: 3
alpha: 1
beta: 2
gamma: 3
collision overwrite: 100
still 3: 3
has alpha: true
has delta: false
after remove beta: 2
has alpha: true
has gamma: true
different classes: 2
IntKey 1: IntKey 1
OtherKey 1: OtherKey 1
iteration: 10=ten,20=twenty
first key id: 10
first value: ten
last key id: 20
last value: twenty
skip 1 key id: 20
keys count: 2
contains IntKey(10): true
contains IntKey(30): false
