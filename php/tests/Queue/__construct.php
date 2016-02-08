<?php
namespace Ds\Tests\Queue;

use ds;
use Ds\Queue;

trait __construct
{
    public function constructDataProvider()
    {
        return [
            [[]],
            [['a']],
            [['a', 'a']],
            [['a', 'b']],
            [$this->sample()],
        ];
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstruct(array $values)
    {
        $this->assertToArray($values, new Queue($values));
        $this->assertToArray($values, ds::queue($values));
    }

    /**
     * @dataProvider constructDataProvider
     */
    public function testConstructUsingIterable(array $values)
    {
        $this->assertToArray($values, new Queue(new \ArrayIterator($values)));
        $this->assertToArray($values, ds::queue(new \ArrayIterator($values)));
    }

    public function testConstructCapacity()
    {
        $n = 20;
        $this->assertEquals(32, (new Queue($n))->capacity());
        $this->assertEquals(32, (ds::queue($n))->capacity());
    }

    public function testConstructNoParams()
    {
        $this->assertToArray([], new Queue());
        $this->assertToArray([], ds::queue());
    }
}
