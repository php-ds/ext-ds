<?php
namespace Ds\Tests\Vector;

trait capacity
{
    public function testCapacity()
    {
        $instance = $this->getInstance();
        $this->assertEquals(10, $instance->capacity());

        for ($i = 0; $i < 10; $i++) {
            $instance->push($i);
        }

        // Should not resize when full after push
        $this->assertEquals(10, $instance->capacity());

        // Should resize if full before push
        $instance->push(10);
        $this->assertEquals(15, $instance->capacity());
    }

    public function testAutoTruncate()
    {
        $instance = $this->getInstance(range(1, self::MANY));
        $capacity = $instance->capacity();

        for ($i = 0; $i <= 3 * self::MANY / 4; $i++) {
            $instance->pop();
        }

        $this->assertEquals(floor($capacity / 2), $instance->capacity());
    }

    public function testClearResetsCapacity()
    {
        $instance = $this->getInstance(range(1, self::MANY));
        $instance->clear();
        $this->assertEquals(10, $instance->capacity());
    }
}
