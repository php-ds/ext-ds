<?php
namespace Ds\Tests\Queue;

trait pop
{
    public function popDataProvider()
    {
        // initial, returned, expected result
        return [
            [['a'],         'a',    []],
            [['a', 'b'],    'a',    ['b']],
        ];
    }

    /**
     * @dataProvider popDataProvider
     */
    public function testPop($initial, $returned, array $expected)
    {
        $instance = $this->getInstance($initial);

        $value = $instance->pop();

        $this->assertToArray($expected, $instance);
        $this->assertEquals($returned, $value);
        $this->assertEquals(count($initial) - 1, count($instance));
    }

    public function testPopNotAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->pop();
    }

    public function testPopAll()
    {
        $instance = $this->getInstance(range(1, self::MANY));

        while ( ! $instance->isEmpty()) {
            $instance->pop();
        }

        $this->assertEquals(count($instance), 0);
    }
}
