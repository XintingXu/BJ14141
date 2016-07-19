#include "yl_device.h"

yl_device::yl_device(long id /* = 0 */)
: id_(id)
{}

void yl_device::set_id(long id)
{
	id_ = id;
}

long yl_device::get_id() const
{
	return id_;
}

