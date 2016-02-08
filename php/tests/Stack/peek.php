<?php
namespace Ds\Tests\Stack;

trait peek
{
    public function peekDataProvider()
    {
        // initial, returned, expected result
        return [
            [['a'],         'a'],
            [['a', 'b'],    'b'],
        ];
    }

    /**
     * @dataProvider peekDataProvider
     */
    public function testPeek($initial, $returned)
    {
        $instance = $this->getInstance($initial);

        $value = $instance->peek();

        $this->assertToArray(array_reverse($initial), $instance);
        $this->assertEquals($returned, $value);
    }

    public function testPeekNotAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->peek();
    }
}
