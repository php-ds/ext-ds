--TEST--
Key: comprehensive Map operations with Key objects
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

// put and get
$m = new Ds\Map();
$m->put(new Id(1), "one");
$m->put(new Id(2), "two");
$m->put(new Id(3), "three");
echo "count: " . $m->count() . "\n";
echo "get 1: " . $m->get(new Id(1)) . "\n";
echo "get 2: " . $m->get(new Id(2)) . "\n";
echo "get 3: " . $m->get(new Id(3)) . "\n";

// put overwrites
$m->put(new Id(2), "TWO");
echo "overwrite: " . $m->get(new Id(2)) . "\n";
echo "count unchanged: " . $m->count() . "\n";

// get with default
echo "get missing with default: " . $m->get(new Id(99), "default") . "\n";

// get missing without default throws
try {
    $m->get(new Id(99));
    echo "ERROR: should have thrown\n";
} catch (\OutOfBoundsException $e) {
    echo "missing throws: yes\n";
}

// hasKey
echo "hasKey 1: " . var_export($m->hasKey(new Id(1)), true) . "\n";
echo "hasKey 99: " . var_export($m->hasKey(new Id(99)), true) . "\n";

// hasValue
echo "hasValue one: " . var_export($m->hasValue("one"), true) . "\n";
echo "hasValue missing: " . var_export($m->hasValue("missing"), true) . "\n";

// remove
$removed = $m->remove(new Id(3));
echo "removed: $removed\n";
echo "after remove: " . $m->count() . "\n";
echo "hasKey removed: " . var_export($m->hasKey(new Id(3)), true) . "\n";

// remove with default
$result = $m->remove(new Id(99), "not-found");
echo "remove missing with default: $result\n";

// keys and values
$m2 = new Ds\Map();
$m2->put(new Id(10), "ten");
$m2->put(new Id(20), "twenty");
$m2->put(new Id(30), "thirty");

$values = $m2->values();
echo "values type: " . get_class($values) . "\n";
echo "values count: " . $values->count() . "\n";

$keys = $m2->keys();
echo "keys type: " . get_class($keys) . "\n";
echo "keys count: " . $keys->count() . "\n";

// diff
$m3 = new Ds\Map();
$m3->put(new Id(1), "a");
$m3->put(new Id(2), "b");
$m3->put(new Id(3), "c");

$m4 = new Ds\Map();
$m4->put(new Id(2), "x");
$m4->put(new Id(4), "y");

$diff = $m3->diff($m4);
echo "diff count: " . $diff->count() . "\n";
echo "diff has 1: " . var_export($diff->hasKey(new Id(1)), true) . "\n";
echo "diff has 3: " . var_export($diff->hasKey(new Id(3)), true) . "\n";
echo "diff has 2: " . var_export($diff->hasKey(new Id(2)), true) . "\n";

// intersect
$inter = $m3->intersect($m4);
echo "intersect count: " . $inter->count() . "\n";
echo "intersect has 2: " . var_export($inter->hasKey(new Id(2)), true) . "\n";
echo "intersect value: " . $inter->get(new Id(2)) . "\n";

// union
$union = $m3->union($m4);
echo "union count: " . $union->count() . "\n";
echo "union 1: " . $union->get(new Id(1)) . "\n";
echo "union 2: " . $union->get(new Id(2)) . "\n";
echo "union 3: " . $union->get(new Id(3)) . "\n";
echo "union 4: " . $union->get(new Id(4)) . "\n";

// isEmpty / clear
$m5 = new Ds\Map();
echo "new isEmpty: " . var_export($m5->isEmpty(), true) . "\n";
$m5->put(new Id(1), "v");
echo "after put isEmpty: " . var_export($m5->isEmpty(), true) . "\n";
$m5->clear();
echo "after clear isEmpty: " . var_export($m5->isEmpty(), true) . "\n";
echo "after clear count: " . $m5->count() . "\n";

// copy independence
$m6 = new Ds\Map();
$m6->put(new Id(1), "original");
$m7 = $m6->copy();
$m7->put(new Id(1), "modified");
echo "original: " . $m6->get(new Id(1)) . "\n";
echo "copy: " . $m7->get(new Id(1)) . "\n";
?>
--EXPECT--
count: 3
get 1: one
get 2: two
get 3: three
overwrite: TWO
count unchanged: 3
get missing with default: default
missing throws: yes
hasKey 1: true
hasKey 99: false
hasValue one: true
hasValue missing: false
removed: three
after remove: 2
hasKey removed: false
remove missing with default: not-found
values type: Ds\Seq
values count: 3
keys type: Ds\Set
keys count: 3
diff count: 2
diff has 1: true
diff has 3: true
diff has 2: false
intersect count: 1
intersect has 2: true
intersect value: b
union count: 4
union 1: a
union 2: x
union 3: c
union 4: y
new isEmpty: true
after put isEmpty: false
after clear isEmpty: true
after clear count: 0
original: original
copy: modified
