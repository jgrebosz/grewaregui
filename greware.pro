#-------------------------------------------------
#
# Project created by QtCreator 2010-12-17 T12:19:05
#
#-------------------------------------------------

TARGET = greware
TEMPLATE = app

# here below should be > 4 because since ver 5. some include files are not
# anymore in subdirectory <QtGui/...>
greaterThan(QT_MAJOR_VERSION,4): QT += widgets


#=========================================================
# when DEBUG ---- uncomment this 2 lines below
# CONFIG  += warn_on  static debug   # <-- this DEBUG is relevant (1)
# QMAKE_CXXFLAGS += -g        # <-- -g is must be on for debug  (2)

#when RELEASE    ===========================================
 CONFIG  += warn_on  static release

#    (static is probably for pictures (loaded into program))

#=========================================================
#-L /opt/Qt5.12.9/5.12.9/gcc_64/lib
#=========================================================

#QMAKE_INCDIR = /usr/include/qt5/QtWidgets/

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS +=  -Wno-ctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op
QMAKE_CXXFLAGS += -Wno-old-style-cast
#QMAKE_CXXFLAGS +=-Wno-sign-conversion
QMAKE_CXXFLAGS +=-Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-format-nonliteral
QMAKE_CXXFLAGS += -Wmissing-declarations
#QMAKE_CXXFLAGS += -Wall -Wextra -Wcast-align -Wcast-qual
#QMAKE_CXXFLAGS += -Wmissing-include-dirs -Wnoexcept
#QMAKE_CXXFLAGS +=-Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-null-sentinel -Wswitch-default -Wundef -Werror -Wno-unused
#QMAKE_CXXFLAGS +=-Wstrict-overflow=5  -Wsign-promo

# -Weffc++

#target.path = ./grewareGUI/
#INSTALLS += target

SOURCES += main.cxx\
        appl_form.cxx \
    paths.cpp \
    options.cpp \
    spectrum_2D.cpp \
    spectrum_1D.cpp \
    box_of_spectrum.cpp \
    box_of_matrix.cpp \
    box_of_counts.cpp \
    box_of_channels.cpp \
    swiat.cpp \
    gate_1D.cpp \
    Tpinup.cpp \
    t4markers_manager.cpp \
    tundo.cpp \
    polygon_gate.cpp \
    tplate_spectrum.cpp \
    Tfile_helper.cpp \
    t4results_of_integration.cpp \
    t4sum_spectra_dialog.cpp \
    t4select_spectra.cpp \
    t4setting_paths_dlg.cpp \
    t4size_spectrum_dlg.cpp \
    t4setting_times_dlg.cpp \
    t4binning_change_dlg.cpp \
    t4merge_2d_spectra_dlg.cpp \
    t4about_dlg.cpp \
    t4welcome.cpp \
    t4info_dlg.cpp \
    t4auto_calib_wizard.cpp \
    t4manager_user_spectra.cpp \
    t4user_spectrum_wizard.cpp \
    user_spectrum_description.cpp \
    t4incrementer_selector.cpp \
    t4manager_user_conditions.cpp \
    t4user_condition_wizard.cpp \
    Tcondition_description_cracow.cpp \
    t4addline_1d_wizard.cpp \
    t4important_dlg.cxx \
    t4collect_ntuple.cxx \
    t4addback_dlg.cxx \
    t4zet_aoq_ranges.cpp \
    t4verbose_dlg.cpp \
    t4good_minibal.cpp \
    t4doppler_dlg.cpp \
    t4other_spy_options.cpp \
    t4good_ger_dlg.cpp \
    t4good_hector_dlg.cpp \
    t4lycca_parameters.cpp \
    t4active_stopper_dlg.cpp \
    t4manager_user_incrementers.cpp \
    Tincrementer_description_cracow.cpp \
    t4user_incrementer_wizard.cpp \
    Tself_gate_abstract_descr.cpp \
    Tself_gate_ger_descr.cpp \
    Tself_gate_paris_descr.cpp \
    t4options_visibility.cpp \
    t4selfgate_ger_dlg.cpp \
    t4selfgate_choice_dlg.cpp \
    t4picture_angle.cpp \
    tselfgate_type.cxx \
    t4selfgate_hec_dlg.cpp \
    t4selfgate_paris_dlg.cpp \
   t4selfgate_kratta_2dlg.cpp \
    Tself_gate_mib_descr.cpp \
	 Tself_gate_hec_descr_simple.cpp \
    Tself_gate_kratta_descr.cpp \
    Tself_gate_ger_addback_descr.cpp \
    Tself_gate_agata_psa_descr.cxx \
    t4selfgate_miniball_dlg.cpp \
    t4selfgate_geraddback_dlg.cpp \
    t4selfgate_agata_psa_dlg.cpp \
    t4pattern_defining_dlg.cpp \
    t4selfgate_selector.cpp \
    t4update_info_dlg.cpp \
    tgui_options.cpp \
    t4interesting_ranges_galileo.cpp \
    t4good_galileo_ge_dlg.cpp \
    t4selfgate_ge_crystal_galileo.cpp \
    Tself_gate_ge_galileo_descr.cpp \
    tgalileo_signals_dlg.cpp \
    tcalibration_factors_dlg.cpp \
    Tfile_line_det_cal_lookup.cpp \
    t4good_kratta_dlg.cpp \
    t4general_selector.cpp \
    t4tiling_marix_dlg.cpp
#    t4checking_incrementers_dlg.cpp


HEADERS  += appl_form.h \
    options.h \
    paths.h \
    spectrum_2D.h \
    spectrum_1D.h \
    box_of_spectrum.h \
    box_of_matrix.h \
    box_of_counts.h \
    box_of_channels.h \
    swiat.h \
    gate_1D.h \
    Tpinup.h \
    t4markers_manager.h \
    tundo.h \
    polygon_gate.h \
    tplate_spectrum.h \
    Tfile_helper.h \
    t4results_of_integration.h \
    t4sum_spectra_dialog.h \
    t4select_spectra.h \
    t4setting_paths_dlg.h \
    t4size_spectrum_dlg.h \
    t4setting_times_dlg.h \
    t4binning_change_dlg.h \
    t4merge_2d_spectra_dlg.h \
    t4about_dlg.h \
    t4welcome.h \
    t4info_dlg.h \
    t4auto_calib_wizard.h \
    t4manager_user_spectra.h \
    t4user_spectrum_wizard.h \
    user_spectrum_description.h \
    t4incrementer_selector.h \
    t4manager_user_conditions.h \
    t4user_condition_wizard.h \
    Tcondition_description_cracow.h \
    t4addline_1d_wizard.h \
    t4important_dlg.h \
    t4collect_ntuple.h \
    t4addback_dlg.h \
    t4zet_aoq_ranges.h \
    t4verbose_dlg.h \
    t4good_minibal.h \
    t4doppler_dlg.h \
    t4other_spy_options.h \
    t4good_ger_dlg.h \
    t4good_hector_dlg.h \
    t4lycca_parameters.h \
    t4active_stopper_dlg.h \
    t4manager_user_incrementers.h \
    t4user_incrementer_wizard.h \
    Tself_gate_abstract_descr.h \
    Tself_gate_ger_descr.h \
    Tself_gate_paris_descr.h \
    t4options_visibility.h \
    t4selfgate_ger_dlg.h \
    t4selfgate_choice_dlg.h \
    t4picture_angle.h \
    tselfgate_type.h \
    t4selfgate_hec_dlg.h \
    t4selfgate_paris_dlg.h \
    t4selfgate_kratta_2dlg.h \
    Tself_gate_mib_descr.h \
	 Tself_gate_hec_descr_simple.h \
    Tself_gate_kratta_descr.h \
    Tself_gate_ger_addback_descr.h \
    Tself_gate_agata_psa_descr.h \
    t4selfgate_miniball_dlg.h \
    t4selfgate_geraddback_dlg.h \
    t4selfgate_agata_psa_dlg.h \
    t4pattern_defining_dlg.h \
    t4selfgate_selector.h \
    t4update_info_dlg.h \
    tgui_options.h \
    t4interesting_ranges_galileo.h \
    t4good_galileo_ge_dlg.h \
    t4selfgate_ge_crystal_galileo.h \
    Tself_gate_ge_galileo_descr.h \
    tgalileo_signals_dlg.h \
    tcalibration_factors_dlg.h \
    Tfile_line_det_cal_lookup.h \
    t4good_kratta_dlg.h \
    t4general_selector.h \
    t4tiling_marix_dlg.h \
#    t4checking_incrementers_dlg.h


FORMS    += appl_form.ui \
    t4markers_manager.ui \
    tplate_spectrum.ui \
    t4results_of_integration.ui \
    t4sum_spectra_dialog.ui \
    t4select_spectra.ui \
    t4setting_paths_dlg.ui \
    t4size_spectrum_dlg.ui \
    t4setting_times_dlg.ui \
    t4binning_change_dlg.ui \
    t4merge_2d_spectra_dlg.ui \
    t4about_dlg.ui \
    t4welcome.ui \
    t4info_dlg.ui \
    t4auto_calib_wizard.ui \
    t4manager_user_spectra.ui \
    t4user_spectrum_wizard.ui \
    t4incrementer_selector.ui \
    t4manager_user_conditions.ui \
    t4user_condition_wizard.ui \
    t4addline_1d_wizard.ui \
    t4important_dlg.ui \
    t4collect_ntuple.ui \
    t4addback_dlg.ui \
    t4zet_aoq_ranges.ui \
    t4verbose_dlg.ui \
    t4good_minibal.ui \
    t4doppler_dlg.ui \
    t4other_spy_options.ui \
    t4good_ger_dlg.ui \
    t4good_hector_dlg.ui \
    t4lycca_parameters.ui \
    t4active_stopper_dlg.ui \
    t4manager_user_incrementers.ui \
    t4user_incrementer_wizard.ui \
    t4options_visibility.ui \
    t4selfgate_ger_dlg.ui \
    t4selfgate_choice_dlg.ui \
    t4picture_angle.ui \
    t4selfgate_hec_dlg.ui \
    t4selfgate_paris_dlg.ui \
    t4selfgate_miniball_dlg.ui \
    t4selfgate_geraddback_dlg.ui \
    t4selfgate_agata_psa_dlg.ui \
    t4selfgate_selector.ui \
    t4pattern_defining_dlg.ui \
    t4update_info_dlg.ui \
    tgui_options.ui \
    t4interesting_ranges_galileo.ui \
    t4good_galileo_ge_dlg.ui \
    t4selfgate_ge_crystal_galileo.ui \
    tgalileo_signals_dlg.ui \
    tcalibration_factors_dlg.ui \
    t4selfgate_kratta_2dlg.ui \
    t4good_kratta_dlg.ui \
    t4general_selector.ui \
    t4tiling_marix_dlg.ui \
#    t4checking_incrementers_dlg.ui
    t4checking_incrementers_dlg.ui


OTHER_FILES +=

RESOURCES += \
    greware_icons.qrc

DISTFILES += \
    info_to_remember.txt







