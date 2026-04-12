--TEST--
Set::Key - comprehensive Ds\Key interface tests
--FILE--
<?php
class IntKey implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
    public function __toString(): string { return "IntKey($this->id)"; }
}

// Hash collision class: all objects hash to the same value
class CollisionKey implements Ds\Key {
    public function __construct(private string $name) {}
    public function hash(): mixed { return 42; } // all same hash
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->name === $other->name;
    }
    public function __toString(): string { return "CK($this->name)"; }
}

// --- Basic add and dedup ---
$s = new Ds\Set();
$s->add(new IntKey(1));
$s->add(new IntKey(2));
$s->add(new IntKey(3));
$s->add(new IntKey(1)); // duplicate
$s->add(new IntKey(2)); // duplicate
echo "basic count: " . $s->count() . "\n";

// --- contains ---
echo "contains 1: " . var_export($s->contains(new IntKey(1)), true) . "\n";
echo "contains 2: " . var_export($s->contains(new IntKey(2)), true) . "\n";
echo "contains 4: " . var_export($s->contains(new IntKey(4)), true) . "\n";

// --- contains multiple ---
echo "contains 1,2: " . var_export($s->contains(new IntKey(1), new IntKey(2)), true) . "\n";
echo "contains 1,4: " . var_export($s->contains(new IntKey(1), new IntKey(4)), true) . "\n";

// --- remove ---
$s->remove(new IntKey(2));
echo "after remove 2: " . $s->count() . "\n";
echo "still has 1: " . var_export($s->contains(new IntKey(1)), true) . "\n";
echo "still has 3: " . var_export($s->contains(new IntKey(3)), true) . "\n";
echo "has 2: " . var_export($s->contains(new IntKey(2)), true) . "\n";

// --- Hash collisions ---
$c = new Ds\Set();
$c->add(new CollisionKey("alpha"));
$c->add(new CollisionKey("beta"));
$c->add(new CollisionKey("gamma"));
$c->add(new CollisionKey("alpha")); // duplicate by equals despite same hash
echo "collision count: " . $c->count() . "\n";
echo "collision contains alpha: " . var_export($c->contains(new CollisionKey("alpha")), true) . "\n";
echo "collision contains beta: " . var_export($c->contains(new CollisionKey("beta")), true) . "\n";
echo "collision contains delta: " . var_export($c->contains(new CollisionKey("delta")), true) . "\n";

// Remove from collision chain
$c->remove(new CollisionKey("beta"));
echo "after collision remove: " . $c->count() . "\n";
echo "still has alpha: " . var_export($c->contains(new CollisionKey("alpha")), true) . "\n";
echo "still has gamma: " . var_export($c->contains(new CollisionKey("gamma")), true) . "\n";
echo "has beta: " . var_export($c->contains(new CollisionKey("beta")), true) . "\n";

// --- Key objects in set operations ---
$s1 = new Ds\Set();
$s1->add(new IntKey(1), new IntKey(2), new IntKey(3));

$s2 = new Ds\Set();
$s2->add(new IntKey(2), new IntKey(3), new IntKey(4));

// diff
$diff = $s1->diff($s2);
echo "diff count: " . $diff->count() . "\n";
echo "diff has 1: " . var_export($diff->contains(new IntKey(1)), true) . "\n";

// intersect
$inter = $s1->intersect($s2);
echo "intersect count: " . $inter->count() . "\n";
echo "intersect has 2: " . var_export($inter->contains(new IntKey(2)), true) . "\n";
echo "intersect has 3: " . var_export($inter->contains(new IntKey(3)), true) . "\n";

// union
$union = $s1->union($s2);
echo "union count: " . $union->count() . "\n";

// xor
$xor = $s1->xor($s2);
echo "xor count: " . $xor->count() . "\n";
echo "xor has 1: " . var_export($xor->contains(new IntKey(1)), true) . "\n";
echo "xor has 4: " . var_export($xor->contains(new IntKey(4)), true) . "\n";
echo "xor has 2: " . var_export($xor->contains(new IntKey(2)), true) . "\n";

// --- Key object vs non-Key not equal ---
$mixed = new Ds\Set();
$mixed->add(new IntKey(1));
$mixed->add(1); // integer, not a Key object
echo "key vs int count: " . $mixed->count() . "\n";
?>
--EXPECT--
basic count: 3
contains 1: true
contains 2: true
contains 4: false
contains 1,2: true
contains 1,4: false
after remove 2: 2
still has 1: true
still has 3: true
has 2: false
collision count: 3
collision contains alpha: true
collision contains beta: true
collision contains delta: false
after collision remove: 2
still has alpha: true
still has gamma: true
has beta: false
diff count: 1
diff has 1: true
intersect count: 2
intersect has 2: true
intersect has 3: true
union count: 4
xor count: 2
xor has 1: true
xor has 4: true
xor has 2: false
key vs int count: 2
