<?php
namespace Ds\Tests\PriorityQueue;

trait clear
{
    public function testClear()
    {
        $instance = $this->getInstance();

        foreach (range(1, self::MANY) as $i) {
            $instance->push($i, rand());
        }

        $this->assertCount(self::MANY, $instance);

        $instance->clear();
        $this->assertCount(0, $instance);
        $this->assertTrue($instance->isEmpty());
        $this->assertToArray([], $instance);
    }
}
