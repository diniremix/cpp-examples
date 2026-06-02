#include <fmt/core.h>
#include <regex>
#include <string>

bool validate_email(std::string email, bool rfc_compatible)
{
    // fmt::println("validando: '{}'", email);

    std::regex pattern;

    if (rfc_compatible) {
        // más compatible con RFC 5322
        pattern = R"(^(?!\.)(?!.*\.\.)([a-zA-Z0-9!#$%&'*+/=?^_`{|}~.-]+)@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,})$)";
    } else {
        pattern = R"(^((?!\.)[\w\-_.+]*[^.])(@\w+)(\.\w+(\.\w+)?[^.\W])$)";
    }

    return std::regex_match(email, pattern);
}

int main()
{
    fmt::println("regex email validator demo");
    fmt::println("");
    fmt::println("compatibilidad con RFC 5322");
    fmt::println("");
    fmt::println("'usuarioejemplo.com': {}", validate_email("usuarioejemplo.com", true));                   // false
    fmt::println("'usuario@ejemplo.com': {}", validate_email("usuario@ejemplo.com", true));                 // true
    fmt::println("'testmail@mail.com.': {}", validate_email("testmail@mail.com.", true));                   // false
    fmt::println("'.testmail@mail.com': {}", validate_email(".testmail@mail.com", true));                   // false
    fmt::println("'testmail@mail.de.org': {}", validate_email("testmail@mail.de.org", true));               // true
    fmt::println("'testmail.demo@mail.rg': {}", validate_email("testmail.demo@mail.rg", true));             // true
    fmt::println("'lama.llama.loca123@inca.com': {}", validate_email("lama.llama.loca123@inca.com", true)); // true
    fmt::println("'testmail+demo@mail.com': {}", validate_email("testmail+demo@mail.com", true));           // true
    fmt::println("' testmail@mail.com': {}", validate_email(" testmail@mail.com", true));                   // false
    fmt::println("'testmail.demo+one@mail.com': {}", validate_email("testmail.demo+one@mail.com", true));   // true
    fmt::println("'testmail+two.three@mail.com': {}", validate_email("testmail+two.three@mail.com", true)); // true
    fmt::println("'testmail+two.three+four@mail.com': {}",
                 validate_email("testmail+two.three+four@mail.com", true));                                 // true
    fmt::println("'user.name+tag@example.co.uk': {}", validate_email("user.name+tag@example.co.uk", true)); // true
    fmt::println("'user@subdomain.domain.com': {}", validate_email("user@subdomain.domain.com", true));     // true
    fmt::println("'user..name@example.com': {}", validate_email("user..name@example.com", true));           // false

    fmt::println("");
    fmt::println("================");
    fmt::println("más sencillo");
    fmt::println("");
    fmt::println("'usuarioejemplo.com': {}", validate_email("usuarioejemplo.com", false));                   // false
    fmt::println("'usuario@ejemplo.com': {}", validate_email("usuario@ejemplo.com", false));                 // true
    fmt::println("'testmail@mail.com.': {}", validate_email("testmail@mail.com.", false));                   // false
    fmt::println("'.testmail@mail.com': {}", validate_email(".testmail@mail.com", false));                   // false
    fmt::println("'testmail@mail.de.org': {}", validate_email("testmail@mail.de.org", false));               // true
    fmt::println("'testmail.demo@mail.rg': {}", validate_email("testmail.demo@mail.rg", false));             // true
    fmt::println("'lama.llama.loca123@inca.com': {}", validate_email("lama.llama.loca123@inca.com", false)); // true
    fmt::println("'testmail+demo@mail.com': {}", validate_email("testmail+demo@mail.com", false));           // true
    fmt::println("' testmail@mail.com': {}", validate_email(" testmail@mail.com", false));                   // false
    fmt::println("'testmail.demo+one@mail.com': {}", validate_email("testmail.demo+one@mail.com", false));   // true
    fmt::println("'testmail+two.three@mail.com': {}", validate_email("testmail+two.three@mail.com", false)); // true
    fmt::println("'testmail+two.three+four@mail.com': {}",
                 validate_email("testmail+two.three+four@mail.com", false));                                 // true
    fmt::println("'user.name+tag@example.co.uk': {}", validate_email("user.name+tag@example.co.uk", false)); // true
    fmt::println("'user@subdomain.domain.com': {}", validate_email("user@subdomain.domain.com", false));     // true
    fmt::println("'user..name@example.com': {}", validate_email("user..name@example.com", false));           // true

    return 0;
}
