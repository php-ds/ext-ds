--TEST--
Key: hash collisions - same hash but different equals coexist
--FILE--
<?php
class CollisionKey implements Ds\Key {
    public function __construct(private string $name) {}
    public function hash(): mixed { return 1; } // all same hash
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->name === $other->name;
    }
    public function getName(): string { return $this->name; }
}

// Map: multiple keys with same hash coexist
$m = new Ds\Map();
$m->put(new CollisionKey("alpha"), 1);
$m->put(new CollisionKey("beta"), 2);
$m->put(new CollisionKey("gamma"), 3);

echo "map count: " . $m->count() . "\n";
echo "alpha: " . $m->get(new CollisionKey("alpha")) . "\n";
echo "beta: " . $m->get(new CollisionKey("beta")) . "\n";
echo "gamma: " . $m->get(new CollisionKey("gamma")) . "\n";

// Overwrite with same hash and same equals
$m->put(new CollisionKey("alpha"), 100);
echo "overwritten alpha: " . $m->get(new CollisionKey("alpha")) . "\n";
echo "count after overwrite: " . $m->count() . "\n";

// hasKey with collisions
echo "hasKey alpha: " . var_export($m->hasKey(new CollisionKey("alpha")), true) . "\n";
echo "hasKey delta: " . var_export($m->hasKey(new CollisionKey("delta")), true) . "\n";

// Remove with collisions
$m->remove(new CollisionKey("beta"));
echo "after remove beta: " . $m->count() . "\n";
echo "alpha still: " . $m->get(new CollisionKey("alpha")) . "\n";
echo "gamma still: " . $m->get(new CollisionKey("gamma")) . "\n";

// Set: collision keys coexist
$s = new Ds\Set();
$s->add(new CollisionKey("x"));
$s->add(new CollisionKey("y"));
$s->add(new CollisionKey("z"));
echo "set count: " . $s->count() . "\n";

// Deduplication with collision
$s->add(new CollisionKey("x"));
echo "after dup: " . $s->count() . "\n";

// Contains with collisions
echo "contains x: " . var_export($s->contains(new CollisionKey("x")), true) . "\n";
echo "contains y: " . var_export($s->contains(new CollisionKey("y")), true) . "\n";
echo "contains w: " . var_export($s->contains(new CollisionKey("w")), true) . "\n";

// Remove from set with collisions
$s->remove(new CollisionKey("y"));
echo "after remove y: " . $s->count() . "\n";
echo "still has x: " . var_export($s->contains(new CollisionKey("x")), true) . "\n";
echo "still has z: " . var_export($s->contains(new CollisionKey("z")), true) . "\n";
echo "no y: " . var_export($s->contains(new CollisionKey("y")), true) . "\n";

// Many collisions
$m2 = new Ds\Map();
for ($i = 0; $i < 20; $i++) {
    $m2->put(new CollisionKey("key$i"), $i);
}
echo "20 collisions count: " . $m2->count() . "\n";
echo "key0: " . $m2->get(new CollisionKey("key0")) . "\n";
echo "key19: " . $m2->get(new CollisionKey("key19")) . "\n";

// Verify all values are accessible
$allOk = true;
for ($i = 0; $i < 20; $i++) {
    if ($m2->get(new CollisionKey("key$i")) !== $i) {
        $allOk = false;
        break;
    }
}
echo "all 20 accessible: " . var_export($allOk, true) . "\n";
?>
--EXPECT--
map count: 3
alpha: 1
beta: 2
gamma: 3
overwritten alpha: 100
count after overwrite: 3
hasKey alpha: true
hasKey delta: false
after remove beta: 2
alpha still: 100
gamma still: 3
set count: 3
after dup: 3
contains x: true
contains y: true
contains w: false
after remove y: 2
still has x: true
still has z: true
no y: false
20 collisions count: 20
key0: 0
key19: 19
all 20 accessible: true
