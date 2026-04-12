--TEST--
Key: objects implementing Key interface work as Map/Set keys
--FILE--
<?php
class Point implements Ds\Key {
    public function __construct(public int $x, public int $y) {}
    public function hash(): mixed { return "$this->x,$this->y"; }
    public function equals(mixed $other): bool {
        return $other instanceof self && $this->x === $other->x && $this->y === $other->y;
    }
}

// Map with Key objects
$m = new Ds\Map();
$m->put(new Point(1, 2), "A");
$m->put(new Point(3, 4), "B");
$m->put(new Point(1, 2), "C"); // should overwrite "A"

echo "map count: " . $m->count() . "\n";
echo "point(1,2): " . $m->get(new Point(1, 2)) . "\n";
echo "point(3,4): " . $m->get(new Point(3, 4)) . "\n";

// Set with Key objects
$s = new Ds\Set();
$s->add(new Point(1, 2));
$s->add(new Point(3, 4));
$s->add(new Point(1, 2)); // duplicate

echo "set count: " . $s->count() . "\n";
echo "contains (1,2): " . var_export($s->contains(new Point(1, 2)), true) . "\n";
echo "contains (5,6): " . var_export($s->contains(new Point(5, 6)), true) . "\n";
?>
--EXPECT--
map count: 2
point(1,2): C
point(3,4): B
set count: 2
contains (1,2): true
contains (5,6): false
