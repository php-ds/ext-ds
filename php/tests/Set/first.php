<?php
namespace Ds\Tests\Set;

trait first
{
    public function firstDataProvider()
    {
        // initial, returned
        return [
            [['a'],             'a'],
            [['a', 'b'],        'a'],
            [['a', 'b', 'c'],   'a'],
        ];
    }

    /**
     * @dataProvider firstDataProvider
     */
    public function testFirst(array $initial, $expected)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals($expected, $instance->first());
    }

    public function testFirstNowAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->first();
    }
}
