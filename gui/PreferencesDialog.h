#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include "MainWindow.h"
#include "DataflowModel.h"
#include "ModelManipulator.h"

#include <QDialog>
#include <QtWidgets>

class PreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    PreferencesDialog(ProjectModel * projectModel, ModelManipulator * manipulator, QWidget * parent);
    ~PreferencesDialog();

private slots:
    virtual void savePreferencesClicked();

private:
    ModelManipulator * manipulator;

    QScopedPointer<QVBoxLayout> mainLayout;

    QScopedPointer<QHBoxLayout> nameLayout;
    QScopedPointer<QLabel> nameLabel;
    QScopedPointer<QLineEdit> nameEdit;

    QScopedPointer<QHBoxLayout> buttonsLayout;
    QScopedPointer<QPushButton> okButton;
    QScopedPointer<QPushButton> cancelButton;

};

#endif
