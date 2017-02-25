"""This module is intended to process temperature measurements in different units"""


def F_to_C(tf):
    """Convert degrees of Fahrenheit to degrees of Celsius. Argument: tf -- temperature in Fahrenheit """
    return 5*(tf-32)/9

def average_C_temp(data):
    """Calculate average in degrees of Celsius of many measurements in deg. F and deg. C.
    Argument: data -- list of measurements. Each measurement is a string of the form '15.3C',
    meaning 15.3 deg. C or '28F' meaning 28.0 deg.
    F. Returns: tc -- Average temperature in degrees of Celsius as a float value. """
    tmp = 0
    for t in data:
        value, unit = t[:-1], t[-1]
        tmp += {
            'c': float(value),
            'F': F_to_C(float(value))
        }[unit]
    return tmp


if __name__ == "__main__":
    print(F_to_C(30))
    print(average_C_temp(["30F","20c","10c","100c"]))
