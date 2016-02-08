<?php
namespace Ds\Tests\Pair;

trait toArray
{
    public function testToArray()
    {
        $instance = $this->getPair('a', 1);
        $this->assertEquals(['a', 1], $instance->toArray());
    }
}
