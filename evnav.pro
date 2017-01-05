TEMPLATE = subdirs

SUBDIRS += 3rdparty
SUBDIRS += libevnav
SUBDIRS += example
SUBDIRS += evnav-cli
SUBDIRS += evnav-srv
SUBDIRS += tests

libevnav.depends = 3rdparty
example.depends = libevnav
evnav-cli.depends = libevnav
evnav-srv.depends = libevnav
tests.depends = libevnav

