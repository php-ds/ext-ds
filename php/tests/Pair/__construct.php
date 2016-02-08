<?php
namespace Ds\Tests\Pair;

use ds;
use Ds\Pair;

trait __construct
{
    public function testConstruct()
    {
        $pair = new Pair('a', 1);
        $this->assertEquals('a', $pair->key);
        $this->assertEquals( 1,  $pair->value);
    }

    public function testReconstruct()
    {
        $pair = new Pair('a', 1);

        $this->expectReconstructionNotAllowedException();
        $pair->__construct('b', 2);

        $this->assertEquals('a', $pair->key);
        $this->assertEquals( 1,  $pair->value);
    }

    public function testConstructUsingStatic()
    {
        $pair = ds::pair('a', 1);
        $this->assertEquals('a', $pair->key);
        $this->assertEquals( 1,  $pair->value);
    }

    public function testReconstructUsingStatic()
    {
        $pair = ds::pair('a', 1);

        $this->expectReconstructionNotAllowedException();
        $pair->__construct('b', 2);

        $this->assertEquals('a', $pair->key);
        $this->assertEquals( 1,  $pair->value);
    }
}
