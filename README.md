# PHP Decimal

[![Build Status](https://travis-ci.org/php-decimal/extension.svg?branch=master)](https://travis-ci.org/php-decimal/extension)
[![Build status](https://ci.appveyor.com/api/projects/status/lg5nw5tqgpmv1c33?svg=true)](https://ci.appveyor.com/project/rtheunissen/php-decimal)

Adds support for correctly-rounded arbitrary precision decimal floating point arithmetic in PHP 7.

This library is unreleased, and still a work in progress.

```
sudo apt-get install libmpdec-dev

phpize
./configure
make
sudo make install
```

You will also need to enable the extension. You can do this on temporarily using
`php -d extension=decimal.so` or by adding `extension=decimal.so` to your INI. If
you're managing your PHP installs with [phpbrew](), you can use `phpbrew ext enable decimal`.



