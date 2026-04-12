#ifndef DS_SEQ_HANDLERS_H
#define DS_SEQ_HANDLERS_H

#include "php.h"

extern zend_object_handlers php_seq_handlers;

void php_ds_register_seq_handlers();

#endif
