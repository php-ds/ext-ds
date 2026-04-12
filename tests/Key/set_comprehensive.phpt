--TEST--
Key: comprehensive Set operations with Key objects
--FILE--
<?php
class Id implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
    public function getId(): int { return $this->id; }
}

// add and contains
$s = new Ds\Set();
$s->add(new Id(1));
$s->add(new Id(2));
$s->add(new Id(3));
echo "count: " . $s->count() . "\n";
echo "contains 1: " . var_export($s->contains(new Id(1)), true) . "\n";
echo "contains 2: " . var_export($s->contains(new Id(2)), true) . "\n";
echo "contains 99: " . var_export($s->contains(new Id(99)), true) . "\n";

// remove
$s->remove(new Id(2));
echo "after remove 2: " . $s->count() . "\n";
echo "contains 2: " . var_export($s->contains(new Id(2)), true) . "\n";
echo "contains 1: " . var_export($s->contains(new Id(1)), true) . "\n";
echo "contains 3: " . var_export($s->contains(new Id(3)), true) . "\n";

// diff
$s1 = new Ds\Set();
$s1->add(new Id(1), new Id(2), new Id(3), new Id(4));

$s2 = new Ds\Set();
$s2->add(new Id(2), new Id(4), new Id(5));

$diff = $s1->diff($s2);
echo "diff count: " . $diff->count() . "\n";
echo "diff has 1: " . var_export($diff->contains(new Id(1)), true) . "\n";
echo "diff has 3: " . var_export($diff->contains(new Id(3)), true) . "\n";
echo "diff has 2: " . var_export($diff->contains(new Id(2)), true) . "\n";
echo "diff has 4: " . var_export($diff->contains(new Id(4)), true) . "\n";

// intersect
$inter = $s1->intersect($s2);
echo "intersect count: " . $inter->count() . "\n";
echo "intersect has 2: " . var_export($inter->contains(new Id(2)), true) . "\n";
echo "intersect has 4: " . var_export($inter->contains(new Id(4)), true) . "\n";
echo "intersect has 1: " . var_export($inter->contains(new Id(1)), true) . "\n";

// union
$union = $s1->union($s2);
echo "union count: " . $union->count() . "\n";
echo "union has 1: " . var_export($union->contains(new Id(1)), true) . "\n";
echo "union has 2: " . var_export($union->contains(new Id(2)), true) . "\n";
echo "union has 3: " . var_export($union->contains(new Id(3)), true) . "\n";
echo "union has 4: " . var_export($union->contains(new Id(4)), true) . "\n";
echo "union has 5: " . var_export($union->contains(new Id(5)), true) . "\n";

// isEmpty / clear
$s3 = new Ds\Set();
echo "new isEmpty: " . var_export($s3->isEmpty(), true) . "\n";
$s3->add(new Id(1));
echo "after add isEmpty: " . var_export($s3->isEmpty(), true) . "\n";
$s3->clear();
echo "after clear isEmpty: " . var_export($s3->isEmpty(), true) . "\n";
echo "after clear count: " . $s3->count() . "\n";

// copy independence
$s4 = new Ds\Set();
$s4->add(new Id(1), new Id(2));
$s5 = $s4->copy();
$s5->add(new Id(3));
echo "original count: " . $s4->count() . "\n";
echo "copy count: " . $s5->count() . "\n";

// remove non-existing (should not throw)
$s6 = new Ds\Set();
$s6->add(new Id(1));
$s6->remove(new Id(99));
echo "remove non-existing: count=" . $s6->count() . "\n";

// toArray
$s7 = new Ds\Set();
$s7->add(new Id(10), new Id(20));
$arr = $s7->toArray();
echo "toArray count: " . count($arr) . "\n";
echo "toArray is array: " . var_export(is_array($arr), true) . "\n";

// Iteration
$s8 = new Ds\Set();
$s8->add(new Id(5), new Id(10), new Id(15));
$ids = [];
foreach ($s8 as $item) {
    $ids[] = $item->getId();
}
sort($ids);
echo "iterated: " . implode(",", $ids) . "\n";

// Symmetric diff via (A diff B) union (B diff A)
$a = new Ds\Set();
$a->add(new Id(1), new Id(2), new Id(3));
$b = new Ds\Set();
$b->add(new Id(2), new Id(3), new Id(4));
$symDiff = $a->diff($b)->union($b->diff($a));
echo "sym diff count: " . $symDiff->count() . "\n";
echo "sym diff has 1: " . var_export($symDiff->contains(new Id(1)), true) . "\n";
echo "sym diff has 4: " . var_export($symDiff->contains(new Id(4)), true) . "\n";
echo "sym diff has 2: " . var_export($symDiff->contains(new Id(2)), true) . "\n";
?>
--EXPECT--
count: 3
contains 1: true
contains 2: true
contains 99: false
after remove 2: 2
contains 2: false
contains 1: true
contains 3: true
diff count: 2
diff has 1: true
diff has 3: true
diff has 2: false
diff has 4: false
intersect count: 2
intersect has 2: true
intersect has 4: true
intersect has 1: false
union count: 5
union has 1: true
union has 2: true
union has 3: true
union has 4: true
union has 5: true
new isEmpty: true
after add isEmpty: false
after clear isEmpty: true
after clear count: 0
original count: 2
copy count: 3
remove non-existing: count=1
toArray count: 2
toArray is array: true
iterated: 5,10,15
sym diff count: 2
sym diff has 1: true
sym diff has 4: true
sym diff has 2: false
