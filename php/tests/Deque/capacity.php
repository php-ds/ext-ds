<?php
namespace Ds\Tests\Deque;

trait capacity
{
    public function testCapacity()
    {
        $instance = $this->getInstance();
        $this->assertEquals(8, $instance->capacity());

        for ($i = 0; $i < 8; $i++) {
            $instance->push($i);
        }

        // Should resize when full after push
        $this->assertEquals(16, $instance->capacity());
    }

    public function testAutoTruncate()
    {
        $instance = $this->getInstance(range(1, self::MANY));
        $capacity = $instance->capacity();

        for ($i = 0; $i < 3 * self::MANY / 4; $i++) {
            $instance->pop();
        }

        $this->assertEquals($capacity / 2, $instance->capacity());
    }

    public function testClearResetsCapacity()
    {
        $instance = $this->getInstance(range(1, self::MANY));
        $instance->clear();
        $this->assertEquals(8, $instance->capacity());
    }
}
