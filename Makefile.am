bin_PROGRAMS =
check_PROGRAMS =
noinst_PROGRAMS =
lib_LTLIBRARIES =
nobase_include_HEADERS =
pkgconfig_DATA =
BUILT_SOURCES =
TESTS =
CLEANFILES =
DISTCLEANFILES =
EXTRA_DIST =
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

EXTRA_DIST += LICENSE
EXTRA_DIST += README.md
EXTRA_DIST += CHANGELOG.md
EXTRA_DIST += ChangeLog.old

AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-I${top_srcdir}/protobuf-c \
	-I${top_builddir} \
	-I${top_srcdir}
AM_CFLAGS = ${my_CFLAGS}
AM_LDFLAGS =

# code coverage

AM_CFLAGS += ${CODE_COVERAGE_CFLAGS}
AM_LDFLAGS += ${CODE_COVERAGE_LDFLAGS}
CODE_COVERAGE_LCOV_OPTIONS = --no-external
CODE_COVERAGE_IGNORE_PATTERN = "$(abs_top_builddir)/t/*"
@CODE_COVERAGE_RULES@

#
# libprotobuf-c
#

LIBPROTOBUF_C_CURRENT=1
LIBPROTOBUF_C_REVISION=0
LIBPROTOBUF_C_AGE=0

lib_LTLIBRARIES += \
	protobuf-c/libprotobuf-c.la

nobase_include_HEADERS += \
	protobuf-c/protobuf-c.h \
	protobuf-c/protobuf-c.proto

protobuf_c_libprotobuf_c_la_SOURCES = \
	protobuf-c/protobuf-c.c \
	protobuf-c/protobuf-c.h

protobuf_c_libprotobuf_c_la_LDFLAGS = $(AM_LDFLAGS) \
	-version-info $(LIBPROTOBUF_C_CURRENT):$(LIBPROTOBUF_C_REVISION):$(LIBPROTOBUF_C_AGE) \
	-no-undefined

if HAVE_LD_VERSION_SCRIPT
protobuf_c_libprotobuf_c_la_LDFLAGS += \
    -Wl,--version-script=$(top_srcdir)/protobuf-c/libprotobuf-c.sym
else
protobuf_c_libprotobuf_c_la_LDFLAGS += \
    -export-symbols-regex "^(protobuf_c_[a-z].*)"
endif
EXTRA_DIST += protobuf-c/libprotobuf-c.sym

pkgconfig_DATA += protobuf-c/libprotobuf-c.pc
CLEANFILES += protobuf-c/libprotobuf-c.pc
EXTRA_DIST += protobuf-c/libprotobuf-c.pc.in

#
# protoc-gen-c
#

if BUILD_COMPILER

bin_PROGRAMS += protoc-gen-c/protoc-gen-c
protoc_gen_c_protoc_gen_c_SOURCES = \
	protoc-gen-c/c_bytes_field.cc \
	protoc-gen-c/c_bytes_field.h \
	protoc-gen-c/c_enum.cc \
	protoc-gen-c/c_enum.h \
	protoc-gen-c/c_enum_field.cc \
	protoc-gen-c/c_enum_field.h \
	protoc-gen-c/c_extension.cc \
	protoc-gen-c/c_extension.h \
	protoc-gen-c/c_field.cc \
	protoc-gen-c/c_field.h \
	protoc-gen-c/c_file.cc \
	protoc-gen-c/c_file.h \
	protoc-gen-c/c_generator.cc \
	protoc-gen-c/c_generator.h \
	protoc-gen-c/c_helpers.cc \
	protoc-gen-c/c_helpers.h \
	protoc-gen-c/c_message.cc \
	protoc-gen-c/c_message.h \
	protoc-gen-c/c_message_field.cc \
	protoc-gen-c/c_message_field.h \
	protoc-gen-c/c_primitive_field.cc \
	protoc-gen-c/c_primitive_field.h \
	protoc-gen-c/c_service.cc \
	protoc-gen-c/c_service.h \
	protoc-gen-c/c_string_field.cc \
	protoc-gen-c/c_string_field.h \
	protoc-gen-c/compat.h \
	protobuf-c/protobuf-c.pb.cc \
	protobuf-c/protobuf-c.pb.h \
	protoc-gen-c/main.cc
protoc_gen_c_protoc_gen_c_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	$(protobuf_CFLAGS)
protoc_gen_c_protoc_gen_c_LDADD = \
	-lprotoc \
	$(protobuf_LIBS)

protobuf-c/protobuf-c.pb.cc protobuf-c/protobuf-c.pb.h: @PROTOC@ $(top_srcdir)/protobuf-c/protobuf-c.proto
	$(AM_V_GEN)@PROTOC@ -I$(top_srcdir) --cpp_out=$(top_builddir) $(top_srcdir)/protobuf-c/protobuf-c.proto

BUILT_SOURCES += \
		 protobuf-c/protobuf-c.pb.cc \
		 protobuf-c/protobuf-c.pb.h

#
# protoc-gen-pawn
#

bin_PROGRAMS += protoc-gen-pawn/protoc-gen-pawn
protoc_gen_pawn_protoc_gen_pawn_SOURCES = \
	protoc-gen-pawn/pawn_enum.cc \
	protoc-gen-pawn/pawn_enum.h \
	protoc-gen-pawn/pawn_file.cc \
	protoc-gen-pawn/pawn_file.h \
	protoc-gen-pawn/pawn_generator.cc \
	protoc-gen-pawn/pawn_generator.h \
	protoc-gen-pawn/pawn_helpers.cc \
	protoc-gen-pawn/pawn_helpers.h \
	protoc-gen-pawn/pawn_message.cc \
	protoc-gen-pawn/pawn_message.h \
	protoc-gen-pawn/pawn_service.cc \
	protoc-gen-pawn/pawn_service.h \
	protoc-gen-pawn/compat.h \
	protoc-gen-pawn/main.cc
protoc_gen_pawn_protoc_gen_pawn_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	$(protobuf_CFLAGS)
protoc_gen_pawn_protoc_gen_pawn_LDADD = \
	-lprotoc \
	$(protobuf_LIBS)

#
# protoc-c compat link
#

install-exec-hook:
	rm -f $(DESTDIR)$(bindir)/protoc-c
	ln -s protoc-gen-c $(DESTDIR)$(bindir)/protoc-c

#
# protobuf-c tests
#

if CROSS_COMPILING
#
# skip tests on cross-compiling
#
else

LOG_COMPILER = $(VALGRIND)

check_PROGRAMS += \
	t/generated-code/test-generated-code \
	t/generated-code2/test-generated-code2 \
	t/generated-code3/test-generated-code3 \
	t/version/version

TESTS += \
	t/generated-code/test-generated-code \
	t/generated-code2/test-generated-code2 \
	t/generated-code3/test-generated-code3 \
	t/version/version

t_generated_code_test_generated_code_SOURCES = \
	t/generated-code/test-generated-code.c \
	t/test.pb-c.c
t_generated_code_test_generated_code_LDADD = \
	protobuf-c/libprotobuf-c.la

t_generated_code2_test_generated_code2_SOURCES = \
	t/generated-code2/test-generated-code2.c \
	t/test-full.pb-c.c \
	t/test-optimized.pb-c.c
t_generated_code2_test_generated_code2_LDADD = \
	protobuf-c/libprotobuf-c.la

t_generated_code3_test_generated_code3_CPPFLAGS = \
	-DPROTO3

t_generated_code3_test_generated_code3_SOURCES = \
	t/generated-code/test-generated-code.c \
	t/test-proto3.pb-c.c

t_generated_code3_test_generated_code3_LDADD = \
	protobuf-c/libprotobuf-c.la

noinst_PROGRAMS += \
	t/generated-code2/cxx-generate-packed-data

t_generated_code2_cxx_generate_packed_data_SOURCES = \
	t/generated-code2/cxx-generate-packed-data.cc \
	t/test-full.pb.cc \
	protobuf-c/protobuf-c.pb.cc
$(t_generated_code2_cxx_generate_packed_data_OBJECTS): t/test-full.pb.h
t_generated_code2_cxx_generate_packed_data_CXXFLAGS = \
	$(AM_CXXFLAGS) \
	$(protobuf_CFLAGS)
t_generated_code2_cxx_generate_packed_data_LDADD = \
	$(protobuf_LIBS)

t/test.pb-c.c t/test.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/test.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/test.proto

t/test-optimized.pb-c.c t/test-optimized.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/test-optimized.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/test-optimized.proto

t/test-full.pb-c.c t/test-full.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/test-full.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/test-full.proto

t/test-full.pb.cc t/test-full.pb.h: @PROTOC@ $(top_srcdir)/t/test-full.proto
	$(AM_V_GEN)@PROTOC@ -I$(top_srcdir) --cpp_out=$(top_builddir) $(top_srcdir)/t/test-full.proto

t/test-proto3.pb-c.c t/test-proto3.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/test-proto3.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/test-proto3.proto

t/generated-code2/test-full-cxx-output.inc: t/generated-code2/cxx-generate-packed-data$(EXEEXT)
	$(AM_V_GEN)$(top_builddir)/t/generated-code2/cxx-generate-packed-data$(EXEEXT) > $(top_builddir)/t/generated-code2/test-full-cxx-output.inc

BUILT_SOURCES += \
	t/test.pb-c.c t/test.pb-c.h \
	t/test-full.pb-c.c t/test-full.pb-c.h \
	t/test-optimized.pb-c.c t/test-optimized.pb-c.h \
	t/test-full.pb.cc t/test-full.pb.h \
	t/test-proto3.pb-c.c t/test-proto3.pb-c.h \
	t/generated-code2/test-full-cxx-output.inc

t_version_version_SOURCES = \
	t/version/version.c
t_version_version_LDADD = \
	protobuf-c/libprotobuf-c.la

# Pawn tests
TESTS += t/test-pawn-enum.sh t/test-pawn-message.sh
EXTRA_DIST += t/test-pawn-enum.sh t/test-pawn-message.sh

# Rule to generate pawn code from test_enum.proto
test_enum.pb-pawn.inc: $(top_builddir)/protoc-gen-pawn/protoc-gen-pawn$(EXEEXT) $(top_srcdir)/test_enum.proto
	$(AM_V_GEN)$(top_builddir)/protoc-gen-pawn/protoc-gen-pawn$(EXEEXT) --pawn_out=$(top_builddir) -I$(top_srcdir) $(top_srcdir)/test_enum.proto

# Rule to generate pawn code from test_message.proto
test_message.pb-pawn.inc: $(top_builddir)/protoc-gen-pawn/protoc-gen-pawn$(EXEEXT) $(top_srcdir)/test_message.proto
	$(AM_V_GEN)$(top_builddir)/protoc-gen-pawn/protoc-gen-pawn$(EXEEXT) --pawn_out=$(top_builddir) -I$(top_srcdir) $(top_srcdir)/test_message.proto

BUILT_SOURCES += test_enum.pb-pawn.inc test_message.pb-pawn.inc
CLEANFILES += test_enum.pb-pawn.inc test_message.pb-pawn.inc

# Issue #204
check_PROGRAMS += \
	t/issue204/issue204
TESTS += \
	t/issue204/issue204
t_issue204_issue204_SOURCES = \
	t/issue204/issue204.c \
	t/issue204/issue204.pb-c.c
t_issue204_issue204_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue204/issue204.pb-c.c t/issue204/issue204.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue204/issue204.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue204/issue204.proto
BUILT_SOURCES += \
	t/issue204/issue204.pb-c.c t/issue204/issue204.pb-c.h
EXTRA_DIST += \
	t/issue204/issue204.proto

# Issue #220
check_PROGRAMS += \
	t/issue220/issue220
TESTS += \
	t/issue220/issue220
t_issue220_issue220_SOURCES = \
	t/issue220/issue220.c \
	t/issue220/issue220.pb-c.c
t_issue220_issue220_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue220/issue220.pb-c.c t/issue220/issue220.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue220/issue220.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue220/issue220.proto
BUILT_SOURCES += \
	t/issue220/issue220.pb-c.c t/issue220/issue220.pb-c.h
EXTRA_DIST += \
	t/issue220/issue220.proto

# Issue #251
check_PROGRAMS += \
	t/issue251/issue251
TESTS += \
	t/issue251/issue251
t_issue251_issue251_SOURCES = \
	t/issue251/issue251.c \
	t/issue251/issue251.pb-c.c
t_issue251_issue251_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue251/issue251.pb-c.c t/issue251/issue251.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue251/issue251.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue251/issue251.proto
BUILT_SOURCES += \
	t/issue251/issue251.pb-c.c t/issue251/issue251.pb-c.h
EXTRA_DIST += \
	t/issue251/issue251.proto

# Issue #330
check_PROGRAMS += \
	t/issue330/issue330
TESTS += \
	t/issue330/issue330
t_issue330_issue330_SOURCES = \
	t/issue330/issue330.c \
	t/issue330/issue330.pb-c.c
t_issue330_issue330_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue330/issue330.pb-c.c t/issue330/issue330.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue330/issue330.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue330/issue330.proto
BUILT_SOURCES += \
	t/issue330/issue330.pb-c.c t/issue330/issue330.pb-c.h
EXTRA_DIST += \
	t/issue330/issue330.proto

t_issue330_issue330_SOURCES += \
	t/issue389/issue389.pb-c.c # Tack onto issue330 since there is no need for a separate binary here
t/issue389/issue389.pb-c.c t/issue389/issue389.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue389/issue389.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue389/issue389.proto
BUILT_SOURCES += \
	t/issue389/issue389.pb-c.c t/issue389/issue389.pb-c.h
EXTRA_DIST += \
	t/issue389/issue389.proto

# Issue #375
check_PROGRAMS += \
	t/issue375/issue375
TESTS += \
	t/issue375/issue375
t_issue375_issue375_SOURCES = \
	t/issue375/issue375.c \
	t/issue375/issue375.pb-c.c
t_issue375_issue375_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue375/issue375.pb-c.c t/issue375/issue375.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue375/issue375.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue375/issue375.proto
BUILT_SOURCES += \
	t/issue375/issue375.pb-c.c t/issue375/issue375.pb-c.h
EXTRA_DIST += \
	t/issue375/issue375.proto

# Issue #440
check_PROGRAMS += \
	t/issue440/issue440
TESTS += \
	t/issue440/issue440
t_issue440_issue440_SOURCES = \
	t/issue440/issue440.c \
	t/issue440/issue440.pb-c.c
t_issue440_issue440_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue440/issue440.pb-c.c t/issue440/issue440.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue440/issue440.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue440/issue440.proto
BUILT_SOURCES += \
	t/issue440/issue440.pb-c.c t/issue440/issue440.pb-c.h
EXTRA_DIST += \
	t/issue440/issue440.proto

# Issue #745
check_PROGRAMS += \
	t/issue745/issue745
TESTS += \
	t/issue745/issue745
t_issue745_issue745_SOURCES = \
	t/issue745/issue745.c \
	t/issue745/issue745.pb-c.c
t_issue745_issue745_LDADD = \
	protobuf-c/libprotobuf-c.la
t/issue745/issue745.pb-c.c t/issue745/issue745.pb-c.h: $(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) $(top_srcdir)/t/issue745/issue745.proto
	$(AM_V_GEN)@PROTOC@ --plugin=protoc-gen-c=$(top_builddir)/protoc-gen-c/protoc-gen-c$(EXEEXT) -I$(top_srcdir) --c_out=$(top_builddir) $(top_srcdir)/t/issue745/issue745.proto
BUILT_SOURCES += \
	t/issue745/issue745.pb-c.c t/issue745/issue745.pb-c.h
EXTRA_DIST += \
	t/issue745/issue745.proto

endif # CROSS_COMPILING

endif # BUILD_COMPILER

EXTRA_DIST += \
	t/test.proto \
	t/test-full.proto \
	t/test-optimized.proto \
	t/test-proto3.proto \
	t/generated-code2/common-test-arrays.h

#
#
#

CLEANFILES += $(BUILT_SOURCES)

dist-hook:
	rm -f `find $(distdir) -name '*.pb-c.[ch]' -o -name '*.pb.cc' -o -name '*.pb.h'`

install-data-hook:
	$(MKDIR_P) $(DESTDIR)$(includedir)/google/protobuf-c
	cd $(DESTDIR)$(includedir)/google/protobuf-c && rm -f protobuf-c.h
	cd $(DESTDIR)$(includedir)/google/protobuf-c && $(LN_S) ../../protobuf-c/protobuf-c.h protobuf-c.h

#
# documentation
#

if HAVE_DOXYGEN
stamp-html: $(DOXYGEN_INPUT_FILES) $(top_builddir)/Doxyfile $(top_srcdir)/DoxygenLayout.xml $(include_HEADERS) $(nobase_include_HEADERS)
	$(AM_V_GEN) $(DOXYGEN)
	@touch $@
html-local: stamp-html

clean-local:
	rm -rf $(top_builddir)/html $(top_builddir)/stamp-html
endif

EXTRA_DIST += Doxyfile.in
EXTRA_DIST += DoxygenLayout.xml
EXTRA_DIST += build-cmake/CMakeLists.txt
EXTRA_DIST += build-cmake/Config.cmake.in
