<?php
namespace Ds\Tests\Map;

trait _clone
{
    public function cloneDataProvider()
    {
        return $this->basicDataProvider();
    }

    /**
     * @dataProvider cloneDataProvider
     */
    public function testClone($values, array $expected)
    {
        $instance = $this->getInstance($values);

        $clone = clone $instance;

        $this->assertEquals(get_class($instance), get_class($clone));
        $this->assertEquals($instance->toArray(), $clone->toArray());
        $this->assertFalse($clone === $instance);
    }
}
