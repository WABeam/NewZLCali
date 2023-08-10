#ifndef TEST_FRAMEWORK_CORE_GLOBAL_HPP_
#define TEST_FRAMEWORK_CORE_GLOBAL_HPP_

#include <climits>
#include <type_traits>
#include <QtCore/QtGlobal>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QDebug>

#ifdef CORE_BUILD_STATIC
#define CORE_API
#else
#ifdef CORE_API_EXPORT
#define CORE_API Q_DECL_EXPORT
#else
#define CORE_API Q_DECL_IMPORT
#endif
#endif

namespace core {
namespace details {
template<class T, std::size_t... N>
constexpr std::remove_cv_t<T> bswapImpl(T i, std::index_sequence<N...>) noexcept
{
    return (((i >> N * CHAR_BIT & 0xff) << (sizeof(T) - 1 - N) * CHAR_BIT) | ...);
}
}
template<class T>
requires std::conjunction_v<std::is_unsigned<T>, std::negation<std::is_same<std::remove_cv_t<T>, bool>>>
constexpr std::remove_cv_t<T> bswap(T i) noexcept {
  return details::bswapImpl<T>(i, std::make_index_sequence<sizeof(T)>{});
}

struct NonCopyable {
protected:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class QObjectEventDeleter {
public:
    explicit QObjectEventDeleter(
            Qt::ConnectionType type = Qt::AutoConnection) noexcept
            : m_type(type) {}

    void operator()(QObject* obj) const noexcept {
        if (obj) {
            QMetaObject::invokeMethod(obj, &QObject::deleteLater, m_type);
        }
    }

private:
    Qt::ConnectionType m_type;
};

class QObjectOptionalDeleter {
public:
    using NeedDeleteFuncion = bool (*)(QObject*) noexcept;

    explicit QObjectOptionalDeleter(NeedDeleteFuncion f = nullptr) noexcept
            : m_f(normalize(f)) {}

    void operator()(QObject* obj) const noexcept {
        if (m_f(obj)) {
            qDebug() << "do delete.";
            delete obj;
        }
    }

private:
    NeedDeleteFuncion normalize(NeedDeleteFuncion f) const noexcept {
        return f ? f : [](QObject*) noexcept { return true; };
    }

    NeedDeleteFuncion m_f;
};

inline bool isOrphan(QObject* obj) noexcept { return obj && !obj->parent(); }

template <class T>
struct QtMetaTypeRegister {
    QtMetaTypeRegister() { qRegisterMetaType<T>(); }
};
}  // namespace core

#define CONCAT_X(x, y) x##y
#define CONCAT(x, y) CONCAT_X(x, y)

#define REGISTER_QT_METATYPE(Type, X)                            \
    Q_DECLARE_METATYPE(Type)                                     \
    inline static const auto CONCAT(qMetaTypeAutoRegister_, X) = \
            core::QtMetaTypeRegister<Type>{};

#endif  // TEST_FRAMEWORK_CORE_GLOBAL_HPP_
