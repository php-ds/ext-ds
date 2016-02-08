<?php
namespace Ds\Tests\PriorityQueue;

trait count
{
    public function testCount()
    {
        $instance = $this->getInstance();

        foreach (range(1, self::MANY) as $i) {
            $instance->push($i, rand());
        }

        $this->assertCount(self::MANY, $instance);
    }
}
