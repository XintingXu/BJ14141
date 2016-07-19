#ifndef ARDUINO_YEELINKLIB_DEVICE_H
#define ARDUINO_YEELINKLIB_DEVICE_H

class yl_device
{
public:
	yl_device(long id = 0);

	void set_id(long id);
	long get_id() const;

private:
	long id_;
};


#endif
