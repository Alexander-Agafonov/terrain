#include <fstream>
#include <iostream>
#include <cmath>
#include "interface_noise_layers.h"

#define NOISE_LAYERS 10
#define PGM_16_BIT 65535


interface_noise_layers :: interface_noise_layers(QWidget* parent) : QWidget(parent) {

    this -> setSizePolicy(QSizePolicy :: Expanding, QSizePolicy :: Fixed);

    box = new QVBoxLayout(this);
    box -> setSpacing(15);
    box -> setAlignment(Qt :: AlignTop);

    noise_layers_label = new QLabel("Noise Layers", this);
    noise_layers_label -> setAlignment(Qt::AlignCenter);
    noise_layers_label -> setFixedHeight(30);
    noise_layers_label -> setStyleSheet("QLabel { color: rgba(190, 190, 222, 1); background-color: rgba(30, 33, 39, 1); height: 30px; margin: 0; }");

    box -> addWidget(noise_layers_label);

    for (int i = 0; i < NOISE_LAYERS; i++) {
        layers_vector.push_back(new interface_noise_layer(this, i + 1));
        box -> addWidget(layers_vector[i]);
    }

    alpha_map = new noise_layer;

    layers_vector[0] -> activate_layer();
    update_new_layer_button();

    layers_vector[1] -> activate_layer();
    update_new_layer_button();
    layers_vector[1] -> frequency_spinbox -> setValue(3);

    layers_vector[2] -> activate_layer();
    update_new_layer_button();
    layers_vector[2] -> frequency_spinbox -> setValue(5);

    layers_vector[3] -> activate_layer();
    update_new_layer_button();
    layers_vector[3] -> frequency_spinbox -> setValue(10);

    layers_vector[4] -> activate_layer();
    update_new_layer_button();
    layers_vector[4] -> frequency_spinbox -> setValue(60);
    layers_vector[4] -> amplitude_spinbox -> setValue(4);

//    layers_vector[5] -> activate_layer();
//    update_new_layer_button();
//    layers_vector[5] -> frequency_spinbox -> setValue(60);
//    layers_vector[5] -> amplitude_spinbox -> setValue(20);

//    layers_vector[6] -> activate_layer();
//    update_new_layer_button();
//    layers_vector[6] -> frequency_spinbox -> setValue(80);
//    layers_vector[6] -> amplitude_spinbox -> setValue(10);

//    layers_vector[7] -> activate_layer();
//    update_new_layer_button();
//    layers_vector[7] -> frequency_spinbox -> setValue(200);
//    layers_vector[7] -> amplitude_spinbox -> setValue(5);


}


void interface_noise_layers :: build_layers(int width, int height, bool random) {

    std :: vector < std :: vector <int> > heightmap(height, std :: vector <int> (width, 0));


    int ctr = 0;

    for (int i = 0; i < NOISE_LAYERS; i++) {
        if (layers_vector[i] -> active) {
            ctr ++;
            layers_vector[i] -> noise -> create_layer(width, height,
                layers_vector[i] -> frequency_spinbox -> value() + 1, layers_vector[i] -> frequency_spinbox -> value() + 1,
                layers_vector[i] -> amplitude_spinbox -> value(), random);
//            layers_vector[i] -> noise -> create_layer(width, height,
//            width * layers_vector[i] -> frequency_spinbox -> value(), height * layers_vector[i] -> frequency_spinbox -> value(), 1);
//            layers_vector[i] -> noise

            for (int k = 0; k < height; k++) {
                for (int x = 0; x < width; x++) {
                    heightmap[k][x] += (layers_vector[i] -> noise -> heightmap[k][x] * layers_vector[i] -> transparency_spinbox -> value());
                   //std :: cout << heightmap[k][x] << "hmap val\n";
                }
            }
        }
    }


//    float min = -1;
//    float max = 100000;

//    for (int i = 0; i < height; i++) {
//        for (int k = 0; k < width; k++) {
//            if (heightmap[i][k] < min) min = heightmap[i][k];
//            if (heightmap[i][k] > max) max = heightmap[i][k];
//        }
//    }
    alpha_map -> create_layer(width, height, 9, 9, 200, 200);
    float min = 10000;
    float max = 0;

    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            if (alpha_map -> heightmap[i][k] < min) min = alpha_map -> heightmap[i][k];
            if (alpha_map -> heightmap[i][k] > max) max = alpha_map -> heightmap[i][k];
        }
    }


    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            alpha_map -> heightmap[i][k] -= min;
        }
    }

    max -= min;

    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            alpha_map -> heightmap[i][k] /= max;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++ ) {
//            heightmap[i][k] *= ((max - min) / 65535);
            heightmap[i][k] = floor(heightmap[i][k]);
            //heightmap[i][k] *= alpha_map -> heightmap[i][k];
        }
    }






//    for (int k = 0; k < height; k++) {
//        for (int x = 0; x < width; x++) {
//            heightmap[k][x] = heightmap[k][x];
//           //std :: cout << heightmap[k][x] << "hmap val\n";
//        }
//    }

        std :: ofstream happy_file;
        happy_file.open("../terrain/heightmap2.pgm");
        happy_file << "P2\n";
        happy_file << width << ' ' << height << '\n';
        happy_file << "65535\n";

        for (int i = 0; i < static_cast <int> (heightmap.size()); i++) {
            for (int k = 0; k < static_cast <int> (heightmap[0].size()); k++) {
                if (k == static_cast <int> (heightmap[0].size()) - 1) happy_file << heightmap[i][k] << '\n';
                else happy_file << heightmap[i][k] << ' ';
            }
        }
        happy_file.close();


}


void interface_noise_layers :: update_new_layer_button() {

    sort_layers();

    if (!layers_vector[1] -> active) layers_vector[0] -> delete_layer_button -> setEnabled(false);
    else layers_vector[0] -> delete_layer_button -> setEnabled(true);

    for (int i = 1; i < NOISE_LAYERS; i++) {
        if (layers_vector[i - 1] -> active && !layers_vector[i] -> active) {
            layers_vector[i] -> display_button();
            for (int k = i + 1; k < NOISE_LAYERS; k++) layers_vector[k] -> deactivate_layer();
            return;
        }
    }
}


void interface_noise_layers :: sort_layers() {

    for (int i = 0; i < NOISE_LAYERS; i++) {
        box -> removeWidget(layers_vector[i]);
        if (i < (NOISE_LAYERS - 1) && !layers_vector[i] -> active && layers_vector[i + 1] -> active) {
            layers_vector[NOISE_LAYERS] = layers_vector[i];
            layers_vector[i] = layers_vector[i + 1];
            layers_vector[i + 1] = layers_vector[NOISE_LAYERS];
        }
    }

    for (int i = 0; i < NOISE_LAYERS; i++) {
        layers_vector[i] -> layer_label -> setText(QString("L" + QString :: number(i + 1)));
        box -> addWidget(layers_vector[i]);
    }
}
