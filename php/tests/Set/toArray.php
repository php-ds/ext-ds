<?php
namespace Ds\Tests\Set;

trait toArray
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testToArray(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertToArray($expected, $instance);
    }
}
