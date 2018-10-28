# PHP Decimal

[![Build Status](https://travis-ci.org/php-decimal/extension.svg?branch=master)](https://travis-ci.org/php-decimal/extension)
[![Build status](https://ci.appveyor.com/api/projects/status/lg5nw5tqgpmv1c33?svg=true)](https://ci.appveyor.com/project/rtheunissen/php-decimal)

Correctly-rounded, arbitrary precision decimal floating-point arithmetic in PHP 7.

## Documentation

See http://php-decimal.io

## Dependencies

- PHP 7
- libmpdec 2.4

```bash
sudo apt-get install libmpdec-dev
```

## Install

### PECL

```
pecl install decimal
```

### Manual

```
git clone git@github.com:php-decimal/extension ext-decimal
cd ext-decimal

phpize
./configure
make
sudo make install
```

## Enable

You can do this temporarily using `php -dextension=decimal.so` or by adding `extension=decimal.so` to your INI. If you manage PHP with [phpbrew](https://github.com/phpbrew/phpbrew), you can use `phpbrew ext enable decimal`.

## Tests

```
php run-tests.php -P -q
```


