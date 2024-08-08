#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(const std::vector<int>& values, QWidget* parent) : QDialog(parent), ui(new Ui::Dialog())
{
	ui->setupUi(this);

	QObject::connect(ui->okBtn, &QPushButton::clicked, this, &QDialog::accept);
	QObject::connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
	QObject::connect(ui->okBtn, &QPushButton::clicked, this, &SettingsDialog::emit_settings);

	this->values = values;
	le_vector = std::vector<QLineEdit*>({ ui->iters_le , ui->wheel_le , ui->sizex_le, ui->sizey_le, 
		ui->init_hue_le , ui->hue_cycle_le , ui->hue_dir_le, ui->s_le, ui->v_le});
	for (int i = 0; i != values.size(); ++i)
		le_vector[i]->setText(QString::number(values[i]));
	ranges = std::vector<std::vector<int>>({ {0, 1000}, {1010, 10000}, {50, 3000}, {50, 3000}, 
		{0, 359}, {1, 1000}, {0, 1}, {0, 255}, {0, 255}});
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::emit_settings()
{
	bool ok;
	QString s;
	int n;
	std::vector<int> new_values(values.size());
	for (int i = 0; i != values.size(); ++i)
	{
		s = le_vector[i]->text();
		n = s.toInt(&ok);
		if (!ok)
			new_values[i] = values[i];
		else if (n < ranges[i][0])
			new_values[i] = ranges[i][0];
		else if (n > ranges[i][1])
			new_values[i] = ranges[i][1];
		else
			new_values[i] = n;
	}
	emit settings_signal(new_values);
}