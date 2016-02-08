<?php
namespace Ds\Tests\Set;

trait sort
{
    public function testSort()
    {
        $instance = $this->getInstance([4, 1, 2, 5, 3]);

        $this->assertToArray([1, 2, 3, 4, 5], $instance->sort());
        $this->assertToArray([4, 1, 2, 5, 3], $instance);
    }

    public function testSortUsingComparator()
    {
        $instance = $this->getInstance([4, 1, 2, 5, 3]);

        $sorted = $instance->sort(function($a, $b) {
            return $b <=> $a;
        });

        $this->assertToArray([5, 4, 3, 2, 1], $sorted);
        $this->assertToArray([4, 1, 2, 5, 3], $instance);
    }

    public function testSortIsStable()
    {
        $range = range(1, self::MANY);
        $pairs = array_map(function($a) { return [$a, 1]; }, $range);

        $instance = $this->getInstance($pairs);

        $sorted = $instance->sort(function($a, $b) {
            return $b[0] <=> $a[0];
        });

        $this->assertToArray(array_reverse($pairs), $sorted);
    }
}
