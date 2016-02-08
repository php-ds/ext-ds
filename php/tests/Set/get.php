<?php
namespace Ds\Tests\Set;

trait get
{
    public function getDataProvider()
    {
        // initial, index, return
        return [

            [[0], 0, 0],

            [['a'], 0, 'a'],

            [['a', 'b'], 0, 'a'],
            [['a', 'b'], 1, 'b'],

            [['a', 'b', 'c'], 0, 'a'],
            [['a', 'b', 'c'], 1, 'b'],
            [['a', 'b', 'c'], 2, 'c'],
        ];
    }

    /**
     * @dataProvider getDataProvider
     */
    public function testGet(array $initial, $index, $return)
    {
        $instance = $this->getInstance($initial);

        $returned = $instance->get($index);

        $this->assertEquals(count($initial), count($instance));
        $this->assertEquals($return, $returned);
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testGetIndexOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectIndexOutOfRangeException();
        $instance->get($index);
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testGetIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance();
        $this->expectWrongIndexTypeException();
        $instance->get($index);
    }

    /**
     * @dataProvider getDataProvider
     */
    public function testArrayAccessGet(array $initial, $index, $return)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals($return, $instance[$index]);
    }

    /**
     * @dataProvider badIndexDataProvider
     */
    public function testArrayAccessGetIndexBadIndex($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectWrongIndexTypeException();
        $instance[$index];
    }

    /**
     * @dataProvider outOfRangeDataProvider
     */
    public function testArrayAccessGetIndexOutOfRange($initial, $index)
    {
        $instance = $this->getInstance($initial);
        $this->expectIndexOutOfRangeException();
        $instance[$index];
    }

    public function testArrayAccessGetByReference()
    {
        $instance = $this->getInstance([[1]]);
        $this->expectAccessByReferenceNotAllowedException();
        $instance[0][0] = null;
    }

    public function testGetFirstAfterRemove()
    {
        $instance = $this->getInstance();

        $instance->add('a', 'b', 'c');
        $this->assertEquals('a', $instance->get(0));

        $instance->remove('a');
        $this->assertEquals('b', $instance->get(0));

        $instance->remove('b');
        $this->assertEquals('c', $instance->get(0));
    }

    public function testGetLastAfterRemove()
    {
        $instance = $this->getInstance();

        $instance->add('a', 'b', 'c');
        $this->assertEquals('c', $instance->get(2));

        $instance->remove('c');
        $this->assertEquals('b', $instance->get(1));

        $instance->remove('b');
        $this->assertEquals('a', $instance->get(0));
    }
}
