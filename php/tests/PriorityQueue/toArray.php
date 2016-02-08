<?php
namespace Ds\Tests\PriorityQueue;

trait toArray
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testToArray(array $values, array $expected)
    {
        $instance = $this->getInstance($values);

        // Also check that toArray is not destructive
        $this->assertToArray($expected, $instance);
        $this->assertToArray($expected, $instance);
    }
}
