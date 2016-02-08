<?php
namespace Ds\Tests\Vector;

use ds;
use Ds\Vector;

trait __construct
{
    public function constructDataProvider()
    {
        return array_map(function($a) { return [$a, $a]; }, [
            [],
            ['a'],
            ['a', 'b'],
            ['a', 'b', 'c'],
            $this->sample(),
            range(1, self::MANY),
        ]);
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstruct($values, array $expected)
    {
        $this->assertToArray($expected, new Vector($values));
        $this->assertToArray($expected, ds::vector($values));
    }

   /**
     * @dataProvider constructDataProvider
     */
    public function testConstructUsingNonArrayIterable(array $values, array $expected)
    {
        $this->assertToArray($expected, new Vector(new \ArrayIterator($values)));
        $this->assertToArray($expected, ds::vector(new \ArrayIterator($values)));
    }

    public function testConstructCapacity()
    {
        $n = 20;
        $this->assertEquals($n, (new Vector($n))->capacity());
        $this->assertEquals($n, (ds::vector($n))->capacity());
    }

    public function testConstructNoParams()
    {
        $this->assertToArray([], new Vector());
        $this->assertToArray([], ds::vector());
    }
}
