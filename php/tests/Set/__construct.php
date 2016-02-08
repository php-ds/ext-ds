<?php
namespace Ds\Tests\Set;

use ds;
use Ds\Set;

trait __construct
{
    public function constructDataProvider()
    {
        list($unique, $duplicated) = $this->getUniqueAndDuplicateData();

        return [
            [[],            []],
            [['a'],         ['a']],
            [['a', 'a'],    ['a']],
            [['a', 'b'],    ['a', 'b']],
            [$unique,       $unique],
            [$duplicated,   $unique],
        ];
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstruct(array $values, array $expected)
    {
        $this->assertToArray($expected, new Set($values));
        $this->assertToArray($expected, ds::set($values));
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstructUsingIterable(array $values, array $expected)
    {
        $this->assertToArray($expected, new Set(new \ArrayIterator($values)));
        $this->assertToArray($expected, ds::set(new \ArrayIterator($values)));
    }

    public function testConstructNoParams()
    {
        $this->assertToArray([], new Set());
        $this->assertToArray([], ds::set());
    }

    public function testConstructCapacity()
    {
        $this->assertEquals(32, (new Set(20))->capacity());
        $this->assertEquals(32, (ds::set(20))->capacity());

    }
}
