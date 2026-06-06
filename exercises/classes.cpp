#include <fmt/core.h>
#include <format>
#include <iostream>
#include <memory> // smart pointers (RAII)
#include <stdexcept>
#include <string>
#include <vector>

// Una clase abstracta no puede instanciarse.
// Contiene al menos una función virtual pura.

// clase base abstracta
class MetodoPago
{
  protected:
    std::string id_;
    double monto_;

  public:
    MetodoPago(std::string id, double monto) : id_{std::move(id)}, monto_{monto}
    {
        if (monto_ <= 0) {
            throw std::invalid_argument("El monto debe ser positivo");
        }
    }

    // Destructor virtual (para polimorfismo)
    virtual ~MetodoPago() = default;

    // Método puro virtual (= 0) → clase abstracta
    [[nodiscard]]
    virtual std::string procesar() const = 0;

    // Método virtual con implementación por defecto
    [[nodiscard]]
    virtual std::string obtenerDetalles() const
    {
        return std::format("ID: {}, Monto: ${:.2f}", id_, monto_);
    }

    // Getters
    [[nodiscard]]
    const std::string& getId() const
    {
        return id_;
    }
    [[nodiscard]]
    double getMonto() const
    {
        return monto_;
    }
};

//
class TarjetaCredito : public MetodoPago
{
  private:
    std::string numeroTarjeta_; // últimos 4 dígitos
    std::string titular_;

  public:
    TarjetaCredito(std::string id, double monto, std::string numeroTarjeta, std::string titular)
        : MetodoPago{std::move(id), monto} // llamar constructor base
          ,
          numeroTarjeta_{std::move(numeroTarjeta)}, titular_{std::move(titular)}
    {
    }

    // Implementar método puro virtual (override explícito)
    [[nodiscard]]
    std::string procesar() const override
    {
        return std::format("[Tarjeta] Procesando ${:.2f} para {} (****{})", monto_, titular_,
                           numeroTarjeta_.substr(numeroTarjeta_.size() - 4));
    }

    // Sobrescribir método con implementación por defecto
    [[nodiscard]]
    std::string obtenerDetalles() const override
    {
        return std::format("{}\nTitular: {}, Tarjeta: ****{}",
                           MetodoPago::obtenerDetalles(), // llamar método base
                           titular_, numeroTarjeta_.substr(numeroTarjeta_.size() - 4));
    }
};

//
class PayPal : public MetodoPago
{
  private:
    std::string email_;

  public:
    PayPal(std::string id, double monto, std::string email) : MetodoPago{std::move(id), monto}, email_{std::move(email)}
    {
    }

    [[nodiscard]]
    std::string procesar() const override
    {
        return std::format("[PayPal] Enviando ${:.2f} a {}", monto_, email_);
    }

    [[nodiscard]]
    std::string obtenerDetalles() const override
    {
        return std::format("{}\nEmail: {}", MetodoPago::obtenerDetalles(), email_);
    }
};

class Crypto : public MetodoPago
{
  private:
    std::string wallet_;
    std::string moneda_; // BTC, ETH, etc.

  public:
    Crypto(std::string id, double monto, std::string wallet, std::string moneda)
        : MetodoPago{std::move(id), monto}, wallet_{std::move(wallet)}, moneda_{std::move(moneda)}
    {
    }

    [[nodiscard]]
    std::string procesar() const override
    {
        return std::format("[Crypto] Transfiriendo ${:.2f} en {} a {}", monto_, moneda_, wallet_);
    }
};

// función polimórfica
void procesarPago(const MetodoPago& pago)
{
    fmt::println("=== Procesando Pago ===");
    fmt::println("{}", pago.obtenerDetalles());
    fmt::println("{}", pago.procesar());
    fmt::println("");
}

int main()
{
    try {
        // Crear diferentes métodos de pago
        // Usamos unique_ptr (RAII) en lugar de raw pointers
        auto tarjeta = std::make_unique<TarjetaCredito>("PAY-001", 99.99, "1234567890123456", "Juan Pérez");

        auto paypal = std::make_unique<PayPal>("PAY-002", 49.50, "juan@example.com");

        auto crypto = std::make_unique<Crypto>("PAY-003", 150.00, "0x1234abcd...", "ETH");

        // Polimorfismo: tratar todos como MetodoPago
        procesarPago(*tarjeta);
        procesarPago(*paypal);
        procesarPago(*crypto);

        // Vector de pagos (polimorfismo en contenedores)
        std::vector<std::unique_ptr<MetodoPago>> pagos;
        pagos.push_back(std::make_unique<TarjetaCredito>("PAY-004", 25.00, "9876543210987654", "María López"));
        pagos.push_back(std::make_unique<PayPal>("PAY-005", 75.00, "maria@example.com"));

        fmt::println("=== Procesando Múltiples Pagos ===");
        for (const auto& pago : pagos) {
            procesarPago(*pago);
        }

        // Verificación de tipo en runtime (similar a isinstance)
        const MetodoPago& primerPago = *pagos[0];
        if (dynamic_cast<const TarjetaCredito*>(&primerPago)) {
            fmt::println("El primer pago es con tarjeta");
        }

    } catch (const std::exception& e) {
        fmt::println(stderr, "Error: ", e.what());
    }

    return 0;
}
