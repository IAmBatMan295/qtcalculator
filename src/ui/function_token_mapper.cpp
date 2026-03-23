#include "calculator_ui.hpp"

FunctionTokenMapper::FunctionNamePair FunctionTokenMapper::Resolve_Function_Names(const QString &function_group_name, bool is_inverse_mode_enabled) {
    FunctionNamePair function_name_pair;

    if (function_group_name == "sin" || function_group_name == "cos" || function_group_name == "tan") {
        if (is_inverse_mode_enabled) {
            function_name_pair.display_function_name = "a" + function_group_name;
            function_name_pair.backend_function_name = "a" + function_group_name;
        } else {
            function_name_pair.display_function_name = function_group_name;
            function_name_pair.backend_function_name = function_group_name;
        }
        return function_name_pair;
    }

    if (function_group_name == "ln") {
        function_name_pair.display_function_name = is_inverse_mode_enabled ? "exp" : "ln";
        function_name_pair.backend_function_name = is_inverse_mode_enabled ? "exp" : "ln";
        return function_name_pair;
    }

    if (function_group_name == "log") {
        function_name_pair.display_function_name = is_inverse_mode_enabled ? "pow10" : "log";
        function_name_pair.backend_function_name = is_inverse_mode_enabled ? "powt" : "log";
        return function_name_pair;
    }

    function_name_pair.display_function_name = function_group_name;
    function_name_pair.backend_function_name = function_group_name;
    return function_name_pair;
}

QString FunctionTokenMapper::Build_Backend_Function_Token(const QString &backend_function_name, bool is_radian_mode_enabled) {
    if (is_radian_mode_enabled || !Is_Trig_Function(backend_function_name)) {
        return backend_function_name + "(";
    }

    if (Is_Inverse_Trig_Function(backend_function_name)) {
        return "(180/PI)*" + backend_function_name + "(";
    }

    return backend_function_name + "((PI/180)*";
}

bool FunctionTokenMapper::Is_Trig_Function(const QString &backend_function_name) {
    return backend_function_name == "sin" || backend_function_name == "cos" || backend_function_name == "tan" || Is_Inverse_Trig_Function(backend_function_name);
}

bool FunctionTokenMapper::Is_Inverse_Trig_Function(const QString &backend_function_name) {
    return backend_function_name == "asin" || backend_function_name == "acos" || backend_function_name == "atan";
}
