<?php

namespace Ds\Tests;

use Ds\Hashable;

/**
 *
 */
class HashableObject implements Hashable {

    private $id;

    public function __construct(int $id)
    {
        $this->id = $id;
    }

    public function hash(): string {
        return $this->id;
    }

    public function equals($obj): bool {
        return $obj && ($obj->id === $this->id);
    }
}
