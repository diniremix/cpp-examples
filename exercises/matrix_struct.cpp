#include <fmt/core.h>
#include <vector>

struct Matrix {
    // Usamos un array de arrays para representar la matriz
    std::vector<std::vector<float>> data;

    int rows() const
    {
        return data.size();
    }

    int cols() const
    {
        return data[0].size();
    }

    bool is_symmetric() const
    {
        if (this->rows() != this->cols()) {
            return false; // no es cuadrada
        }

        int _rows = this->rows();
        int _cols = this->cols();

        for (int i = 0; i < _rows; ++i) {
            for (int j = i + 1; j < _rows; ++j) {
                // Usar tolerancia para floats
                // if (std::abs(data[i][j] - data[j][i]) > 1e-6)
                if (this->data[i][j] != this->data[j][i]) {
                    return false; // No es simétrica
                }
            }
        }
        return true;
    }

    bool triangular_inequality() const
    {
        int _rows = this->rows();

        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _rows; ++j) {
                for (int k = 0; k < _rows; ++k) {
                    if (this->data[i][j] > this->data[i][k] + this->data[k][j]) {
                        fmt::println("Violación en: d({},{}) + d({},{}) = {} + {} = {}", i, k, k, j, // ← Corregido
                                     data[i][k], data[k][j], data[i][k] + data[k][j]);

                        fmt::println("La desigualdad triangular no se cumple para los puntos {}, {} y {}", i + 1, j + 1,
                                     k + 1);
                        return false; // No cumple
                    }
                }
            }
        }
        return true;
    }
};

int main()
{
    Matrix matrices[] = {
        // 4x4
        {{{0.0, 7.74, 12.5, 17.06}, {6.51, 0.0, 5.47, 10.03}, {13.27, 6.95, 0.0, 3.5}, {16.62, 10.29, 6.0, 0.0}}},
        // Válida 4x4
        {{{0.0, 3.5, 7.0, 10.5}, {3.5, 0.0, 3.5, 7.0}, {7.0, 3.5, 0.0, 3.5}, {10.5, 7.0, 3.5, 0.0}}},
        // 5x5
        {{{0.0, 4.2, 8.4, 12.6, 16.8},
          {4.2, 0.0, 4.2, 8.4, 12.6},
          {8.4, 4.2, 0.0, 4.2, 8.4},
          {12.6, 8.4, 4.2, 0.0, 4.2},
          {16.8, 12.6, 8.4, 4.2, 0.0}}},
        // Válida 4x4
        {{{0, 3, 6, 9}, {3, 0, 3, 6}, {6, 3, 0, 3}, {9, 6, 3, 0}}},
        // 4x3
        {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}}},
        // 5x3
        {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}, {13, 14, 15}}},
        // 5x4
        {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}, {17, 18, 19, 20}}},
        // 5x5
        {{{0, 4, 8, 12, 16}, {4, 0, 4, 8, 12}, {8, 4, 0, 4, 8}, {12, 8, 4, 0, 4}, {16, 12, 8, 4, 0}}}};

    for (const auto& mat : matrices) {
        fmt::println("========");
        fmt::println("rows: {}", mat.rows());
        fmt::println("cols: {}", mat.cols());

        // Verificar simetría
        if (mat.is_symmetric()) {
            fmt::println("La matriz es simétrica.");
        } else {
            fmt::println("La matriz no es simétrica.");
        }

        // Verificar desigualdad triangular
        if (mat.triangular_inequality()) {
            fmt::println("La matriz cumple con la desigualdad triangular.");
        } else {
            fmt::println("La matriz no cumple con la desigualdad triangular.");
        }
    }

    return 0;
}
