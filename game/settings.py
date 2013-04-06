
######################################################################
# Evennia MU* server configuration file
#
# You may customize your setup by copy&pasting the variables you want
# to change from the master config file src/settings_default.py to
# this file. Try to *only* copy over things you really need to customize
# and do *not* make any changes to src/settings_default.py directly.
# This way you'll always have a sane default to fall back on
# (also, the master config file may change with server updates).
#
######################################################################

from src.settings_default import *

######################################################################
# Custom settings
######################################################################


######################################################################
# SECRET_KEY was randomly seeded when settings.py was first created.
# Don't share this with anybody. Warning: if you edit SECRET_KEY
# *after* creating any accounts, your users won't be able to login,
# since SECRET_KEY is used to salt passwords.
######################################################################
SECRET_KEY = 'h$Cp"}V5-dL^.1E+3eH4Mf<72Q,SWk(q?l_aRO>Z'

