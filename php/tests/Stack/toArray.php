<?php
namespace Ds\Tests\Stack;

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
