#include "settingsdialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidgetItem>
#include <QString>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>

QGroupBox* SettingsDialog::createSizeGroup()
{
	auto layout = new QVBoxLayout();
	auto preset_combo = new QComboBox(this);
	auto size_layout = new QHBoxLayout();
	auto width_edit = new QSpinBox(this);
	auto height_edit = new QSpinBox(this);
	auto wxh_label = new QLabel(QString("x"), this);
	auto count_edit = new QSpinBox(this);
	
	size_layout->addItem(new QWidgetItem(width_edit));
	size_layout->addItem(new QWidgetItem(wxh_label));
	size_layout->addItem(new QWidgetItem(height_edit));
	size_layout->setStretch(0, 10);
	size_layout->setStretch(2, 10);
	layout->addItem(new QWidgetItem(preset_combo));
	layout->addLayout(size_layout);
	layout->addItem(new QWidgetItem(count_edit));
	
	preset_combo->addItems(Settings::sizesPresetList());
	preset_combo->setCurrentIndex(m_settings->sizesPreset());
	width_edit->setMinimum(2);
	width_edit->setMaximum(99);
	height_edit->setMinimum(2);
	height_edit->setMaximum(99);
	count_edit->setMinimum(1);
	count_edit->setMaximum(m_settings->width() * m_settings->height() - 1);
	width_edit->setValue(m_settings->width());
	height_edit->setValue(m_settings->height());
	count_edit->setValue(m_settings->mineCount());
	
	auto updateE = [=](){
		m_settings->setSize(width_edit->value(), height_edit->value());
		count_edit->setMinimum(1);
		count_edit->setMaximum(width_edit->value() * height_edit->value() - 1);
		m_settings->setMineCount(count_edit->value());
		preset_combo->blockSignals(true);
		preset_combo->setCurrentIndex(m_settings->sizesPreset());
		preset_combo->blockSignals(false);
	};
	auto updateC = [=](){
		width_edit->blockSignals(true);
		height_edit->blockSignals(true);
		count_edit->blockSignals(true);
		m_settings->setSizesPreset(Settings::SizesPreset(preset_combo->currentIndex()));
		count_edit->setMinimum(1);
		count_edit->setMaximum(m_settings->width() * m_settings->height() - 1);
		width_edit->setValue(m_settings->width());
		height_edit->setValue(m_settings->height());
		count_edit->setValue(m_settings->mineCount());
		width_edit->blockSignals(false);
		height_edit->blockSignals(false);
		count_edit->blockSignals(false);
	};
	
	connect(width_edit , QOverload<int>::of(&QSpinBox::valueChanged), updateE);
	connect(height_edit, QOverload<int>::of(&QSpinBox::valueChanged), updateE);
	connect(count_edit , QOverload<int>::of(&QSpinBox::valueChanged), updateE);
	connect(preset_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), updateC);
	
	QGroupBox* ret = new QGroupBox(QString("Size and mine count"), this);
	ret->setLayout(layout);
	return ret;
}

QGroupBox* SettingsDialog::createMinegenGroup()
{
	auto layout = new QVBoxLayout();
	auto pattern_label = new QLabel(QString("Pattern:"), this);
	auto pattern_combo = new QComboBox(this);
	auto blueness_label = new QLabel(QString("Pattern strength:"));
	auto blueness_slider = new QSlider(Qt::Horizontal, this);
	
	layout->addItem(new QWidgetItem(pattern_label));
	layout->addItem(new QWidgetItem(pattern_combo));
	layout->addItem(new QWidgetItem(blueness_label));
	layout->addItem(new QWidgetItem(blueness_slider));
	layout->addStretch();
	
	pattern_combo->addItems(Settings::kernelList());
	pattern_combo->setCurrentIndex(m_settings->kernel());
	blueness_slider->setRange(0, 100);
	blueness_slider->setTickPosition(QSlider::NoTicks);
	blueness_slider->setValue(0.5 + m_settings->blueness() * blueness_slider->maximum());
	
	auto update = [=](){
		m_settings->setKernel(Settings::Kernel(pattern_combo->currentIndex()));
		m_settings->setBlueness((double)blueness_slider->value() / blueness_slider->maximum());
	};
	
	connect(pattern_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), update);
	connect(blueness_slider, &QSlider::valueChanged, update);
	
	QGroupBox* ret = new QGroupBox("Mine generation", this);
	ret->setLayout(layout);
	return ret;
}

QWidget* SettingsDialog::createMineGenerationPage()
{
	auto layout = new QVBoxLayout();
	layout->addItem(new QWidgetItem(createSizeGroup()));
	layout->addItem(new QWidgetItem(createMinegenGroup()));
	
	QWidget* ret = new QWidget(this);
	ret->setLayout(layout);
	return ret;
}

QWidget* SettingsDialog::createGameplayPage()
{
	auto layout = new QVBoxLayout();
	auto colorscheme_label = new QLabel("Colorscheme:", this);
	auto colorscheme_combo = new QComboBox(this);
	auto middle_flags = new QCheckBox("Middle click places flags", this);
	auto relative_counts = new QCheckBox("Relative mine counts", this);
	auto right_as_middle = new QCheckBox("Right click acts as middle", this);
	
	layout->addItem(new QWidgetItem(colorscheme_label));
	layout->addItem(new QWidgetItem(colorscheme_combo));
	layout->addItem(new QWidgetItem(middle_flags));
	layout->addItem(new QWidgetItem(relative_counts));
	layout->addItem(new QWidgetItem(right_as_middle));
	layout->addStretch();
	
	colorscheme_combo->addItems(Settings::colorschemeList());
	colorscheme_combo->setCurrentIndex(m_settings->colorscheme());
	middle_flags->setChecked(m_settings->middleClickPlacesFlags());
	relative_counts->setChecked(m_settings->relativeMineCounts());
	right_as_middle->setChecked(m_settings->rightClickAsMiddle());
	
	auto update = [=](){
		m_settings->setColorscheme(Settings::Colorscheme(colorscheme_combo->currentIndex()));
		m_settings->setMiddleClickPlacesFlags(middle_flags->checkState() == Qt::Checked);
		m_settings->setRelativeMineCounts(relative_counts->checkState() == Qt::Checked);
		m_settings->setRightClickAsMiddle(right_as_middle->checkState() == Qt::Checked);
	};
	
	connect(colorscheme_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), update);
	connect(middle_flags   , &QCheckBox::stateChanged, update);
	connect(relative_counts, &QCheckBox::stateChanged, update);
	connect(right_as_middle, &QCheckBox::stateChanged, update);
	
	QWidget* ret = new QWidget(this);
	ret->setLayout(layout);
	return ret;
}

QTabWidget* SettingsDialog::createTabWidget()
{
	QTabWidget* ret = new QTabWidget(this);
	ret->addTab(createMineGenerationPage(), QString("Mine generation"));
	ret->addTab(createGameplayPage(), QString("Gameplay"));
	return ret;
}

SettingsDialog::SettingsDialog(Settings* settings, QWidget *parent)
	: QDialog(parent)
	, m_settings(new Settings(*settings))
	, m_out_settings(settings)
{
	setWindowTitle(QString("qMineSeek Settings"));
	
	auto layout = new QVBoxLayout(this);
	auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	
	layout->addItem(new QWidgetItem(createTabWidget()));
	layout->addItem(new QWidgetItem(buttonBox));
	
	layout->setSizeConstraint( QLayout::SetFixedSize );
	auto accepted = [=](){
		*m_out_settings = *m_settings;
	};
	connect(this, &SettingsDialog::accepted, accepted);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
}

SettingsDialog::~SettingsDialog()
{
	delete m_settings;
}
