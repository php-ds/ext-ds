<?php
namespace Ds\Tests\Queue;

trait copy
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testCopy(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $copy = $instance->copy();

        $this->assertEquals($instance->toArray(), $copy->toArray());
        $this->assertEquals(count($instance), count($copy));
    }
}
