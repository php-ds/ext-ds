<?php

namespace Ds\Tests;

use Ds\Collection;

abstract class CollectionTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Sample size to test larger operations.
     */
    const MANY = 1 << 8 + 1;
    const SOME = 1 << 4 + 1;

    public function setUp()
    {
        srand(1);
    }

    /**
     *
     */
    protected function getInstance(array $values = [])
    {
        return null;
    }

    /**
     *
     */
    public function sample()
    {
        return array_merge(
            [[], null],
            ['#', '1', 1, 1.0, true],
            ['',  '0', 0, 0.0, false],
            ['a', 'A', new \stdClass()],
            range(2, self::SOME)
        );
    }

    public function assertInstanceToString($instance)
    {
        $this->assertEquals("object(" . get_class($instance) . ')', "$instance");
    }

    /**
     *
     */
    public function assertToArray(array $expected, $instance)
    {
        $this->assertEquals($expected, $instance->toArray());
    }

    public function basicDataProvider()
    {
        $sample = $this->sample();

        $values = [
            [],
            ['a'],
            ['a', 'b'],
            ['a', 'b', 'c'],
            $sample,
        ];

        return array_map(function($a) { return [$a, $a]; }, $values);
    }


    public function expectPropertyDoesNotExistException()
    {
        $this->setExpectedException(\OutOfBoundsException::class);
    }

    public function expectReconstructionNotAllowedException()
    {
        $this->setExpectedException('Error');
    }

    public function expectImmutableException()
    {
        $this->setExpectedException('Error');
    }

    public function expectAccessByReferenceNotAllowedException()
    {
        $this->setExpectedException('Error');
    }

    public function expectListNotSupportedException()
    {
        $this->setExpectedException('Error');
    }

    public function expectIterateByReferenceException()
    {
        $this->setExpectedException('Error');
    }

    public function expectWrongIndexTypeException()
    {
        $this->setExpectedException('TypeError');
    }

    public function expectOutOfBoundsException()
    {
        $this->setExpectedException(\OutOfBoundsException::class);
    }

    public function expectArrayAccessUnsupportedException()
    {
        $this->setExpectedException('Error');
    }

    public function expectKeyNotFoundException()
    {
        $this->setExpectedException(\OutOfBoundsException::class);
    }

    public function expectIndexOutOfRangeException()
    {
        $this->setExpectedException(\OutOfRangeException::class);
    }

    public function expectEmptyNotAllowedException()
    {
        $this->setExpectedException(\UnderflowException::class);
    }

    public function expectNotIterableOrArrayException()
    {
        $this->setExpectedException('TypeError');
    }

    public function expectInternalIllegalOffset()
    {
        $this->setExpectedException('PHPUnit_Framework_Error_Warning');
    }

    public function outOfRangeDataProvider()
    {
        return [
            [[ ], -1],
            [[ ],  1],
            [[1],  2],
            [[1], -1],
        ];
    }

    public function badIndexDataProvider()
    {
        return [
            [[], 'a'],
        ];
    }

    public function assertInstanceDump(array $expected, $instance)
    {
        ob_start();
        var_dump($instance);
        $actual = ob_get_clean();

        ob_start();
        var_dump($expected);
        $expected = ob_get_clean();

        $class = preg_quote(get_class($instance));
        $data  = preg_quote(substr($expected, 5)); // Slice past 'array'
        $regex = preg_replace('/#\d+/', '#\d+', "object\($class\)#\d+ $data");

        $this->assertRegExp("~$regex~", $actual);
    }

    public function assertSerialized(array $expected, $instance, $use_keys)
    {
        $unserialized = unserialize(serialize($instance));

        // Check that the instance is an instance of the correct class and that
        // its values reflect the original values.
        $this->assertEquals(get_class($instance), get_class($unserialized));
        $this->assertEquals($instance->toArray(), $unserialized->toArray());
        $this->assertTrue($instance !== $unserialized);
    }

    public function assertForEach(array $expected, $instance)
    {
        $data = [];

        foreach ($instance as $key => $value) {
            $data[$key] = $value;
        }

        $this->assertEquals($expected, $data);
    }

    public function assertForEachByReferenceThrowsException($instance)
    {
        $this->expectIterateByReferenceException();
        foreach ($instance as &$value);
    }
}
