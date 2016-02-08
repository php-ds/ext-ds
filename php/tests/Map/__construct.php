<?php
namespace Ds\Tests\Map;

use ds;
use Ds\Map;

trait __construct
{
    public function constructDataProvider()
    {
        return array_map(function($a) { return [$a, $a]; }, [
            [],
            ['a' => 1],
            ['a' => 1, 'b' => 2],
            ['a' => 1, 'b' => 2, 'c' => 3],
            $this->sample(),
        ]);
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstruct(array $values, array $expected)
    {
        $this->assertToArray($expected, new Map($values));
        $this->assertToArray($expected, ds::map($values));
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstructUsingNonArrayIterable(array $values, array $expected)
    {
        $this->assertToArray($expected, new Map(new \ArrayIterator($values)));
        $this->assertToArray($expected, ds::map(new \ArrayIterator($values)));
    }

    public function testConstructCapacity()
    {
        $n = 20;
        $this->assertEquals(32, (new Map($n))->capacity());
        $this->assertEquals(32, (ds::map($n))->capacity());
    }

    public function testConstructNoParams()
    {
        $this->assertToArray([], new Map());
        $this->assertToArray([], ds::map());
    }
}
