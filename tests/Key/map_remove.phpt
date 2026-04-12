--TEST--
Key: removing Key objects from Map
--FILE--
<?php
class Id implements Ds\Key {
    public function __construct(private int $id) {}
    public function hash(): mixed { return $this->id; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->id === $other->id;
    }
}

$m = new Ds\Map();
$m->put(new Id(1), "one");
$m->put(new Id(2), "two");
$m->put(new Id(3), "three");

echo "before: " . $m->count() . "\n";

$removed = $m->remove(new Id(2));
echo "removed: $removed\n";
echo "after: " . $m->count() . "\n";
echo "has 2: " . var_export($m->hasKey(new Id(2)), true) . "\n";
echo "has 1: " . var_export($m->hasKey(new Id(1)), true) . "\n";
?>
--EXPECT--
before: 3
removed: two
after: 2
has 2: false
has 1: true
