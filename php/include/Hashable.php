<?php
namespace Ds;

interface Hashable
{
    /**
     * Produces a scalar value to be used as the object's hash.
     *
     * @return mixed Scalar hash value.
     */
    function hash();

    /**
     * Returns whether this object is considered equal to another.
     *
     * @param Hashable $obj
     *
     * @return bool true if equal, false otherwise.
     */
    function equals(Hashable $obj): bool;
}
