-- con la version actual de 'nlohmann_json'
-- junio 20, 2026
--[[
The package info of project:
    require(nlohmann_json):
      -> description: JSON for Modern C++
      -> version: v3.12.0
      -> license: MIT
      -> urls:
         -> default:
            -> https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.tar.gz
               -> 4b92eb0c06d10683f7447ce9406cb97cd4b453be18d7279320f7b2f025c10187
            -> https://github.com/nlohmann/json.git
               -> v3.12.0
--
--
tiene problemas utilizando `NLOHMANN_DEFINE_TYPE_INTRUSIVE` con campos `std::optional`
-- ejemplo de: open_meteo.cpp
--
--
[ 23%]: <open_meteo> cache compiling.release exercises/open_meteo.cpp
error: exercises/open_meteo.cpp:107:5: error: no matching function for call to ‘nlohmann::json_abi_v3_12_0::basic_json<>::get_to(std::optional<float>&) const’
  107 |     NLOHMANN_DEFINE_TYPE_INTRUSIVE(ResponseJson, latitude, longitude, generationtime_ms, utc_offset_seconds, timezone,
      |     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/home/diniremix/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include/nlohmann/json.hpp:1825:17: note: candidate: ‘template<class ValueType, typename std::enable_if<((! nlohmann::json_abi_v3_12_0::detail::is_basic_json<BasicJsonType>::value) && nlohmann::json_abi_v3_12_0::detail::has_from_json<nlohmann::json_abi_v3_12_0::basic_json<>, ValueType, void>::value), int>::type <anonymous> > ValueType& nlohmann::json_abi_v3_12_0::basic_json<ObjectType, ArrayType, StringType, BooleanType, NumberIntegerType, NumberUnsignedType, NumberFloatType, AllocatorType, JSONSerializer, BinaryType, CustomBaseClass>::get_to(ValueType&) const [with typename std::enable_if<((! nlohmann::json_abi_v3_12_0::detail::is_basic_json<BasicJsonType>::value) && nlohmann::json_abi_v3_12_0::detail::has_from_json<nlohmann::json_abi_v3_12_0::basic_json<ObjectType, ArrayType, StringType, BooleanType, NumberIntegerType, NumberUnsignedType, NumberFloatType, AllocatorType, JSONSerializer, BinaryType, CustomBaseClass>, ValueType>::value), int>::type <anonymous> = ValueType; ObjectType = std::map; ArrayType = std::vector; StringType = std::__cxx11::basic_string<char>; BooleanType = bool; NumberIntegerType = long int; NumberUnsignedType = long unsigned int; NumberFloatType = double; AllocatorType = std::allocator; JSONSerializer = nlohmann::json_abi_v3_12_0::adl_serializer; BinaryType = std::vector<unsigned char>; CustomBaseClass = void]’
 1825 |     ValueType & get_to(ValueType& v) const noexcept(noexcept(
      |                 ^~~~~~
/home/diniremix/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include/nlohmann/json.hpp:1825:17: note:   template argument deduction/substitution failed:
/home/diniremix/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include/nlohmann/json.hpp:1824:28: error: no type named ‘type’ in ‘struct std::enable_if<false, int>’
 1824 |                    int > = 0 >
      |                            ^
/home/diniremix/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include/nlohmann/json.hpp:1838:17: note: candidate: ‘template<class ValueType, typename std::enable_if<nlohmann::json_abi_v3_12_0::detail::is_basic_json<BasicJsonType>::value, int>::type <anonymous> > ValueType& nlohmann::json_abi_v3_12_0::basic_json<ObjectType, ArrayType, StringType, BooleanType, NumberIntegerType, NumberUnsignedType, NumberFloatType, AllocatorType, JSONSerializer, BinaryType, CustomBaseClass>::get_to(ValueType&) const [with typename std::enable_if<nlohmann::json_abi_v3_12_0::detail::is_basic_json<BasicJsonType>::value, int>::type <anonymous> = ValueType; ObjectType = std::map; ArrayType = std::vector; StringType = std::__cxx11::basic_string<char>; BooleanType = bool; NumberIntegerType = long int; NumberUnsignedType = long unsigned int; NumberFloatType = double; AllocatorType = std::allocator; JSONSerializer = nlohmann::json_abi_v3_12_0::adl_serializer; BinaryType = std::vector<unsigned char>; CustomBaseClass = void]’
 1838 |     ValueType & get_to(ValueType& v) const
      |                 ^~~~~~
/home/diniremix/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include/nlohmann/json.hpp:1838:17: note:   template argument deduction/substitution failed:
/home/diniremix/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include/nlohmann/json.hpp:1837:25: error: no type named ‘type’ in ‘struct std::enable_if<false, int>’
 1837 |                  int> = 0>
      |                         ^
  > in exercises/open_meteo.cpp
error: recipe `runx` failed on line 269 with exit code 255
]]

-- se utiliza la rama develop actual
add_requires("nlohmann_json_git")

package("nlohmann_json_git")
    set_kind("library", {headeronly = true})

    add_urls("https://github.com/nlohmann/json.git")
    -- add_versions("develop", "branch:develop") -- si
    add_versions("develop", "02dfbea39d94b8d19ddcd0eabfdc943a65bf3368")

    on_install(function(package)
        os.cp("single_include/nlohmann", package:installdir("include"))
    end)

    on_test(function(package)
        assert(package:has_cxxincludes("nlohmann/json.hpp"))
    end)
