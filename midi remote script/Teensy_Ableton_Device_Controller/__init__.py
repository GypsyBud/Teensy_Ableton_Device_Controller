# emacs-mode: -*- python-*-
# -*- coding: utf-8 -*-

import Live
from Teensy_Ableton_Device_Controller import Teensy_Ableton_Device_Controller

def create_instance(c_instance):
    ' Creates and returns the APC20 script '
    return Teensy_Ableton_Device_Controller(c_instance)


# local variables:
# tab-width: 4
