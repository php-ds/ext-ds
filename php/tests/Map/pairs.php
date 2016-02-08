<?php
namespace Ds\Tests\Map;

use Ds\Vector;

trait pairs
{
    /**
     * @dataProvider pairsDataProvider
     */
    public function testPairs(array $initial, array $expected)
    {
        $instance = $this->getInstance($initial);
        $pairs = $instance->pairs();

        $this->assertInstanceOf(Vector::class, $pairs);

        //
        $to_array = function ($pair) { return $pair->toArray(); };

        $this->assertEquals($expected, array_map($to_array, $pairs->toArray()));
    }

}
