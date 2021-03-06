#include "interface_hydro_parameters_menu.h"

#include <QApplication>
#include <QString>

#include <iostream>


hydro_parameters_menu :: hydro_parameters_menu(QWidget *parent) : QWidget(parent) {

    h_box = new QHBoxLayout(this);
    v_box = new QVBoxLayout();
    v_box -> setSpacing(15);
    v_box -> setAlignment(Qt :: AlignTop);
    grid = new QGridLayout();

    hydro_parameters_label = new QLabel("Hydraulic Erosion Parameters", this);
    hydro_parameters_label -> setAlignment(Qt::AlignCenter);
    hydro_parameters_label -> setFixedHeight(30);
    hydro_parameters_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");

    random_checkbox = new QCheckBox(this);

    random_label = new QLabel("Water Drops", this);
    random_label -> setFixedHeight(30);
    random_label -> setFixedWidth(160);
    random_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; padding-left: 8px; padding-right: 8px; }");
    random_label -> setAlignment(Qt::AlignCenter);

    cycles_slider = new QSlider(Qt :: Horizontal, this);
    cycles_slider -> setFixedHeight(30);
    cycles_slider -> setRange(10, 1000);
    cycles_slider -> setValue(300);

    cycles_spinbox = new QSpinBox(this);
    cycles_spinbox -> setFixedWidth(54);
    cycles_spinbox -> setRange(10, 1000);
    cycles_spinbox -> setValue(300);
    cycles_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    cycles_label = new QLabel("Number of Cycles", this);
    cycles_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    cycles_label -> setAlignment(Qt::AlignCenter);
    cycles_label -> setFixedHeight(30);

    water_amount_slider = new interface_double_slider(Qt :: Horizontal, this);
    water_amount_slider -> setFixedHeight(30);
    water_amount_slider -> setMaximum(100);
    water_amount_slider -> setValue(20);
    water_amount_slider -> setMinimum(1);

    water_spinbox = new QDoubleSpinBox(this);
    water_spinbox -> setMinimum(0.01);
    water_spinbox -> setMaximum(1.00);
    water_spinbox -> setValue(0.2);
    water_spinbox -> setSingleStep(0.01);
    water_spinbox -> setFixedWidth(54);
    water_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    water_amount_label = new QLabel("Water Amount", this);
    water_amount_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    water_amount_label -> setAlignment(Qt::AlignCenter);
    water_amount_label -> setFixedHeight(30);

    carrying_capacity_slider = new interface_double_slider(Qt :: Horizontal, this);
    carrying_capacity_slider -> setFixedHeight(30);
    carrying_capacity_slider -> setMaximum(10);
    carrying_capacity_slider -> setValue(6);
    carrying_capacity_slider -> setMinimum(1);

    carry_spinbox = new QDoubleSpinBox(this);
    carry_spinbox -> setMinimum(0.01);
    carry_spinbox -> setMaximum(0.1);
    carry_spinbox -> setValue(0.06);
    carry_spinbox -> setSingleStep(0.01);
    carry_spinbox -> setFixedWidth(54);
    carry_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    carrying_capacity_label = new QLabel("Carrying Capacity", this);
    carrying_capacity_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    carrying_capacity_label -> setAlignment(Qt::AlignCenter);
    carrying_capacity_label -> setFixedHeight(30);

    evaporation_rate_slider = new interface_double_slider(Qt :: Horizontal, this);
    evaporation_rate_slider -> setFixedHeight(30);
    evaporation_rate_slider -> setRange(1, 100);
    evaporation_rate_slider -> setValue(30);
    evaporation_rate_slider -> setMinimumHeight(30);

    evaporation_spinbox = new QDoubleSpinBox(this);
    evaporation_spinbox -> setMinimum(0.01);
    evaporation_spinbox -> setMaximum(1.00);
    evaporation_spinbox -> setValue(0.3);
    evaporation_spinbox -> setSingleStep(0.01);
    evaporation_spinbox -> setFixedWidth(54);
    evaporation_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    evaporation_rate_label = new QLabel("Evaporation Rate", this);
    evaporation_rate_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    evaporation_rate_label -> setAlignment(Qt::AlignCenter);
    evaporation_rate_label -> setFixedHeight(30);

    post_evaporation_rate_slider = new interface_double_slider(Qt :: Horizontal, this);
    post_evaporation_rate_slider -> setFixedHeight(30);
    post_evaporation_rate_slider -> setRange(1, 100);
    post_evaporation_rate_slider -> setValue(50);
    post_evaporation_rate_slider -> setMinimumHeight(30);

    post_evaporation_rate_label = new QLabel("Post Evaporation Rate", this);
    post_evaporation_rate_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    post_evaporation_rate_label -> setAlignment(Qt::AlignCenter);
    post_evaporation_rate_label -> setFixedHeight(30);

    post_evaporation_spinbox = new QDoubleSpinBox(this);
    post_evaporation_spinbox -> setMinimum(0.01);
    post_evaporation_spinbox -> setMaximum(1.00);
    post_evaporation_spinbox -> setValue(0.5);
    post_evaporation_spinbox -> setSingleStep(0.01);
    post_evaporation_spinbox -> setFixedWidth(54);
    post_evaporation_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    erosion_rate_slider = new interface_double_slider(Qt :: Horizontal, this);
    erosion_rate_slider -> setFixedHeight(30);
    erosion_rate_slider -> setMaximum(10);
    erosion_rate_slider -> setValue(4);
    erosion_rate_slider -> setMinimum(1);

    erosion_spinbox = new QDoubleSpinBox(this);
    erosion_spinbox -> setMinimum(0.01);
    erosion_spinbox -> setMaximum(0.10);
    erosion_spinbox -> setValue(0.04);
    erosion_spinbox -> setSingleStep(0.01);
    erosion_spinbox -> setFixedWidth(54);
    erosion_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    erosion_rate_label = new QLabel("Erosion Rate", this);
    erosion_rate_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    erosion_rate_label -> setAlignment(Qt::AlignCenter);
    erosion_rate_label -> setFixedHeight(30);

    deposition_rate_slider = new interface_double_slider(Qt :: Horizontal, this);
    deposition_rate_slider -> setFixedHeight(30);
    deposition_rate_slider -> setValue(1);
    deposition_rate_slider -> setMaximum(10);
    deposition_rate_slider -> setMinimum(1);

    deposition_spinbox = new QDoubleSpinBox(this);
    deposition_spinbox -> setMinimum(0.01);
    deposition_spinbox -> setMaximum(0.10);
    deposition_spinbox -> setValue(0.01);
    deposition_spinbox -> setSingleStep(0.01);
    deposition_spinbox -> setFixedWidth(54);
    deposition_spinbox -> setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    deposition_rate_label = new QLabel("Deposition Rate", this);
    deposition_rate_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");
    deposition_rate_label -> setAlignment(Qt::AlignCenter);
    deposition_rate_label -> setFixedHeight(30);

    // sorry
    connect(random_checkbox, SIGNAL(stateChanged(int)), this, SLOT(erosion_parameters_changed()));

    connect(cycles_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(cycles_spinbox, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(cycles_slider, SIGNAL(valueChanged(int)), cycles_spinbox, SLOT(setValue(int)));
    connect(cycles_spinbox, SIGNAL(valueChanged(int)), cycles_slider, SLOT(setValue(int)));

    connect(water_amount_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(water_spinbox, SIGNAL(valueChanged(double)), this, SLOT(erosion_parameters_changed()));
    connect(water_amount_slider, SIGNAL(value_changed_double_signal(double)), water_spinbox, SLOT(setValue(double)));
    connect(water_spinbox, SIGNAL(valueChanged(double)), water_amount_slider, SLOT(set_double_value(double)));

    connect(carrying_capacity_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(carry_spinbox, SIGNAL(valueChanged(double)), this, SLOT(erosion_parameters_changed()));
    connect(carrying_capacity_slider, SIGNAL(value_changed_double_signal(double)), carry_spinbox, SLOT(setValue(double)));
    connect(carry_spinbox, SIGNAL(valueChanged(double)), carrying_capacity_slider, SLOT(set_double_value(double)));

    connect(evaporation_rate_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(evaporation_spinbox, SIGNAL(valueChanged(double)), this, SLOT(erosion_parameters_changed()));
    connect(evaporation_rate_slider, SIGNAL(value_changed_double_signal(double)), evaporation_spinbox, SLOT(setValue(double)));
    connect(evaporation_spinbox, SIGNAL(valueChanged(double)), evaporation_rate_slider, SLOT(set_double_value(double)));

    connect(post_evaporation_rate_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(post_evaporation_spinbox, SIGNAL(valueChanged(double)), this, SLOT(erosion_parameters_changed()));
    connect(post_evaporation_rate_slider, SIGNAL(value_changed_double_signal(double)), post_evaporation_spinbox, SLOT(setValue(double)));
    connect(post_evaporation_spinbox, SIGNAL(valueChanged(double)), post_evaporation_rate_slider, SLOT(set_double_value(double)));

    connect(erosion_rate_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(erosion_spinbox, SIGNAL(valueChanged(double)), erosion_spinbox, SLOT(setValue(double)));
    connect(erosion_rate_slider, SIGNAL(value_changed_double_signal(double)), erosion_spinbox, SLOT(setValue(double)));
    connect(erosion_spinbox, SIGNAL(valueChanged(double)), erosion_rate_slider, SLOT(set_double_value(double)));

    connect(deposition_rate_slider, SIGNAL(valueChanged(int)), this, SLOT(erosion_parameters_changed()));
    connect(deposition_spinbox, SIGNAL(valueChanged(double)), this, SLOT(erosion_parameters_changed()));
    connect(deposition_rate_slider, SIGNAL(value_changed_double_signal(double)), deposition_spinbox, SLOT(setValue(double)));
    connect(deposition_spinbox, SIGNAL(valueChanged(double)), deposition_rate_slider, SLOT(set_double_value(double)));

    connect(this, SIGNAL(eroded_heightmap_valid_signal()), parent, SLOT(eroded_heightmap_valid()));

    hydraulic_erosion = new hydro();

    eroded_heightmap = new eroded_heightmap_preview(this);

    grid -> addWidget(cycles_label, 0, 0, 1, 1);
    grid -> addWidget(cycles_slider, 0, 1, 1, 1);
    grid -> addWidget(cycles_spinbox, 0, 2, 1, 1);

    grid -> addWidget(water_amount_label, 1, 0, 1, 1);
    grid -> addWidget(water_amount_slider, 1, 1, 1, 1);
    grid -> addWidget(water_spinbox, 1, 2, 1, 1);

    grid -> addWidget(carrying_capacity_label, 2, 0, 1, 1);
    grid -> addWidget(carrying_capacity_slider, 2, 1, 1, 1);
    grid -> addWidget(carry_spinbox, 2, 2, 1, 1);

    grid -> addWidget(erosion_rate_label, 3, 0, 1, 1);
    grid -> addWidget(erosion_rate_slider, 3, 1, 1, 1);
    grid -> addWidget(erosion_spinbox, 3, 2, 1, 1);

    grid -> addWidget(deposition_rate_label, 4, 0, 1, 1);
    grid -> addWidget(deposition_rate_slider, 4, 1, 1, 1);
    grid -> addWidget(deposition_spinbox, 4, 2, 1, 1);

    grid -> addWidget(evaporation_rate_label, 5, 0, 1, 1);
    grid -> addWidget(evaporation_rate_slider, 5, 1, 1, 1);
    grid -> addWidget(evaporation_spinbox, 5, 2, 1, 1);

    grid -> addWidget(post_evaporation_rate_label, 6, 0, 1, 1);
    grid -> addWidget(post_evaporation_rate_slider, 6, 1, 1, 1);
    grid -> addWidget(post_evaporation_spinbox, 6, 2, 1, 1);

    grid -> addWidget(random_label, 7, 0, 1, 1);
    grid -> addWidget(random_checkbox, 7, 1, 1, 1);

    v_box -> addWidget(hydro_parameters_label);
    v_box -> addLayout(grid);
    h_box -> addLayout(v_box);
    h_box -> addWidget(eroded_heightmap);

}


void hydro_parameters_menu :: original_heightmap_invalid() {

    eroded_heightmap -> ready = false;
    eroded_heightmap -> reload_button -> setEnabled(false);
    eroded_heightmap -> reload_button -> setText("Please reload the original heightmap first");
    eroded_heightmap -> reload_button -> setStyleSheet("QPushButton { height: 30px; background: rgba(110, 110, 135, 1); border: 0; margin: 0; border-radius: 6px; font-size: 11px; color: rgba(40, 44, 52, 1); } "
                                                       "QPushButton:pressed { background: rgba(110, 110, 135, 1); } ");
}


void hydro_parameters_menu :: original_heightmap_changed() {

    eroded_heightmap -> ready = false;
    eroded_heightmap -> reload_button -> setEnabled(true);
    eroded_heightmap -> reload_button -> setText("Original heightmap changed, press to erode again");
    eroded_heightmap -> reload_button -> setStyleSheet("QPushButton { height: 30px; background: rgba(190, 190, 222, 1); border: 0; margin: 0; border-radius: 6px; font-size: 11px; color: rgba(40, 44, 52, 1); } "
                                                       "QPushButton:pressed { background: rgba(110, 110, 135, 1); } ");
}


void hydro_parameters_menu :: erosion_parameters_changed() {

    if (eroded_heightmap -> reload_button -> isEnabled() || eroded_heightmap -> reload_button -> text().toStdString() == "Currently up-to-date") {

        eroded_heightmap -> reload_button -> setEnabled(true);
        eroded_heightmap -> reload_button -> setText("Erosion parameters changed, press to erode again");
        eroded_heightmap -> reload_button -> setStyleSheet("QPushButton { height: 30px; background: rgba(190, 190, 222, 1); border: 0; margin: 0; border-radius: 6px; font-size: 11px; color: rgba(40, 44, 52, 1); } "
                                                           "QPushButton:pressed { background: rgba(110, 110, 135, 1); } ");
    }
}


void hydro_parameters_menu :: erode_heightmap() {

    eroded_heightmap -> reload_button -> setEnabled(false);
    eroded_heightmap -> reload_button -> setText("Erosion in progress, please wait");
    eroded_heightmap -> reload_button -> setStyleSheet("QPushButton { height: 30px; background: rgba(225, 185, 185, 1); border: 0; margin: 0; border-radius: 6px; font-size: 11px; color: rgba(40, 44, 52, 1); } "
                                                       "QPushButton:pressed { background: rgba(110, 110, 135, 1); } ");

    qApp -> processEvents();

    eroded_heightmap -> setEnabled(true);

    hydraulic_erosion -> set_parameters(water_spinbox -> value(), carry_spinbox -> value(), erosion_spinbox -> value(), deposition_spinbox -> value(), evaporation_spinbox -> value(), post_evaporation_spinbox -> value(), random_checkbox -> isChecked());

    hydraulic_erosion -> erode(cycles_slider -> value());

    eroded_heightmap -> reload_heightmap();

    emit eroded_heightmap_valid_signal();

}


void hydro_parameters_menu :: eroded_heightmap_valid() { emit eroded_heightmap_valid_signal(); }
