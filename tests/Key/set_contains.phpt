--TEST--
Key: Set operations - add, contains, deduplication by equals()
--FILE--
<?php
class Id implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$s = new Ds\Set();

// Add unique elements
$s->add(new Id(1));
$s->add(new Id(2));
$s->add(new Id(3));
echo "after 3 unique: " . $s->count() . "\n";

// Add duplicate - should not increase count
$s->add(new Id(1));
echo "after dup add: " . $s->count() . "\n";

// Add another duplicate
$s->add(new Id(2));
echo "after dup add 2: " . $s->count() . "\n";

// Contains existing
echo "contains Id(1): " . var_export($s->contains(new Id(1)), true) . "\n";
echo "contains Id(2): " . var_export($s->contains(new Id(2)), true) . "\n";
echo "contains Id(3): " . var_export($s->contains(new Id(3)), true) . "\n";

// Contains non-existing
echo "contains Id(4): " . var_export($s->contains(new Id(4)), true) . "\n";
echo "contains Id(99): " . var_export($s->contains(new Id(99)), true) . "\n";

// Add multiple at once with duplicates
$s2 = new Ds\Set();
$s2->add(new Id(1), new Id(2), new Id(1), new Id(3), new Id(2));
echo "variadic with dups: " . $s2->count() . "\n";

// Contains with variadic
echo "contains all: " . var_export($s2->contains(new Id(1), new Id(2), new Id(3)), true) . "\n";
echo "contains partial: " . var_export($s2->contains(new Id(1), new Id(99)), true) . "\n";

// Add many elements
$s3 = new Ds\Set();
for ($i = 0; $i < 100; $i++) {
    $s3->add(new Id($i));
}
echo "100 unique: " . $s3->count() . "\n";

// Add all again - should stay at 100
for ($i = 0; $i < 100; $i++) {
    $s3->add(new Id($i));
}
echo "after re-add: " . $s3->count() . "\n";

// Contains all of them
$allFound = true;
for ($i = 0; $i < 100; $i++) {
    if (!$s3->contains(new Id($i))) {
        $allFound = false;
        break;
    }
}
echo "all 100 found: " . var_export($allFound, true) . "\n";
?>
--EXPECT--
after 3 unique: 3
after dup add: 3
after dup add 2: 3
contains Id(1): true
contains Id(2): true
contains Id(3): true
contains Id(4): false
contains Id(99): false
variadic with dups: 3
contains all: true
contains partial: false
100 unique: 100
after re-add: 100
all 100 found: true
