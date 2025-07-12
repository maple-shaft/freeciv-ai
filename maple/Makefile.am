## Process this file with automake to produce Makefile.in

aimodule_LTLIBRARIES = fc_ai_maple.la

AM_CPPFLAGS = \
-I$(FCBLDPATH)/gen_headers \
-I${FCSRCPATH}/gen_headers/enums \
-I$(FCSRCPATH)/utility \
-I$(FCSRCPATH)/common \
-I$(FCSRCPATH)/common/networking \
-I$(FCSRCPATH)/common/aicore \
-I$(FCSRCPATH)/ai \
-I$(FCSRCPATH)/server \
-I$(FCSRCPATH)/server/advisors \
-I$(FCSRCPATH)/ai/default

fc_ai_maple_la_SOURCES = \
ai_stub.h \
ai_stub.c 

fc_ai_maple_la_LDFLAGS = -module
fc_ai_maple_la_LIBADD = -lm