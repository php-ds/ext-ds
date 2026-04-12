--TEST--
Set::ArrayAccess - array syntax operations
--FILE--
<?php
$s = new Ds\Set([10, 20, 30]);

// Read by index via $s[0]
echo "s[0]: " . $s[0] . "\n";
echo "s[1]: " . $s[1] . "\n";
echo "s[2]: " . $s[2] . "\n";

// Push via $s[] = value
$s[] = 40;
echo "after push: " . $s->join(",") . "\n";
echo "count: " . $s->count() . "\n";

// Push duplicate is ignored
$s[] = 10;
echo "after dup push: " . $s->count() . "\n";

// Setting by index throws Error
try {
    $s[0] = 99;
} catch (Error $e) {
    echo "set by index: " . $e->getMessage() . "\n";
}

// isset by key is not supported
try {
    isset($s[0]);
} catch (Error $e) {
    echo "isset: " . $e->getMessage() . "\n";
}

// unset by key is not supported
try {
    unset($s[0]);
} catch (Error $e) {
    echo "unset: " . $e->getMessage() . "\n";
}

// Read out of range
try {
    $s[100];
} catch (OutOfRangeException $e) {
    echo "out of range: " . $e->getMessage() . "\n";
}

// Negative index
try {
    $s[-1];
} catch (OutOfRangeException $e) {
    echo "negative: " . $e->getMessage() . "\n";
}
?>
--EXPECT--
s[0]: 10
s[1]: 20
s[2]: 30
after push: 10,20,30,40
count: 4
after dup push: 4
set by index: Array access by key is not supported
isset: Array access by key is not supported
unset: Array access by key is not supported
out of range: Index out of range: 100, expected 0 <= x <= 3
negative: Index out of range: -1, expected 0 <= x <= 3
