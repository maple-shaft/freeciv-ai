## Process this file with automake to produce Makefile.in
lib_LTLIBRARIES = fc_ai_maple.la

ACLOCAL_AMFLAGS = -I m4

COMMON_CPPFLAGS = \
-I$(FCBLDPATH)/gen_headers \
-I${FCSRCPATH}/gen_headers/enums \
-I$(FCSRCPATH)/utility \
-I$(FCSRCPATH)/common \
-I$(FCSRCPATH)/common/networking \
-I$(FCSRCPATH)/common/aicore \
-I$(FCSRCPATH)/ai \
-I$(FCSRCPATH)/server \
-I$(FCSRCPATH)/server/advisors \
-I$(FCSRCPATH)/ai/default \
 $(CURL_CFLAGS) $(JANSSON_CFLAGS)

AM_CPPFLAGS = $(COMMON_CPPFLAGS)

fc_ai_maple_la_LDFLAGS = -module
fc_ai_maple_la_LIBADD = -lm $(CURL_LIBS) $(JANSSON_LIBS)

fc_ai_maple_la_SOURCES = \
maple_ai_city.h \
maple_ai_data.h \
maple_ai_diplomacy.h \
maple_ai_domestic.h \
maple_ai_effects.h \
maple_ai_hand.h \
maple_ai_military.h \
maple_ai_player.h \
maple_ai_settler.h \
maple_ai_tech.h \
maple_ai_tools.h \
maple_ai_unit.h \
ai_stub.h \
serialize.h \
agent.h \
maple_ai_city.c \
maple_ai_data.c \
maple_ai_diplomacy.c \
maple_ai_domestic.c \
maple_ai_effects.c \
maple_ai_hand.c \
maple_ai_military.c \
maple_ai_player.c \
maple_ai_settler.c \
maple_ai_tech.c \
maple_ai_tools.c \
maple_ai_unit.c \
ai_stub.c \
serialize.c \
agent.c

noinst_PROGRAMS = test_maple_ai_serialize test_maple_ai_agent

TEST_CPPFLAGS = \
-Itests \
$(COMMON_CPPFLAGS)

test_maple_ai_serialize_CPPFLAGS = $(TEST_CPPFLAGS)
test_maple_ai_agent_CPPFLAGS = $(TEST_CPPFLAGS)

TEST_SOURCES = \
tests/unity.h \
tests/unity.c

test_maple_ai_serialize_SOURCES = \
$(TEST_SOURCES) \
serialize.h \
serialize.c \
tests/test_serialize.c

test_maple_ai_agent_SOURCES = \
$(TEST_SOURCES) \
agent.h \
agent.c \
tests/test_agent.c

TEST_LDADD = \
$(FCBLDPATH)/common/libfreeciv.la \
$(FCBLDPATH)/dependencies/cvercmp/libcvercmp.la \
fc_ai_maple.la \
$(CURL_LIBS) $(JANSSON_LIBS) -licuuc -licudata -llzma -L/usr/lib -lsqlite3 -lm -lz

test_maple_ai_serialize_LDADD = $(TEST_LDADD)
test_maple_ai_agent_LDADD = $(TEST_LDADD)