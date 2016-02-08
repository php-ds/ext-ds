<?php
namespace Ds\Tests\Stack;

trait pop
{

    public function popDataProvider()
    {
        // initial, returned, expected result
        return [
            [['a'],       'a',  []],
            [['a', 'b'],  'b',  ['a']],
        ];
    }

    /**
     * @dataProvider popDataProvider
     */
    public function testPop($initial, $returned, array $expected)
    {
        $instance = $this->getInstance($initial);

        $result = $instance->pop();

        $this->assertToArray($expected, $instance);
        $this->assertEquals($returned, $result);
        $this->assertEquals(count($initial) - 1, count($instance));
    }

    public function testPopAll()
    {
        $instance = $this->getInstance(range(1, self::MANY));

        while ( ! $instance->isEmpty()) {
            $instance->pop();
        }

        $this->assertEquals(count($instance), 0);
    }

    public function testPopNowAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->pop();
    }
}
