<?php
namespace Ds\Tests\PriorityQueue;

use ds;
use Ds\PriorityQueue;

trait __construct
{
    public function testConstruct()
    {
        $this->assertToArray([], new PriorityQueue());
        $this->assertToArray([], ds::priority_queue());
    }
}
