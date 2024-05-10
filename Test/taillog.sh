# For debugging: print the end of syslog without the other stuff spammed to it
tail -100 /var/log/syslog | grep "student kernel"
