#include "PreferencesDialog.h"


PreferencesDialog::PreferencesDialog(ProjectModel * projectModel, ModelManipulator * manipulator, QWidget * parent) : QDialog(parent)
{
	this->manipulator = manipulator;

	mainLayout.reset(new QVBoxLayout(this));

	// Name preference
	nameLayout.reset(new QHBoxLayout());
	nameLabel.reset(new QLabel("Project name: "));
	nameEdit.reset(new QLineEdit("NewProject", this));
	nameLayout->addWidget(nameLabel.data());
	nameLayout->addWidget(nameEdit.data());
	if (projectModel->getName().size())
		nameEdit->setText(QString::fromStdString(projectModel->getName()));
	mainLayout->addLayout(nameLayout.data());

	// Buttons
	buttonsLayout.reset(new QHBoxLayout());
	okButton.reset(new QPushButton("OK", this));
	connect(okButton.data(), SIGNAL(clicked()), this, SLOT(savePreferencesClicked()));
	cancelButton.reset(new QPushButton("Cancel", this));
	connect(cancelButton.data(), SIGNAL(clicked()), this, SLOT(reject()));

	// Connecting all preferences
	buttonsLayout->addWidget(okButton.data());
	buttonsLayout->addWidget(cancelButton.data());
	mainLayout->addLayout(buttonsLayout.data());

	// Window settings
	setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
	setLayout(mainLayout.data());
	setModal(true);
	setWindowTitle("Project properties");
	setWindowIcon(QIcon(":/images/gear.png"));
}


PreferencesDialog::~PreferencesDialog()
{
}


void PreferencesDialog::savePreferencesClicked()
{
	manipulator->setProjectName(nameEdit->text().toStdString());
	accept();
}