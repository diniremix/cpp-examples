/*
 * Perceptrón y fundamentos de Redes Neuronales.
 *
 * Se implementó desde cero una neurona logística (perceptrón con activación
 * sigmoide) utilizando C++ 17+, sin librerías de Machine Learning.
 *
 * Este proyecto busca comprender el funcionamiento interno de una neurona,
 * el descenso por gradiente y la construcción manual de pequeñas redes
 * neuronales sin Backpropagation.
 * Cada perceptrón se entrena de forma completamente independiente, la neurona
 * XOR no aprende directamente las entradas originales, sino las
 * salidas producidas por la capa oculta (OR y NAND). Esto demuestra cómo una
 * representación intermedia puede convertir un problema no linealmente
 * separable en uno separable utilizando varios perceptrones.
 *
 * Algunos hitos
 * - Este ejemplo NO implementa Backpropagation
 * - XOR no puede aprenderse mediante un único perceptrón.
 * - OR y NAND generan una representación donde XOR sí es separable.
 * - No existe propagación del error entre capas
 * - Entrenamiento independiente de un perceptrón
 * - Inicialización aleatoria de pesos y bias
 * - Función de activación Sigmoide
 * - Descenso por gradiente
 * - Aprendizaje de compuertas lógicas:
 *      AND
 *      OR
 *      NAND
 * - Construcción manual de XOR utilizando:
 * ```
 *   OR --------\
 *               \
 *                XOR
 *               /
 *   NAND ------/
 * ```
 * - Perceptrón
 * - Neurona logística
 * - Función sigmoide
 * - Bias
 * - Pesos sinápticos
 * - Descenso por gradiente
 * - Error de clasificación
 * - Dataset de entrenamiento
 * - Dataset de prueba
 * - Shuffle de muestras
 * - Forward Pass
 * - Separabilidad lineal
 * - Composición manual de una red neuronal
 */

#include <algorithm>
#include <ctime>
#include <iostream>
#include <math.h>
#include <random>
#include <stdlib.h>
#include <string>
#include <vector>

#define EPOCA 1000     // Numero de iteraciones del entrenamiento
#define K 0.2f         // Taza de aprendizaje
#define THRESHOLD 0.5f // Umbral
using namespace std;

struct Perceptron {
    float pesos[2];
    float bias;
    float error_total;
};

struct Sample {
    float x0;
    float x1;
    float target;
};

/// Inicia los pesos aleatoriamente
void pesos_init(Perceptron& p)
{
    // Perceptron p_and{.bias = 1.0f};
    p.bias = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

    for (int i = 0; i < 2; i++) {
        p.pesos[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    }
}

/// Funcion sigmoide
float sigmoide(float s)
{
    return (1 / (1 + exp(-1 * s)));
}

// funcion de entrenamiento
float TrainNetwork(Perceptron& p, float x0, float x1, float target)
{
    float net = 0;
    float out = 0;
    float delta[2]; // Es la variacion de los pesos sinapticos
    float Error;

    // net = p.pesos[0] * x0 + p.pesos[1] * x1 + p.bias;
    net = p.pesos[0] * x0 + p.pesos[1] * x1 - p.bias;
    out = sigmoide(net);

    Error = target - out;

    // Derivada de la sigmoide
    float gradiente = out * (1.0f - out);

    // Actualización del bias
    // p.bias += K * Error * gradiente;
    p.bias -= K * Error * gradiente;

    // Actualización de los pesos
    delta[0] = K * Error * gradiente * x0;
    delta[1] = K * Error * gradiente * x1;

    p.pesos[0] += delta[0];
    p.pesos[1] += delta[1];

    // cout << "error=" << Error << " grad=" << gradiente << " dw0=" << delta[0]
    //<< " dw1=" << delta[1] << endl;
    p.error_total += fabs(Error);

    return out;
}

/// Funcion que entrega las salidas
float RunNetwork(Perceptron& p, float x0, float x1)
{
    float net = 0;
    float out = 0;

    // net = p.pesos[0] * x0 + p.pesos[1] * x1 + p.bias;
    net = p.pesos[0] * x0 + p.pesos[1] * x1 - p.bias;
    net = sigmoide(net);

    out = net;
    return out;
}

/// funcion de entrenamiento XOR
void TrainNetwork_Xor(Perceptron& p_or, Perceptron& p_nand, Perceptron& p_xor, std::vector<Sample>& data,
                      bool early_stop = false)
{
    cout << "=====Entrenando XOR=====" << endl;

    vector<Sample> xor_resp_data;

    // Generamos los datos de entrenamiento usando las salidas de OR
    float or_00 = RunNetwork(p_or, 0, 0);
    float or_01 = RunNetwork(p_or, 0, 1);
    float or_10 = RunNetwork(p_or, 1, 0);
    float or_11 = RunNetwork(p_or, 1, 1);

    // Generamos los datos de entrenamiento usando las salidas de NAND
    float nand_00 = RunNetwork(p_nand, 0, 0);
    float nand_01 = RunNetwork(p_nand, 0, 1);
    float nand_10 = RunNetwork(p_nand, 1, 0);
    float nand_11 = RunNetwork(p_nand, 1, 1);

    xor_resp_data.push_back({or_00, nand_00, 0}); // XOR(0,0) → 0
    xor_resp_data.push_back({or_01, nand_01, 1}); // XOR(0,1) → 1
    xor_resp_data.push_back({or_10, nand_10, 1}); // XOR(1,0) → 1
    xor_resp_data.push_back({or_11, nand_11, 0}); // XOR(1,1) → 0

    cout << "\nDatos de entrenamiento para XOR (entrada: OR, NAND):" << endl;
    for (const auto& s : xor_resp_data) {
        cout << "(" << s.x0 << ", " << s.x1 << ") → " << s.target << endl;
    }

    // Entrenar XOR
    int i = 0;
    std::mt19937 rng(std::random_device{}());

    while (i < EPOCA) {
        i++;
        p_xor.error_total = 0;

        // cambiar los datos de orden antes de entrenar
        std::shuffle(xor_resp_data.begin(), xor_resp_data.end(), rng);

        for (const auto& s : xor_resp_data) {
            TrainNetwork(p_xor, s.x0, s.x1, s.target);
        }

        if (early_stop) {
            // Verificar si aprendió
            bool aprendio = true;

            for (const auto& s : xor_resp_data) {
                float out = RunNetwork(p_xor, s.x0, s.x1);
                bool predict = out > THRESHOLD;

                if (predict != (bool)s.target) {
                    aprendio = false;
                    break;
                }
            }

            if (aprendio) {
                cout << "clasificacion para XOR aprendida en la época " << i << endl;
                break;
            }
        }
    }

    cout << endl;
    cout << "Salida XOR" << endl;
    cout << "K = " << K << endl;
    cout << "Peso[0] = " << p_xor.pesos[0] << endl;
    cout << "Peso[1] = " << p_xor.pesos[1] << endl;
    cout << "bias = " << p_xor.bias << endl;
    cout << "Error(epoca: " << i << ") = " << p_xor.error_total << endl;

    cout << endl;
    cout << "Test XOR" << endl;
    /// Se hace la prueba para ver el estado
    /// del entrenamiento
    for (const auto& s : data) {
        float s_or = RunNetwork(p_or, s.x0, s.x1);
        float s_nand = RunNetwork(p_nand, s.x0, s.x1);

        cout << "run: (" << s.x0 << "," << s.x1 << "): " << RunNetwork(p_xor, s_or, s_nand) << endl;
    }
}

/// Entrena una neurona logística (perceptrón con activación sigmoide)
/// mediante descenso por gradiente sobre un conjunto de muestras.
/// Esta función entrena únicamente una neurona independiente.
/// No propaga errores entre capas y, por tanto, no implementa
/// Backpropagation.
void train_perceptron(std::string layer_name, Perceptron& p, std::vector<Sample>& data, bool early_stop = false)
{
    cout << "====train_perceptron fn " << layer_name << "====" << endl;

    int i = 0;
    std::mt19937 rng(std::random_device{}());

    // Entrenando la funcion logica
    while (i < EPOCA) {
        i++;
        p.error_total = 0;

        // cambiar los datos de orden antes de entrenar
        std::shuffle(data.begin(), data.end(), rng);

        for (const auto& s : data) {
            TrainNetwork(p, s.x0, s.x1, s.target);
        }

        if (early_stop) {
            // revisar en que epoca aprende y detenerse
            bool aprendio = true;

            for (const auto& s : data) {
                float out = RunNetwork(p, s.x0, s.x1);
                bool predict = out > THRESHOLD; // la clasificacion supera el umbral para AND

                if (predict != (bool)s.target) {
                    aprendio = false;
                    break;
                }
            }

            // cout << "Epoca " << i << "  Error=" << p_and.error_total << endl;

            if (aprendio) {
                cout << "clasificacion para " << layer_name << " aprendida en la epoca " << i << endl;
                break;
            }
        }
    } // while

    cout << endl;
    cout << "Salida " << layer_name << endl;
    cout << "K = " << K << endl;
    cout << "Peso[0] = " << p.pesos[0] << endl;
    cout << "Peso[1] = " << p.pesos[1] << endl;
    cout << "bias = " << p.bias << endl;
    cout << "Error(epoca: " << i << ") = " << p.error_total << endl;
    cout << endl;

    cout << "Test " << layer_name << endl;
    /// Se hace la prueba para ver el estado
    /// del entrenamiento
    for (const auto& s : data) {
        cout << "run: (" << s.x0 << "," << s.x1 << "): " << RunNetwork(p, s.x0, s.x1) << endl;
    }

} // train_perceptron

/// main
int main()
{
    srand(time(nullptr));

    Perceptron p_and{};
    Perceptron p_or{};
    Perceptron p_nand{};
    Perceptron p_xor{};

    pesos_init(p_and);
    pesos_init(p_or);
    pesos_init(p_nand);
    pesos_init(p_xor);

    // AND
    std::vector<Sample> and_data = {{1, 1, 1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 0}};

    //  OR
    std::vector<Sample> or_data = {{1, 1, 1}, {1, 0, 1}, {0, 1, 1}, {0, 0, 0}};

    //  NAND
    std::vector<Sample> nand_data = {{1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {0, 0, 1}};

    //  XOR
    std::vector<Sample> xor_data = {{1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {0, 0, 0}};

    train_perceptron("AND", p_and, and_data);
    //
    train_perceptron("OR", p_or, or_data);
    //
    train_perceptron("NAND", p_nand, nand_data);
    //
    cout << endl;
    //
    // Entrenamos XOR usando las salidas de OR y NAND
    TrainNetwork_Xor(p_or, p_nand, p_xor, xor_data);

    return 0;
}
