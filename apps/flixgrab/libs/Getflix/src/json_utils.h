#pragma once

#include <QtCore>


namespace jsonutils {
    
    template<QJsonValue::Type type>
    constexpr auto castJsonValue(const QJsonValue& value) {
        if constexpr (type == QJsonValue::Type::Bool)
            return value.toBool();
        else if constexpr (type == QJsonValue::Type::Object)
            return value.toObject();
        else if constexpr (type == QJsonValue::Type::String)
            return value.toString();
        else if constexpr (type == QJsonValue::Type::Double)
            return value.toDouble();
        else if constexpr (type == QJsonValue::Type::Array)
            return value.toArray();
        else
            static_assert(false, "Unknow json value type");
    }

    QString jsonTypeStr(QJsonValue::Type type) {
        switch (type) {
            case QJsonValue::Type::Null: return "Null";
            case QJsonValue::Type::Bool: return "Bool";
            case QJsonValue::Type::Double: return "Double";
            case QJsonValue::Type::String: return "String";
            case QJsonValue::Type::Array: return "Array";
            case QJsonValue::Type::Object: return "Object";
            case QJsonValue::Type::Undefined: return "Undefined";
        }
        
        throw std::logic_error("Unkown json value type");
    }

    bool checkJsonValueType(const QJsonValue& value, QJsonValue::Type expectedType) {
        if (value.type() != expectedType) {
            //qDebug() << "key is incorrect, actual: " << jsonTypeStr(value.type()) << " expected: " << jsonTypeStr(expectedType);
            return false;
        }
        if (value.isNull()) {
            qDebug() << "json value is null";
            return false;
        }
        if (value.isUndefined()) {
            qDebug() << "json value is undefined";
            return false;
        }
        
        return true;
    }

    template<QJsonValue::Type expectedType>
    auto getJsonValue(const QJsonValue& value, bool* ok) {
        assert(ok != nullptr);
        *ok = checkJsonValueType(value, expectedType);
        if (!ok)
            return decltype(castJsonValue<expectedType>({})){};
        
        return castJsonValue<expectedType>(value);
    }

    template<QJsonValue::Type expectedType>
    auto getJsonValue(const QJsonObject& object, const QString& key, bool *ok) {
        QJsonValue value = object[key];
        return getJsonValue<expectedType>(value, ok);
    }

    template<QJsonValue::Type expectedType>
    auto getJsonValue(const QJsonArray& array, int idx, bool *ok) {
        assert(ok != nullptr);
        if (idx >= array.size()) {
            qDebug() << "idx: " << idx << " is out of range. array size: " << array.size();
            ok = false;
            return decltype(castJsonValue<expectedType>({})){};
        }
        QJsonValue value = array[idx];
        
        return getJsonValue<expectedType>(value, ok);
    }
}