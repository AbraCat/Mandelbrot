#pragma once
#include "ui_SettingsDialog.h"
#include <vector>
#include <qdialog.h>
class SettingsDialog : public QDialog
{
	Q_OBJECT
public:
	SettingsDialog(const std::vector<int>& values, QWidget* parent = nullptr);
	~SettingsDialog();
public slots:
	void emit_settings();
signals:
	void settings_signal(const std::vector<int>& values);
private:
	Ui::Dialog* ui;
	std::vector<int> values;
	std::vector<QLineEdit*> le_vector;
	std::vector<std::vector<int>> ranges;
};

