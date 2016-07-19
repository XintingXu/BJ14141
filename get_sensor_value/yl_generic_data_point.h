#ifndef ARDUINO_YEELINKLIB_GENERICDATAPOint_H
#define ARDUINO_YEELINKLIB_GENERICDATAPOint_H

#include "yl_data_point.h"

class yl_generic_data_point : public yl_data_point
{
public:
	yl_generic_data_point(const String &key);

protected:
	virtual String key_to_string() const;
	virtual bool key_from_string(const String &str);

};

#endif
