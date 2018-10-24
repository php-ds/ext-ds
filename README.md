PHP Decimal
===

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



