<?php
namespace Ds\Tests\Map;

trait reverse
{
    public function reverseDataProvider()
    {
        $reverse = function($a) {
            return [$a[0], array_reverse($a[1], 1)];
        };

        return array_map($reverse, $this->basicDataProvider());
    }

    /**
     * @dataProvider reverseDataProvider
     */
    public function testReverse(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertToArray($expected, $instance->reverse());
        $this->assertToArray($values, $instance);
    }
}
