/********************************************************************************
** Form generated from reading UI file 't4markers_manager_dlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_T4MARKERS_MANAGER_DLG_H
#define UI_T4MARKERS_MANAGER_DLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label;
    QListView *listView_sets;
    QGridLayout *gridLayout;
    QPushButton *pushButton_remove;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_apply;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(400, 300);
        gridLayout_2 = new QGridLayout(Dialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        listView_sets = new QListView(Dialog);
        listView_sets->setObjectName(QString::fromUtf8("listView_sets"));

        gridLayout_2->addWidget(listView_sets, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton_remove = new QPushButton(Dialog);
        pushButton_remove->setObjectName(QString::fromUtf8("pushButton_remove"));

        gridLayout->addWidget(pushButton_remove, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        pushButton_apply = new QPushButton(Dialog);
        pushButton_apply->setObjectName(QString::fromUtf8("pushButton_apply"));

        gridLayout->addWidget(pushButton_apply, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 1);

        buttonBox = new QDialogButtonBox(Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 3, 0, 1, 1);


        retranslateUi(Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "List of currently stored sets of markers", nullptr));
        pushButton_remove->setText(QCoreApplication::translate("Dialog", "Remove", nullptr));
        pushButton_apply->setText(QCoreApplication::translate("Dialog", "Apply", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_T4MARKERS_MANAGER_DLG_H
