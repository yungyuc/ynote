#include <vector>
#include <stdio.h>
#include <boost/utility.hpp>

//#define USE_BOOST_SHARED_PTR
//#define USE_ENABLE_SHARED_FROM_THIS
//#define USE_MAKE_SHARED

#ifdef USE_BOOST_SHARED_PTR

#define SHARED_PTR boost::shared_ptr
#define WEAK_PTR boost::weak_ptr
#define ENABLE_SHARED_FROM_THIS boost::enable_shared_from_this
#define GET_DELETER boost::get_deleter

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/weak_ptr.hpp>

#else

#define SHARED_PTR std::shared_ptr
#define WEAK_PTR std::weak_ptr
#define ENABLE_SHARED_FROM_THIS std::enable_shared_from_this
#define GET_DELETER std::get_deleter

#include <memory>

#endif

class Object;

static WEAK_PTR<Object> gweak;

class Object
  : public boost::noncopyable
#ifdef USE_ENABLE_SHARED_FROM_THIS
  , public ENABLE_SHARED_FROM_THIS<Object>
#endif // USE_ENABLE_SHARED_FROM_THIS
{
public:
    Object(int value) : m_value(value) {
        printf("  Object::Object(value=%d) %p\n", m_value, this);
    };
    ~Object() {
        printf("  Object::~Object(value=%d) %p\n", m_value, this);
    };

    static SHARED_PTR<Object> g_strong;
    static WEAK_PTR<Object> g_weak;

    int m_value;

    static void print_status() {
        printf("  Object::print_status()");
        printf("\n");
        printf("    g_strong.use_count()=%ld", g_strong.use_count());
        printf(" g_strong.get()=%p", g_strong.get());
        printf(" &g_strong=%p", &g_strong);
        printf("\n");
        printf("    g_weak.use_count()=%ld", g_weak.use_count());
        printf(" &g_weak=%p", &g_weak);
        printf("\n");
#ifdef WTYPE_BPY
        printf("    get_deleter<shared_ptr_deleter>(g_strong)=%p",
               GET_DELETER<boost::python::converter::shared_ptr_deleter>(g_strong));
        printf("\n");
#endif // WTYPE_BPY
    }

    static SHARED_PTR<Object> get_strong() { return g_strong; }
    static size_t get_strong_count() { return g_strong.use_count(); }
    static void reset_strong() { g_strong.reset(); }
    static WEAK_PTR<Object> get_weak() { return g_weak; }
    static size_t get_weak_count() { return g_weak.use_count(); }
    static void reset_weak() { g_weak.reset(); }

    static SHARED_PTR<Object> make(int value) {
        SHARED_PTR<Object> & obj = g_strong;
        if (!obj) {
#ifdef USE_MAKE_SHARED
            obj = boost::make_shared<Object>(value);
#else // USE_MAKE_SHARED
            obj = SHARED_PTR<Object>(new Object(value));
#endif // USE_MAKE_SHARED
            g_weak = obj;
        }
        return obj;
    }
};

SHARED_PTR<Object> Object::g_strong = SHARED_PTR<Object>();
WEAK_PTR<Object> Object::g_weak = WEAK_PTR<Object>();

void print_from_python(const SHARED_PTR<Object> & obj) {
    printf("  print_from_python(obj)");
    printf("\n");
    printf("    obj.use_count()=%ld", obj.use_count());
    printf(" obj.get()=%p", obj.get());
    printf(" &obj=%p", &obj);
    printf("\n");
#ifdef WTYPE_BPY
    printf("    get_deleter<shared_ptr_deleter>(obj)=%p",
           GET_DELETER<boost::python::converter::shared_ptr_deleter>(obj));
    printf("\n");
#endif // WTYPE_BPY
}

size_t get_cpp_count(const SHARED_PTR<Object> & obj) {
    return obj.use_count();
}

struct Holder {
    Holder(const SHARED_PTR<Object> & obj)
#ifdef USE_ENABLE_SHARED_FROM_THIS
      : m_weak(obj->shared_from_this())
#else // USE_ENABLE_SHARED_FROM_THIS
      : m_weak(obj)
#endif // USE_ENABLE_SHARED_FROM_THIS
    {}
    WEAK_PTR<Object> m_weak;
    size_t get_count() const { return m_weak.use_count(); }
};

// vim: set ai et nu sw=4 ts=4:
